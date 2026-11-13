
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "validaciones.h"

#define TAM_ENTRADA 256

// Elimina los caracteres restantes de la entrada actual.
void limpiarBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* Consumir caracteres */
    }
}

// Lee una linea completa desde el teclado.
static int leerLinea(char entrada[TAM_ENTRADA]) {
    size_t longitud;

    if (fgets(entrada, TAM_ENTRADA, stdin) == NULL) {
        printf("\n[ERROR] No se pudo leer la entrada.\n");
        exit(EXIT_FAILURE);
    }

    longitud = strlen(entrada);

    // Comprueba si la linea excede el tamaño permitido.
    if (longitud > 0 && entrada[longitud - 1] == '\n') {
        entrada[longitud - 1] = '\0';
    } else if (!feof(stdin)) {
        limpiarBuffer();
        return 0;
    }

    return 1;
}

// Comprueba si despues del numero solo hay espacios.
static int soloEspacios(const char *texto) {
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        texto++;
    }

    return 1;
}

// Lee un numero entero dentro de un rango.
int leerEnteroRango(const char *mensaje, int min, int max) {
    char entrada[TAM_ENTRADA];
    char *fin;
    long valor;
    int valido;

    if (min > max) {
        printf("[ERROR] Rango de enteros incorrecto.\n");
        exit(EXIT_FAILURE);
    }

    do {
        printf("%s", mensaje);

        if (!leerLinea(entrada)) {
            printf("[ERROR] La entrada es demasiado larga.\n");
            continue;
        }

        errno = 0;
        valor = strtol(entrada, &fin, 10);

        valido = (fin != entrada &&
                  errno != ERANGE &&
                  soloEspacios(fin));

        if (!valido) {
            printf("[ERROR] Debe ingresar un numero entero valido.\n");
        } else if (valor < min || valor > max) {
            printf("[ERROR] El numero debe estar en el rango [%d, %d].\n",
                   min, max);
            valido = 0;
        }

    } while (!valido);

    return (int)valor;
}

// Lee un numero decimal mayor o igual al minimo indicado.
double leerDoubleMinimo(const char *mensaje, double min) {
    char entrada[TAM_ENTRADA];
    char *fin;
    double valor;
    int valido;

    if (!isfinite(min)) {
        printf("[ERROR] Limite minimo incorrecto.\n");
        exit(EXIT_FAILURE);
    }

    do {
        printf("%s", mensaje);

        if (!leerLinea(entrada)) {
            printf("[ERROR] La entrada es demasiado larga.\n");
            continue;
        }

        errno = 0;
        valor = strtod(entrada, &fin);

        valido = (fin != entrada &&
                  errno != ERANGE &&
                  isfinite(valor) &&
                  soloEspacios(fin));

        if (!valido) {
            printf("[ERROR] Debe ingresar un numero decimal valido.\n");
        } else if (valor < min) {
            printf("[ERROR] El valor debe ser mayor o igual a %.2f.\n", min);
            valido = 0;
        }

    } while (!valido);

    return valor;
}

// Lee un numero decimal dentro de un rango.
double leerDoubleRango(const char *mensaje, double min, double max) {
    char entrada[TAM_ENTRADA];
    char *fin;
    double valor;
    int valido;

    if (!isfinite(min) || !isfinite(max) || min > max) {
        printf("[ERROR] Rango decimal incorrecto.\n");
        exit(EXIT_FAILURE);
    }

    do {
        printf("%s", mensaje);

        if (!leerLinea(entrada)) {
            printf("[ERROR] La entrada es demasiado larga.\n");
            continue;
        }

        errno = 0;
        valor = strtod(entrada, &fin);

        valido = (fin != entrada &&
                  errno != ERANGE &&
                  isfinite(valor) &&
                  soloEspacios(fin));

        if (!valido) {
            printf("[ERROR] Debe ingresar un numero decimal valido.\n");
        } else if (valor < min || valor > max) {
            printf("[ERROR] El valor debe estar en el rango [%.2f, %.2f].\n",
                   min, max);
            valido = 0;
        }

    } while (!valido);

    return valor;
}
