
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "simulador.h"
#include "validaciones.h"
#include "interfaz.h"

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int opcion;
    DatosSimulacion datos;

    mostrarBanner();

    do {
        printf("\n=======================================================\n");
        printf("              MENU PRINCIPAL - MISION SLS              \n");
        printf("=======================================================\n");
        printf("1. Ejecutar simulacion con datos reales de Artemis I\n");
        printf("2. Ingresar parametros de simulacion personalizados\n");
        printf("3. Modo Desafio: Control de Mision (3 intentos)\n");
        printf("4. Salir\n");

        opcion = leerEnteroRango("Seleccione una opcion [1-4]: ", 1, 4);

        if (opcion == 1) {
            // Parametros originales de la consigna.
            datos.v0 = 0.0;
            datos.y0 = 0.0;
            datos.a1 = 20.0;
            datos.t1 = 120.0;
            datos.a2 = 12.5;
            datos.h  = 160000.0;
            datos.g  = 9.80;

            correrMision(&datos);
        }
        else if (opcion == 2) {
            printf("\n--- INGRESO DE PARAMETROS PERSONALIZADOS ---\n");

            datos.v0 = leerDoubleMinimo(
                "Velocidad inicial v0 [m/s] (>= 0): ", 0.0);

            datos.y0 = leerDoubleMinimo(
                "Altitud inicial y0 [m] (>= 0): ", 0.0);

            datos.a1 = leerDoubleMinimo(
                "Aceleracion fase 1 a1 [m/s^2] (> 0): ", 0.01);

            datos.t1 = leerDoubleMinimo(
                "Tiempo de propulsion t1 [s] (>= 0): ", 0.0);

            datos.a2 = leerDoubleMinimo(
                "Aceleracion fase 2 a2 [m/s^2] (> 0): ", 0.01);

            datos.h = leerDoubleMinimo(
                "Altitud de apagado de motores h [m] (>= 0): ", 0.0);

            datos.g = leerDoubleMinimo(
                "Aceleracion de la gravedad g [m/s^2] (> 0): ", 0.01);

            correrMision(&datos);
        }
        else if (opcion == 3) {
            jugarModoDesafio();
        }

    } while (opcion != 4);

    printf("\nGracias por utilizar el Simulador Artemis!\n");

    return 0;
}
