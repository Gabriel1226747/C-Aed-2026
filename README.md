\# Simulador de Misiones Artemis – Escenario 1



\*\*Universidad Tecnológica Nacional – Facultad Regional Resistencia\*\*  

\*\*Carrera:\*\* Ingeniería en Sistemas de Información  

\*\*Asignatura:\*\* Algoritmos y Estructuras de Datos  

\*\*Trabajo Final Integrador – Laboratorio de C\*\*



\---



\## 1. Integrantes



\* \*\*Comisión:\*\* K1.2 ISI B

\* \*\*Equipo de desarrollo:\*\*

&#x20; \* Gabriel Damian LEDESMA TARAGANO

&#x20; \* Renata VALERO TRENQUE

&#x20; \* Tobias Maximiliano Vilaqui

&#x20; \* Ramiro Agustin Merlo

&#x20; \* Andrés Luciano Almirón



\---



\## 2. Descripción del proyecto



El proyecto consiste en desarrollar un simulador interactivo por consola, utilizando el lenguaje C, basado en el proceso de ascenso y propulsión del cohete \*\*Space Launch System (SLS)\*\*, perteneciente al programa Artemis de la NASA.



El simulador representa las dos etapas principales del ascenso vertical de la nave Orion, considerando el Movimiento Rectilíneo Uniformemente Variado (MRUV) y el desprendimiento de los cohetes propulsores sólidos (SRB).



\### Escenario 1: Ascenso y Propulsión con el SLS

El cohete parte del reposo desde la plataforma de lanzamiento y experimenta dos fases de aceleración:



\* \*\*Primera fase:\*\*

&#x20; \* Velocidad inicial: `0 m/s`.

&#x20; \* Aceleración: `20 m/s²`.

&#x20; \* Tiempo de duración: `120 segundos`.

&#x20; \* Al finalizar, se produce el desprendimiento de los propulsores SRB.

\* \*\*Segunda fase:\*\*

&#x20; \* Aceleración: `12,5 m/s²`.

&#x20; \* Continúa el ascenso desde las condiciones finales de la primera fase.

&#x20; \* Los motores se apagan al alcanzar los `160.000 metros` de altitud.



\*(Se considera despreciable la resistencia del aire durante el ascenso).\*



\---



\## 3. Objetivos del simulador



El programa permite:

1\. Calcular la velocidad y la altura alcanzadas al momento del desprendimiento de los SRB.

2\. Determinar la velocidad de la nave al alcanzar los 160 km de altitud.

3\. Calcular la altura máxima alcanzada ante una falla de los motores y posterior movimiento bajo la aceleración gravitatoria (`g = 9,80 m/s²`).

4\. Expresar la aceleración de la primera fase como múltiplo de la gravedad terrestre.



\---



\## 4. Funcionamiento del programa



El sistema presenta un menú principal desde el cual el usuario puede seleccionar el tipo de simulación que desea realizar. Se contemplan dos modalidades:



\* \*\*Simulación con parámetros establecidos:\*\* utiliza los valores físicos definidos en el escenario Artemis.

\* \*\*Simulación personalizada:\*\* permite ingresar nuevos parámetros para realizar cálculos dentro de las restricciones físicas del modelo.



Al finalizar los cálculos, el sistema presenta un reporte detallado con los resultados obtenidos.



\---



\## 5. Organización y arquitectura del código



El proyecto se encuentra organizado en módulos para separar las responsabilidades y facilitar el mantenimiento del programa:



```text

C-Aed-2026/

├── main.c           # Punto de entrada y menú principal

├── simulador.h      # Estructuras de datos, constantes y prototipos

├── simulador.c      # Cálculos físicos (MRUV) y lógica de simulación

├── validaciones.h   # Prototipos de control de entrada

├── validaciones.c   # Validación de tipos de datos y rangos

├── Makefile         # Automatización de compilación

└── README.md        # Documentación del proyecto

```



| Archivo | Responsabilidad |

| :--- | :--- |

| `main.c` | Menú principal y flujo general del programa. |

| `simulador.h` | Definición de estructuras, constantes y prototipos. |

| `simulador.c` | Desarrollo de los cálculos físicos y lógica de simulación. |

| `validaciones.h` | Declaración de las funciones de validación. |

| `validaciones.c` | Control y validación de los datos ingresados. |

| `Makefile` | Automatización del proceso de compilación. |



\---



\## 6. Requisitos y herramientas



\* \*\*Lenguaje de programación:\*\* C (Estándar C11)

\* \*\*Compilador:\*\* GCC

\* \*\*Control de versiones:\*\* Git

\* \*\*Repositorio:\*\* GitHub

\* \*\*Entorno de ejecución:\*\* Consola o terminal de comandos



\---



\## 7. Compilación y ejecución



Para compilar el proyecto utilizando el `Makefile`:

```bash

make

```

\*(En Windows con MinGW puede ejecutarse como `mingw32-make`).\*



Para ejecutar el simulador:

```bash

\# En Linux / macOS / Git Bash:

./artemis



\# En Windows (CMD / PowerShell):

artemis.exe

```



Para eliminar los archivos objeto (`.o`) y ejecutables generados durante la compilación:

```bash

make clean

```



\---



\## 8. Registro del uso de Inteligencia Artificial



Se utilizaron herramientas de Inteligencia Artificial Generativa como apoyo durante el desarrollo del proyecto:



\* \*\*Herramientas utilizadas:\*\* ChatGPT, Claude y Gemini.

\* \*\*Tareas realizadas:\*\*

&#x20; \* Consulta y comprensión de conceptos de programación en C (punteros, estructuras y modularización).

&#x20; \* Asistencia en la estructura del `Makefile`, flujo de GitHub y documentación en Markdown.

&#x20; \* Verificación de cálculos físicos y fórmulas de MRUV correspondientes al escenario.

&#x20; \* Propuestas de casos de prueba y validación de entradas por consola.

\* \*\*Ejemplos de consultas realizadas:\*\*

&#x20; \* \*"Cómo validar entradas numéricas en C limpiando el buffer para evitar bucles infinitos."\*

&#x20; \* \*"Cómo estructurar un Makefile multiplataforma para compilar varios archivos .c y .h."\*

\* \*\*Validación y adaptación:\*\*

&#x20; \* Todas las sugerencias proporcionadas fueron revisadas, probadas y contrastadas con los requisitos de la consigna. Cada fragmento incorporado fue comprendido y adaptado por los integrantes del equipo mediante pruebas de escritorio y ejecución en consola.



\---

\*Proyecto académico – Algoritmos y Estructuras de Datos – UTN FRRe.\*



