
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
     // Primera fase: desacople de los SRB.
    v1 = datos->v0 + datos->a1 * datos->t1;

    y1 = datos->y0 +
         datos->v0 * datos->t1 +
         (datos->a1 * datos->t1 * datos->t1) / 2.0;

    // La segunda fase requiere que la altitud
    // de apagado sea igual o superior a y1.
    if (datos->h < y1) {
        snprintf(error, tamError,
                 "Error: la altura de apagado es menor que la altura de desacople.");
        return 0;
    }

    // Segunda fase: velocidad al apagar los motores.
    v2 = sqrt(v1 * v1 +
              2.0 * datos->a2 * (datos->h - y1));

    // Tercera fase: ascenso por inercia.
    hMax = datos->h + (v2 * v2) / (2.0 * datos->g);

