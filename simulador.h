
#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stddef.h>

#define CANT_RESULTADOS 4

#define VEL_DESACOPLE 0
#define ALT_DESACOPLE 1
#define VEL_APAGADO 2
#define ALT_MAXIMA 3

typedef struct {
    double v0;
    double y0;
    double a1;
    double t1;
    double a2;
    double h;
    double g;
} DatosSimulacion;

// Realiza todos los calculos de la simulacion.
int ejecutarSimulacion(
    const DatosSimulacion *datos,
    double resultados[CANT_RESULTADOS],
    char *error,
    size_t tamError
);

#endif
