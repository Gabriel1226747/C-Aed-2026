#include <stdio.h>
#include <stdlib.h>
#include "validaciones.h"

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leerEnteroRango(const char *mensaje, int min, int max) {
    int valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%d", &valor);
        if (resultado != 1) {
            printf("[ERROR] Entrada invalida. Debe ingresar un numero entero.\n");
            limpiarBuffer();
        } else if (valor < min || valor > max) {
            printf("[ERROR] El numero debe estar en el rango [%d, %d].\n", min, max);
        }
    } while (resultado != 1 || valor < min || valor > max);
    limpiarBuffer();
    return valor;
}

double leerDoubleMinimo(const char *mensaje, double min) {
    double valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%lf", &valor);
        if (resultado != 1) {
            printf("[ERROR] Entrada invalida. Debe ingresar un numero decimal.\n");
            limpiarBuffer();
        } else if (valor < min) {
            printf("[ERROR] El valor debe ser mayor o igual a %.2f.\n", min);
        }
    } while (resultado != 1 || valor < min);
    limpiarBuffer();
    return valor;
}

double leerDoubleRango(const char *mensaje, double min, double max) {
    double valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%lf", &valor);
        if (resultado != 1) {
            printf("[ERROR] Entrada invalida. Debe ingresar un numero decimal.\n");
            limpiarBuffer();
        } else if (valor < min || valor > max) {
            printf("[ERROR] El valor debe estar en el rango [%.2f, %.2f].\n", min, max);
        }
    } while (resultado != 1 || valor < min || valor > max);
    limpiarBuffer();
    return valor;
}