
#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "simulador.h"

// Muestra el banner inicial del programa.
void mostrarBanner(void);

// Ejecuta una simulacion y muestra su telemetria y reporte.
void correrMision(const DatosSimulacion *datos);

// Ejecuta el modo desafio de control de mision.
void jugarModoDesafio(void);

#endif
