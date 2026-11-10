#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// Limpia el búfer de entrada para evitar lecturas erróneas
void limpiarBuffer(void);

// Lee un entero asegurando que esté en el rango [min, max]
int leerEnteroRango(const char *mensaje, int min, int max);

// Lee un número decimal validando que sea mayor o igual a un valor mínimo
double leerDoubleMinimo(const char *mensaje, double min);

// Lee un número decimal validando que esté dentro del rango [min, max]
double leerDoubleRango(const char *mensaje, double min, double max);

#endif