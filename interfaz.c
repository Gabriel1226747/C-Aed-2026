
#include <stdio.h>

#include "interfaz.h"
#include "validaciones.h"

static void mostrarTelemetria(
    const PuntoTelemetria *historial,
    int cantPuntos,
    double hObjetivo
) {
    printf("\n=============================================================================================\n");
    printf("                         TELEMETRIA DE VUELO Y EVENTOS - MISION ARTEMIS                      \n");
    printf("=============================================================================================\n");
    printf("%-8s | %-11s | %-12s | %-12s | %-6s | %s\n",
           "Tiempo", "Altitud", "Progreso", "Velocidad", "Carga", "Estado / Evento");
    printf("---------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < cantPuntos; i++) {
        const PuntoTelemetria *p = historial + i;
        int bloques = 0;

        if (hObjetivo > 0.0) {
            bloques = (int)((p->altitud / hObjetivo) * 10.0);

            if (bloques > 10) {
                bloques = 10;
            }

            if (bloques < 0) {
                bloques = 0;
            }
        }

        char barra[13];

        barra[0] = '[';

        for (int b = 0; b < 10; b++) {
            barra[b + 1] = (b < bloques) ? '#' : '.';
        }

        barra[11] = ']';
        barra[12] = '\0';

        if (p->fase == 3) {
            printf("---------------------------------------------------------------------------------------------\n");
        }

        printf("%6.1fs | %8.2f km | %s | %8.2f m/s | %4.2fg | %s\n",
               p->tiempo,
               p->altitud / 1000.0,
               barra,
               p->velocidad,
               p->fuerzasG,
               p->evento);

        if (p->fase == 3) {
            printf("---------------------------------------------------------------------------------------------\n");
        }
    }

    printf("=============================================================================================\n");
}

static void mostrarReporte(
    const double resultados[CANT_RESULTADOS],
    double a1,
    double g
) {
    printf("\n=======================================================\n");
    printf("         REPORTE ANALITICO FINAL - ARTEMIS (SLS)       \n");
    printf("=======================================================\n");

    printf("a. Rapidez al desacople (v1):       %.2f m/s (%.2f km/h)\n",
           resultados[VEL_DESACOPLE],
           resultados[VEL_DESACOPLE] * 3.6);

    printf("   Altitud recorrida (y1):          %.2f m (%.2f km)\n",
           resultados[ALT_DESACOPLE],
           resultados[ALT_DESACOPLE] / 1000.0);

    printf("b. Rapidez maxima al apagar (v2):   %.2f m/s (%.2f km/h)\n",
           resultados[VEL_APAGADO],
           resultados[VEL_APAGADO] * 3.6);

    printf("   Duracion Fase 2 (t2):            %.2f s\n",
           resultados[TIEMPO_FASE2]);

    printf("   Tiempo total hasta MECO:         %.2f s (%.2f min)\n",
           resultados[TIEMPO_TOTAL],
           resultados[TIEMPO_TOTAL] / 60.0);

    printf("c. Altitud maxima (falla motores):  %.2f m (%.2f km)\n",
           resultados[ALT_MAXIMA],
           resultados[ALT_MAXIMA] / 1000.0);

    printf("d. Aceleracion maxima (fase 1):     %.2f g\n",
           a1 / g);

    printf("=======================================================\n\n");
}

void mostrarBanner(void) {
    printf("\n=====================================================================\n");
    printf("          /\\                                                         \n");
    printf("         /  \\      NASA ARTEMIS PROGRAM - SPACE LAUNCH SYSTEM (SLS)  \n");
    printf("        |    |     Simulador de Ascenso, Desacople SRB y Telemetria  \n");
    printf("       /| || |\\    UTN FRRe - Algoritmos y Estructuras de Datos      \n");
    printf("      /_| || |_\\                                                     \n");
    printf("=====================================================================\n");
}

void correrMision(const DatosSimulacion *datos) {
    double resultados[CANT_RESULTADOS];
    PuntoTelemetria historial[MAX_TELEMETRIA];
    int cantPuntos = 0;
    char error[256];

    if (ejecutarSimulacion(datos, resultados, error, sizeof(error))) {

        if (generarTelemetria(
                datos,
                historial,
                &cantPuntos,
                error,
                sizeof(error))) {

            mostrarTelemetria(historial, cantPuntos, datos->h);
            mostrarReporte(resultados, datos->a1, datos->g);

        } else {
            printf("\n[ERROR DE TELEMETRIA] %s\n\n", error);
        }

    } else {
        printf("\n[ERROR DE SIMULACION] %s\n\n", error);
    }
}

void jugarModoDesafio(void) {
    DatosSimulacion datos;
    double resultados[CANT_RESULTADOS];
    PuntoTelemetria historial[MAX_TELEMETRIA];

    char error[256];
    char diagnostico[256];

    int cantPuntos = 0;
    int misionCumplida = 0;

    // Condiciones fijas de la mision.
    datos.v0 = 0.0;
    datos.y0 = 0.0;
    datos.h  = 160000.0;
    datos.g  = 9.80;

    printf("\n=====================================================================\n");
    printf("           MODO DESAFIO: CONTROL DE MISION ARTEMIS (3 TURNOS)        \n");
    printf("=====================================================================\n");

    printf("OBJETIVO:\n");
    printf("  - Alcanzar la altitud de apagado h = 160.0 km (160000 m).\n");
    printf("  - Lograr velocidad final v2 entre %.0f m/s y %.0f m/s.\n",
           VEL_OBJETIVO_MIN,
           VEL_OBJETIVO_MAX);

    printf("  - NO superar %.1f g de aceleracion en Fase 1 (seguridad tripulacion).\n",
           LIMITE_G_TRIPULACION);

    printf("  - Desacoplar los SRB (y1) antes de alcanzar los 160.0 km.\n");

    printf("=====================================================================\n");

    for (int intento = 1;
         intento <= MAX_INTENTOS_DESAFIO && !misionCumplida;
         intento++) {

        printf("\n--- INTENTO %d DE %d ---\n",
               intento,
               MAX_INTENTOS_DESAFIO);

        datos.a1 = leerDoubleRango(
            "Aceleracion Fase 1 a1 [5.0 - 50.0 m/s^2]: ",
            5.0,
            50.0);

        datos.t1 = leerDoubleRango(
            "Tiempo de propulsion SRB t1 [30.0 - 200.0 s]: ",
            30.0,
            200.0);

        datos.a2 = leerDoubleRango(
            "Aceleracion Fase 2 a2 [2.0 - 30.0 m/s^2]: ",
            2.0,
            30.0);

        if (!ejecutarSimulacion(
                &datos,
                resultados,
                error,
                sizeof(error))) {

            printf("\n[ABORTADO EN VUELO] %s\n", error);
            continue;
        }

        if (generarTelemetria(
                &datos,
                historial,
                &cantPuntos,
                error,
                sizeof(error))) {

            mostrarTelemetria(historial, cantPuntos, datos.h);

        } else {
            printf("\n[ERROR DE TELEMETRIA] %s\n", error);
        }

        misionCumplida = evaluarDesafio(
            &datos,
            resultados,
            diagnostico,
            sizeof(diagnostico));

        printf("\n*********************************************************************\n");
        printf(" RESULTADO TURNO %d: %s\n",
               intento,
               diagnostico);
        printf("*********************************************************************\n");
    }

    if (!misionCumplida) {
        printf("\n[MISION FRACASADA] Se agotaron los %d intentos de lanzamiento.\n\n",
               MAX_INTENTOS_DESAFIO);
    }
}
