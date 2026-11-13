
#include <stdio.h>
#include <math.h>
#include "simulador.h"

// Comprueba que todos los parametros de la simulacion sean validos.
static int validarDatos(
    const DatosSimulacion *datos,
    char *error,
    size_t tamError
) {
    if (datos == NULL) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: datos de simulacion inexistentes.");
        }
        return 0;
    }

    // Verificacion de valores finitos.
    if (!isfinite(datos->v0) ||
        !isfinite(datos->y0) ||
        !isfinite(datos->a1) ||
        !isfinite(datos->t1) ||
        !isfinite(datos->a2) ||
        !isfinite(datos->h)  ||
        !isfinite(datos->g)) {

        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: todos los parametros deben ser valores numericos finitos.");
        }
        return 0;
    }

    // Verificacion de rangos fisicos.
    if (datos->a1 <= 0.0 ||
        datos->a2 <= 0.0 ||
        datos->g <= 0.0 ||
        datos->t1 < 0.0 ||
        datos->v0 < 0.0 ||
        datos->y0 < 0.0 ||
        datos->h < 0.0) {

        if (error != NULL && tamError > 0) {
            snprintf(error, tamError,
                     "Error: aceleraciones y gravedad deben ser positivas; tiempo, velocidad y altitudes no pueden ser negativos.");
        }
        return 0;
    }

    return 1;
}


// Realiza los calculos analiticos generales de la simulacion.
int ejecutarSimulacion(
    const DatosSimulacion *datos,
    double resultados[CANT_RESULTADOS],
    char *error,
    size_t tamError
) {
    double v1, y1, v2, hMax, t2, tTotal;
    double *r;

    // Validacion de punteros.
    if (resultados == NULL) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: vector de resultados inexistente.");
        }
        return 0;
    }

    if (!validarDatos(datos, error, tamError)) {
        return 0;
    }

    r = resultados;

    // Primera fase: desacople de los SRB.
    v1 = datos->v0 + datos->a1 * datos->t1;

    y1 = datos->y0
       + datos->v0 * datos->t1
       + (datos->a1 * datos->t1 * datos->t1) / 2.0;

    // Verificacion de resultados intermedios.
    if (!isfinite(v1) || !isfinite(y1)) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: desbordamiento numerico en la primera fase.");
        }
        return 0;
    }

    if (datos->h < y1) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError,
                     "Error: la altitud de apagado h (%.2f m) es menor que la altitud de desacople (%.2f m).",
                     datos->h, y1);
        }
        return 0;
    }

    // Segunda fase: velocidad al apagar los motores.
    double radicando = v1 * v1
                     + 2.0 * datos->a2 * (datos->h - y1);

    if (!isfinite(radicando) || radicando < 0.0) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: no se puede calcular la velocidad final.");
        }
        return 0;
    }

    v2 = sqrt(radicando);

    t2 = (v2 - v1) / datos->a2;
    tTotal = datos->t1 + t2;

    // Tercera fase: ascenso por inercia hasta el apogeo.
    hMax = datos->h + (v2 * v2) / (2.0 * datos->g);

    // Comprobacion de resultados finitos.
    if (!isfinite(v2) ||
        !isfinite(t2) ||
        !isfinite(tTotal) ||
        !isfinite(hMax)) {

        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: resultado fuera del rango numerico (overflow/NaN).");
        }
        return 0;
    }

    // Almacenamiento mediante aritmetica de punteros.
    *(r + VEL_DESACOPLE) = v1;
    *(r + ALT_DESACOPLE) = y1;
    *(r + VEL_APAGADO) = v2;
    *(r + ALT_MAXIMA) = hMax;
    *(r + TIEMPO_FASE2) = t2;
    *(r + TIEMPO_TOTAL) = tTotal;

    return 1;
}


// Genera la secuencia temporal de telemetria y eventos.
int generarTelemetria(
    const DatosSimulacion *datos,
    PuntoTelemetria historial[MAX_TELEMETRIA],
    int *cantPuntos,
    char *error,
    size_t tamError
) {
    double resultados[CANT_RESULTADOS];
    double v1, y1, v2, tTotal;
    double paso;
    int idx = 0;

    // Validacion de punteros.
    if (historial == NULL || cantPuntos == NULL) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: punteros nulos al generar telemetria.");
        }
        return 0;
    }

    // Validacion independiente de los datos.
    if (!ejecutarSimulacion(datos, resultados, error, tamError)) {
        return 0;
    }

    v1 = resultados[VEL_DESACOPLE];
    y1 = resultados[ALT_DESACOPLE];
    v2 = resultados[VEL_APAGADO];
    tTotal = resultados[TIEMPO_TOTAL];

    // Ajuste del paso para respetar la capacidad del arreglo.
    paso = DELTA_T;

    if ((tTotal / paso) + 4.0 >= MAX_TELEMETRIA) {
        paso = tTotal / (MAX_TELEMETRIA - 5);
    }

    if (paso <= 0.0 || !isfinite(paso)) {
        paso = DELTA_T;
    }

    // Registro inicial en plataforma (t = 0).
    historial[idx].tiempo = 0.0;
    historial[idx].altitud = datos->y0;
    historial[idx].velocidad = datos->v0;
    historial[idx].fuerzasG = datos->a1 / datos->g;
    historial[idx].fase = 1;

    snprintf(historial[idx].evento,
             sizeof(historial[idx].evento),
             "IGNICION Y DESPEGUE (SRB + NUCLEO)");

    idx++;

    // Fase 1: ascenso con SRB activos.
    for (double t = paso;
         t < datos->t1 && idx < MAX_TELEMETRIA - 2;
         t += paso) {

        historial[idx].tiempo = t;

        historial[idx].altitud =
            datos->y0 + datos->v0 * t
            + 0.5 * datos->a1 * t * t;

        historial[idx].velocidad = datos->v0 + datos->a1 * t;
        historial[idx].fuerzasG = datos->a1 / datos->g;
        historial[idx].fase = 1;

        snprintf(historial[idx].evento,
                 sizeof(historial[idx].evento),
                 "Ascenso Fase 1 (SRB activos)");

        idx++;
    }

    // Evento critico exacto: desacople de los SRB.
    if (idx < MAX_TELEMETRIA - 1) {
        historial[idx].tiempo = datos->t1;
        historial[idx].altitud = y1;
        historial[idx].velocidad = v1;
        historial[idx].fuerzasG = datos->a2 / datos->g;
        historial[idx].fase = 3;

        snprintf(historial[idx].evento,
                 sizeof(historial[idx].evento),
                 ">>> DESACOPLE DE COHETES SRB <<<");

        idx++;
    }

    // Fase 2: ascenso con el motor del nucleo principal.
    for (double dt = paso;
         datos->t1 + dt < tTotal && idx < MAX_TELEMETRIA - 1;
         dt += paso) {

        historial[idx].tiempo = datos->t1 + dt;

        historial[idx].altitud =
            y1 + v1 * dt + 0.5 * datos->a2 * dt * dt;

        historial[idx].velocidad = v1 + datos->a2 * dt;
        historial[idx].fuerzasG = datos->a2 / datos->g;
        historial[idx].fase = 2;

        snprintf(historial[idx].evento,
                 sizeof(historial[idx].evento),
                 "Ascenso Fase 2 (Motor nucleo)");

        idx++;
    }

    // Evento final exacto: apagado de motores (MECO).
    if (idx >= MAX_TELEMETRIA) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: capacidad de telemetria superada.");
        }
        return 0;
    }

    historial[idx].tiempo = tTotal;
    historial[idx].altitud = datos->h;
    historial[idx].velocidad = v2;
    historial[idx].fuerzasG = 0.0;
    historial[idx].fase = 3;

    snprintf(historial[idx].evento,
             sizeof(historial[idx].evento),
             ">>> APAGADO DE MOTORES (MECO) <<<");

    idx++;

    *cantPuntos = idx;

    return 1;
}


// Evalua si una simulacion cumple los objetivos del Modo Desafio.
int evaluarDesafio(
    const DatosSimulacion *datos,
    const double resultados[CANT_RESULTADOS],
    char *diagnostico,
    size_t tamDiag
) {
    double resultadosCalculados[CANT_RESULTADOS];
    double aMax;
    double gMax;
    double v2;

    if (diagnostico == NULL || tamDiag == 0) {
        return 0;
    }

    if (resultados == NULL) {
        snprintf(diagnostico, tamDiag,
                 "ERROR: vector de resultados inexistente.");
        return 0;
    }

    // Se comprueba que los datos y los calculos sean validos.
    if (!ejecutarSimulacion(
            datos,
            resultadosCalculados,
            diagnostico,
            tamDiag)) {
        return 0;
    }

    // Se utilizan los resultados recalculados para evitar datos inconsistentes.
    aMax = (datos->a1 > datos->a2) ? datos->a1 : datos->a2;
    gMax = aMax / datos->g;

    v2 = resultadosCalculados[VEL_APAGADO];

    // Control de desacople antes de alcanzar la altitud objetivo.
    if (resultadosCalculados[ALT_DESACOPLE] >= datos->h) {
        snprintf(diagnostico, tamDiag,
                 "FALLO: los SRB deben desacoplarse antes de alcanzar la altitud objetivo.");
        return 0;
    }

    // Control de aceleracion maxima.
    if (gMax > LIMITE_G_TRIPULACION) {
        snprintf(diagnostico, tamDiag,
                 "FALLO POR SOBRECARGA G: %.2f g supera el limite seguro de %.2f g para la tripulacion.",
                 gMax, LIMITE_G_TRIPULACION);
        return 0;
    }

    // Control de velocidad minima.
    if (v2 < VEL_OBJETIVO_MIN) {
        snprintf(diagnostico, tamDiag,
                 "VELOCIDAD INSUFICIENTE: v2 = %.2f m/s (se requiere al menos %.2f m/s).",
                 v2, VEL_OBJETIVO_MIN);
        return 0;
    }

    // Control de velocidad maxima.
    if (v2 > VEL_OBJETIVO_MAX) {
        snprintf(diagnostico, tamDiag,
                 "EXCESO DE VELOCIDAD: v2 = %.2f m/s supera la ventana objetivo maxima de %.2f m/s.",
                 v2, VEL_OBJETIVO_MAX);
        return 0;
    }

    snprintf(diagnostico, tamDiag,
             "MISION EXITOSA: velocidad final de %.2f m/s con carga maxima de %.2f g.",
             v2, gMax);

    return 1;
}
