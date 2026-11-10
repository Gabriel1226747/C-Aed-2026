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
    double v1, y1, v2, hMax;
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

    // La altitud de apagado h debe ser mayor o igual a y1
    if (datos->h < y1) {
        if (error != NULL && tamError > 0) {
            snprintf(error, tamError, "Error: la altitud de apagado h (%.2fm) es menor que la altitud en desacople (%.2fm).", datos->h, y1);
        }
        return 0;
    }

    // 4. Segunda fase: velocidad al apagar los motores
    v2 = sqrt(v1 * v1 + 2.0 * datos->a2 * (datos->h - y1));

    // 5. Tercera fase: ascenso por inercia hasta el apogeo
    hMax = datos->h + (v2 * v2) / (2.0 * datos->g);   
        
    // Almacenamiento de resultados en el vector
    *(r + VEL_DESACOPLE) = v1;
    *(r + ALT_DESACOPLE) = y1;
    *(r + VEL_APAGADO)   = v2;
    *(r + ALT_MAXIMA)    = hMax;

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
