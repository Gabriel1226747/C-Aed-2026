
#include <stdio.h>
#include <math.h>
#include "simulador.h"

int ejecutarSimulacion(
    const DatosSimulacion *datos,
    double resultados[CANT_RESULTADOS],
    char *error,
    size_t tamError
) {
    double v1, y1, v2, hMax;
    double *r = resultados;

    // Validacion de los datos.
    if (datos == NULL || resultados == NULL) {
        snprintf(error, tamError, "Error: datos inexistentes.");
        return 0;
    }

    if (datos->a1 <= 0 || datos->a2 <= 0 ||
        datos->g <= 0 || datos->t1 < 0 ||
        datos->v0 < 0 || datos->y0 < 0 ||
        datos->h < 0) {

        snprintf(error, tamError, "Error: valores fisicos invalidos.");
        return 0;
    }
    
