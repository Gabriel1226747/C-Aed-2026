#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stddef.h>

#define CANT_RESULTADOS 6
#define MAX_TELEMETRIA  200
#define DELTA_T         15.0

#define VEL_DESACOPLE 0
#define ALT_DESACOPLE 1
#define VEL_APAGADO   2
#define ALT_MAXIMA    3
#define TIEMPO_FASE2  4
#define TIEMPO_TOTAL  5

// Limites fisicos del Modo Desafio (Control de Mision)
#define MAX_INTENTOS_DESAFIO 3
#define LIMITE_G_TRIPULACION 3.5
#define VEL_OBJETIVO_MIN     2500.0
#define VEL_OBJETIVO_MAX     2900.0

typedef struct {
    double v0;
    double y0;
    double a1;
    double t1;
    double a2;
    double h;
    double g;
} DatosSimulacion;

typedef struct {
    double tiempo;       // Tiempo transcurrido [s]
    double altitud;      // Altitud actual [m]
    double velocidad;    // Velocidad instantanea [m/s]
    double fuerzasG;     // Aceleracion experimentada en multiplos de g
    int fase;            // 1: SRB + Nucleo | 2: Solo Nucleo | 3: Evento critico
    char evento[48];     // Descripcion del estado o evento en ese instante
} PuntoTelemetria;

// Realiza los calculos analiticos generales de la simulacion.
int ejecutarSimulacion(
    const DatosSimulacion *datos,
    double resultados[CANT_RESULTADOS],
    char *error,
    size_t tamError
);

// Genera la secuencia temporal de telemetria y eventos en un arreglo de structs.
int generarTelemetria(
    const DatosSimulacion *datos,
    PuntoTelemetria historial[MAX_TELEMETRIA],
    int *cantPuntos,
    char *error,
    size_t tamError
);

// Evalua si una simulacion cumple los objetivos del Modo Desafio.
// Retorna 1 si la mision fue exitosa, 0 si fallo (detallando el motivo en diagnostico).
int evaluarDesafio(
    const DatosSimulacion *datos,
    const double resultados[CANT_RESULTADOS],
    char *diagnostico,
    size_t tamDiag
);

#endif