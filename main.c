#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"
#include "validaciones.h"

void mostrarReporte(const double resultados[CANT_RESULTADOS], double a1, double g) {
    printf("\n=======================================================\n");
    printf("         REPORTE DE SIMULACIÓN - ARTEMIS (SLS)         \n");
    printf("=======================================================\n");
    printf("a. Rapidez al desacople (v1):     %.2f m/s (%.2f km/h)\n", 
           resultados[VEL_DESACOPLE], resultados[VEL_DESACOPLE] * 3.6);
    printf("   Altitud recorrida (y1):        %.2f m (%.2f km)\n", 
           resultados[ALT_DESACOPLE], resultados[ALT_DESACOPLE] / 1000.0);
    printf("b. Rapidez máxima al apagar (v2): %.2f m/s (%.2f km/h)\n", 
           resultados[VEL_APAGADO], resultados[VEL_APAGADO] * 3.6);
    printf("c. Altitud máxima en caída libre: %.2f m (%.2f km)\n", 
           resultados[ALT_MAXIMA], resultados[ALT_MAXIMA] / 1000.0);
    printf("d. Aceleración máxima (fase 1):    %.2f g\n", a1 / g);
    printf("=======================================================\n\n");
}

int main(void) {
    int opcion;

    do {
        printf("\n=======================================================\n");
        printf("    SIMULADOR ARTEMIS - ESCENARIO 1: ASCENSO SLS       \n");
        printf("=======================================================\n");
        printf("1. Ejecutar simulación con datos reales de Artemis I\n");
        printf("2. Ingresar parámetros de simulación personalizados\n");
        printf("3. Salir\n");

        opcion = leerEnteroRango("Seleccione una opción [1-3]: ", 1, 3);

    } while (opcion != 3);

    printf("\n¡Gracias por utilizar el Simulador Artemis!\n");
    return 0;
}