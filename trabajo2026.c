#include <stdio.h>
#include <math.h>
#include <string.h>
#include "simulador.h"

int ejecutarSimulacion(
    const DatosSimulacion *datos,
    double resultados[CANT_RESULTADOS],
    char *error,
    size_t tamError
) {
    double v1, y1, v2, hMax, t2, tTotal;
    double *r = resultados;

    // 1. Validacion de punteros de entrada
    if (datos == NULL || resultados == NULL) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: datos o vector de resultados inexistentes.");
        }
        return 0;
    }

    // 2. Validacion de parametros fisicos
    if (datos->a1 <= 0 || datos->a2 <= 0 ||
        datos->g <= 0  || datos->t1 < 0  ||
        datos->v0 < 0  || datos->y0 < 0  ||
        datos->h < 0) {

        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: valores fisicos invalidos (aceleraciones y g deben ser mayores a 0).");
        }
        return 0;
    }

    // 3. Primera fase: desacople de los SRB
    v1 = datos->v0 + datos->a1 * datos->t1;
    y1 = datos->y0 + datos->v0 * datos->t1 + (datos->a1 * datos->t1 * datos->t1) / 2.0;

    if (datos->h < y1) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: la altitud de apagado h (%.2fm) es menor que la altitud en desacople (%.2fm).", datos->h, y1);
        }
        return 0;
    }

    // 4. Segunda fase: velocidad al apagar los motores y tiempos de vuelo
    v2 = sqrt(v1 * v1 + 2.0 * datos->a2 * (datos->h - y1));
    t2 = (v2 - v1) / datos->a2;
    tTotal = datos->t1 + t2;

    // 5. Tercera fase: ascenso por inercia hasta el apogeo (si fallaran motores)
    hMax = datos->h + (v2 * v2) / (2.0 * datos->g);

    // Almacenamiento de resultados mediante aritmetica de punteros
    *(r + VEL_DESACOPLE) = v1;
    *(r + ALT_DESACOPLE) = y1;
    *(r + VEL_APAGADO)   = v2;
    *(r + ALT_MAXIMA)    = hMax;
    *(r + TIEMPO_FASE2)  = t2;
    *(r + TIEMPO_TOTAL)  = tTotal;

    // 6. Comprobacion de resultados finitos (evita NaN o Inf)
    for (int i = 0; i < CANT_RESULTADOS; i++) {
        if (!isfinite(r[i])) {
            if (error != NULL && tamError > 0) {
                snprintf(error, tamError, "Error: resultado fuera del rango numerico (overflow/NaN).");
            }
            return 0;
        }
    }

    return 1;
}

int generarTelemetria(
    const DatosSimulacion *datos,
    PuntoTelemetria historial[MAX_TELEMETRIA],
    int *cantPuntos,
    char *error,
    size_t tamError
) {
    if (datos == NULL || historial == NULL || cantPuntos == NULL) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: punteros nulos al generar telemetria.");
        }
        return 0;
    }

    double v1 = datos->v0 + datos->a1 * datos->t1;
    double y1 = datos->y0 + datos->v0 * datos->t1 + 0.5 * datos->a1 * datos->t1 * datos->t1;

    if (datos->h < y1) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: h no puede ser menor a y1.");
        }
        return 0;
    }

    double v2 = sqrt(v1 * v1 + 2.0 * datos->a2 * (datos->h - y1));
    double t2 = (v2 - v1) / datos->a2;
    double tTotal = datos->t1 + t2;

    double paso = DELTA_T;
    if ((tTotal / paso) + 4 >= MAX_TELEMETRIA) {
        paso = tTotal / (MAX_TELEMETRIA - 5);
    }

    int idx = 0;
    PuntoTelemetria *p = historial;

    // Registro inicial en plataforma (t = 0)
    p[idx].tiempo = 0.0;
    p[idx].altitud = datos->y0;
    p[idx].velocidad = datos->v0;
    p[idx].fuerzasG = datos->a1 / datos->g;
    p[idx].fase = 1;
    snprintf(p[idx].evento, sizeof(p[idx].evento), "IGNICION Y DESPEGUE (SRB + NUCLEO)");
    idx++;

    // Fase 1: ascenso con SRB activos
    for (double t = paso; t < datos->t1 && idx < MAX_TELEMETRIA - 3; t += paso) {
        p[idx].tiempo = t;
        p[idx].altitud = datos->y0 + datos->v0 * t + 0.5 * datos->a1 * t * t;
        p[idx].velocidad = datos->v0 + datos->a1 * t;
        p[idx].fuerzasG = datos->a1 / datos->g;
        p[idx].fase = 1;
        snprintf(p[idx].evento, sizeof(p[idx].evento), "Ascenso Fase 1 (SRB activos)");
        idx++;
    }

    // Evento critico exacto en t = t1: Desacople de SRB
    if (datos->t1 > 0.0 && idx < MAX_TELEMETRIA - 2) {
        p[idx].tiempo = datos->t1;
        p[idx].altitud = y1;
        p[idx].velocidad = v1;
        p[idx].fuerzasG = datos->a2 / datos->g;
        p[idx].fase = 3;
        snprintf(p[idx].evento, sizeof(p[idx].evento), ">>> DESACOPLE DE COHETES SRB <<<");
        idx++;
    }

    // Fase 2: ascenso solo con motor del nucleo principal
    for (double t = datos->t1 + paso; t < tTotal && idx < MAX_TELEMETRIA - 1; t += paso) {
        double dt2 = t - datos->t1;
        p[idx].tiempo = t;
        p[idx].altitud = y1 + v1 * dt2 + 0.5 * datos->a2 * dt2 * dt2;
        p[idx].velocidad = v1 + datos->a2 * dt2;
        p[idx].fuerzasG = datos->a2 / datos->g;
        p[idx].fase = 2;
        snprintf(p[idx].evento, sizeof(p[idx].evento), "Ascenso Fase 2 (Motor nucleo)");
        idx++;
    }

    // Evento final exacto: Apagado de motores (MECO)
    p[idx].tiempo = tTotal;
    p[idx].altitud = datos->h;
    p[idx].velocidad = v2;
    p[idx].fuerzasG = 0.0;
    p[idx].fase = 3;
    snprintf(p[idx].evento, sizeof(p[idx].evento), ">>> APAGADO DE MOTORES (MECO) <<<");
    idx++;

    *cantPuntos = idx;
    return 1;
}

int evaluarDesafio(
    const DatosSimulacion *datos,
    const double resultados[CANT_RESULTADOS],
    char *diagnostico,
    size_t tamDiag
) {
    if (datos == NULL || resultados == NULL || diagnostico == NULL || tamDiag == 0) {
        return 0;
    }

    double gMax = datos->a1 / datos->g;
    double v2 = resultados[VEL_APAGADO];

    if (gMax > LIMITE_G_TRIPULACION) {
        snprintf(diagnostico, tamDiag,
                 "FALLO POR SOBRECARGA G: %.2f g supera el limite seguro de %.2f g para la tripulacion.",
                 gMax, LIMITE_G_TRIPULACION);
        return 0;
    }

    if (v2 < VEL_OBJETIVO_MIN) {
        snprintf(diagnostico, tamDiag,
                 "VELOCIDAD INSUFICIENTE: v2 = %.2f m/s (se requiere al menos %.2f m/s para insercion orbital).",
                 v2, VEL_OBJETIVO_MIN);
        return 0;
    }

    if (v2 > VEL_OBJETIVO_MAX) {
        snprintf(diagnostico, tamDiag,
                 "EXCESO DE VELOCIDAD: v2 = %.2f m/s supera la ventana orbital maxima de %.2f m/s.",
                 v2, VEL_OBJETIVO_MAX);
        return 0;
    }

    snprintf(diagnostico, tamDiag,
             "MISION EXITOSA: Insercion a %.2f m/s con carga segura de %.2f g.",
             v2, gMax);
    return 1;
}
