*Este proyecto ha sido creado como parte del currículo de 42 por gecasas.*

# ft_printf

> Reimplementation of the standard C `printf()` function, focused on variadic functions, format parsing and output conversion.

<details>
<summary><strong>English</strong></summary>

## Description

`ft_printf` is a project from the 42 curriculum in which I reimplemented the standard C `printf()` function.

The main goal of the project is to understand how variadic functions work in C and how a function can process a variable number of arguments depending on a format string.

My implementation supports all mandatory conversions required by the subject:

* `%c` — character
* `%s` — string
* `%p` — pointer in hexadecimal
* `%d` — signed decimal integer
* `%i` — signed decimal integer
* `%u` — unsigned decimal integer
* `%x` — hexadecimal in lowercase
* `%X` — hexadecimal in uppercase
* `%%` — percent character

The project is implemented as a static library named `libftprintf.a` and can be reused in other C projects.

I did not implement the optional bonus part of the project.

---

## Table of Contents

* [Description](#description)
* [Function Reference](#function-reference)
* [Algorithm and Data Structure](#algorithm-and-data-structure)
* [Project Structure](#project-structure)
* [Instructions](#instructions)
* [Using ft_printf](#using-ft_printf)
* [Quality and Validation](#quality-and-validation)
* [Memory Management](#memory-management)
* [What I Learned](#what-i-learned)
* [AI Usage](#ai-usage)
* [Resources](#resources)
* [Author](#author)

---

## Function Reference

| Category    | Function    | Description                                                                                         |
| ----------- | ----------- | --------------------------------------------------------------------------------------------------- |
| Output      | `ft_printf` | Processes the format string, retrieves the corresponding arguments and prints the requested output. |
| Character   | `%c`        | Prints a single character.                                                                          |
| String      | `%s`        | Prints a null-terminated string.                                                                    |
| Pointer     | `%p`        | Prints a pointer address in hexadecimal notation.                                                   |
| Integer     | `%d`        | Prints a signed decimal integer.                                                                    |
| Integer     | `%i`        | Prints a signed decimal integer.                                                                    |
| Unsigned    | `%u`        | Prints an unsigned decimal integer.                                                                 |
| Hexadecimal | `%x`        | Prints an integer in lowercase hexadecimal.                                                         |
| Hexadecimal | `%X`        | Prints an integer in uppercase hexadecimal.                                                         |
| Format      | `%%`        | Prints the `%` character.                                                                           |

### Supporting functions

I separated the different conversion and output operations into helper functions instead of keeping the complete implementation inside `ft_printf()`. The project also reuses functions from my Libft implementation where appropriate.

Some of the internal functionality includes:

* Format conversion selection.
* Character and string output.
* Signed and unsigned integer handling.
* Decimal and hexadecimal number conversion.
* Pointer formatting.
* Memory allocation and release for generated strings.

---

## Algorithm and Data Structure

The core of my implementation is based on a sequential parsing algorithm.

First, `ft_printf()` traverses the format string character by character. Normal characters are written directly to the output. When a `%` character is found, the following character is interpreted as a conversion specifier.

The conversion specifier is passed to a format selector, which determines which operation must be performed. Each supported conversion is handled separately, keeping the implementation modular and making each part easier to understand and maintain.

For conversions that require an argument, I use the `va_list` mechanism provided by `<stdarg.h>`. The argument is retrieved according to the expected type using `va_arg()` and is then processed by the corresponding conversion function.

For numerical conversions, the implementation converts the value into the required representation before writing it. Decimal and hexadecimal conversions use dedicated helper functions, while pointer values are formatted as hexadecimal addresses.

The project does not require a complex data structure. The main structures used by the implementation are the format string, the `va_list` object used to access the variable arguments, and dynamically allocated character strings when a conversion requires one.

I chose this approach because the problem is naturally based on the relationship between a format string and a variable number of arguments. Keeping the parsing, conversion selection and individual output operations separated makes the implementation easier to follow and allows each conversion to be handled independently.

This design also makes the implementation extensible: additional format specifiers could be integrated into the selector and their corresponding handling functions without rewriting the entire parsing process.

---

## Project Structure

```text
ft_printf/
├── Makefile
├── ft_printf.h
├── ft_printf.c
├── ft_format_selector.c
├── ft_utoa.c
├── ft_xtoa.c
└── libft/
    └── ...
```

The project uses my Libft as an external dependency of `ft_printf`. The Libft source is not included in this repository archive, but it is included and correctly configured in the submitted project.

The implementation is divided into several source files so that the main printing logic, conversion selection and numerical conversions remain separated.

---

## Instructions

### Compilation

The project can be compiled using:

```bash
make
```

This creates:

```text
libftprintf.a
```

in the root directory.

The Makefile uses:

```text
cc -Wall -Wextra -Werror
```

and the required rules are available:

```bash
make
make clean
make fclean
make re
```

The project uses `ar` to create the static library, as required by the subject.

### Makefile rules

| Rule     | Description                                    |
| -------- | ---------------------------------------------- |
| `all`    | Builds `libftprintf.a`.                        |
| `clean`  | Removes object files.                          |
| `fclean` | Removes object files and the compiled library. |
| `re`     | Cleans the project and recompiles it.          |

---

## Using ft_printf

After compiling the project, `libftprintf.a` can be linked into another C project.

For example, when compiling a program that uses the library:

```bash
cc main.c -L. -lftprintf
```

The corresponding header must also be included in the source file:

```c
#include "ft_printf.h"
```

This allows `ft_printf()` to be used in other projects in the same way as a regular static library function.

---

## Quality and Validation

I validated the implementation through several checks during development.

* Norminette
* Tripouille tester
* sfabi28 tester
* Compilation with `-Wall -Wextra -Werror`
* Memory leak checks
* Comparison of the supported conversions with the behaviour expected from the original `printf()`

The implementation supports all mandatory conversions from the subject and does not include the optional bonus features.

---

## Memory Management

Some conversions require dynamically allocated memory, particularly when numerical values are converted into strings.

I made sure that allocated memory is released once it is no longer required. Memory management was also checked during testing to ensure that the implementation does not produce leaks under the tested cases.

The project therefore follows the memory management requirements of the 42 curriculum.

---

## What I Learned

The main concept I learned from this project was how variadic functions work in C.

I learned how to use `va_list`, `va_start`, `va_arg` and `va_end` to create a function capable of receiving an indeterminate number of arguments.

I also learned how the type and meaning of each argument can depend on information contained in another argument, in this case the format string.

Beyond variadic functions, the project helped me improve my ability to:

* Parse and process formatted strings.
* Organize a project into independent functions.
* Handle different integer representations.
* Work with pointers and dynamically allocated memory.
* Separate conversion logic from output logic.
* Check and handle possible errors during execution.
* Build and link a static library.

The most challenging part for me was understanding and correctly using `va_list` and the mechanisms surrounding variadic arguments.

---

## AI Usage

I used AI during this project primarily as a didactic tool.

My use of AI was focused mainly on:

* Resolving specific doubts about `va_list` and variadic functions.
* Understanding how variadic arguments work in C.
* Clarifying isolated concepts that I did not fully understand.
* Checking specific errors or behaviours in particular situations.
* Reviewing code that I had already written in order to better understand possible problems.

I did not use AI to generate the implementation of the project.

**100% of the project code was written by me.**

The purpose of using AI was to support my understanding of the concepts and help me identify and understand errors, rather than to obtain a finished solution.

This approach was especially useful when working with variadic functions, since understanding how `va_list` behaves was one of the main challenges of the project.

---

## Resources

### Documentation and references

* `printf(3)` — Linux manual page.
* `stdarg(3)` — documentation for variadic argument handling.
* C standard library documentation.
* 42 ft_printf subject — Version 12.0.
* My previous Libft implementation and its associated functions.

### AI

I used AI as a learning and debugging aid, mainly to understand variadic functions, `va_list`, and specific errors encountered during development.

I did not use AI-generated code in the final implementation.

---

## Author

**gecasas**

42 Málaga student.

GitHub: https://github.com/gecasas

</details>

<details>
<summary><strong>Español</strong></summary>

## Descripción

`ft_printf` es un proyecto del currículo de 42 en el que he reimplementado la función estándar `printf()` de C.

El objetivo principal del proyecto es comprender cómo funcionan las funciones variádicas en C y cómo una función puede procesar un número indeterminado de argumentos dependiendo de una cadena de formato.

Mi implementación soporta todas las conversiones obligatorias indicadas en el subject:

* `%c` — carácter
* `%s` — cadena de caracteres
* `%p` — puntero en hexadecimal
* `%d` — entero decimal con signo
* `%i` — entero decimal con signo
* `%u` — entero decimal sin signo
* `%x` — hexadecimal en minúsculas
* `%X` — hexadecimal en mayúsculas
* `%%` — símbolo de porcentaje

El proyecto se compila como una librería estática llamada `libftprintf.a`, que puede reutilizarse en otros proyectos de C.

No he implementado la parte bonus opcional del proyecto.

---

## Índice

* [Descripción](#descripción)
* [Referencia de funciones](#referencia-de-funciones)
* [Algoritmo y estructura de datos](#algoritmo-y-estructura-de-datos)
* [Estructura del proyecto](#estructura-del-proyecto)
* [Instrucciones](#instrucciones)
* [Uso de ft_printf](#uso-de-ft_printf)
* [Calidad y validación](#calidad-y-validación)
* [Gestión de memoria](#gestión-de-memoria)
* [Qué he aprendido](#qué-he-aprendido)
* [Uso de IA](#uso-de-ia)
* [Recursos](#recursos)
* [Autor](#autor)

---

## Referencia de funciones

| Categoría   | Función     | Descripción                                                                                              |
| ----------- | ----------- | -------------------------------------------------------------------------------------------------------- |
| Salida      | `ft_printf` | Procesa la cadena de formato, obtiene los argumentos correspondientes e imprime el resultado solicitado. |
| Carácter    | `%c`        | Imprime un único carácter.                                                                               |
| Cadena      | `%s`        | Imprime una cadena de caracteres terminada en `\0`.                                                      |
| Puntero     | `%p`        | Imprime una dirección de memoria en formato hexadecimal.                                                 |
| Entero      | `%d`        | Imprime un entero decimal con signo.                                                                     |
| Entero      | `%i`        | Imprime un entero decimal con signo.                                                                     |
| Sin signo   | `%u`        | Imprime un entero decimal sin signo.                                                                     |
| Hexadecimal | `%x`        | Imprime un entero en hexadecimal utilizando letras minúsculas.                                           |
| Hexadecimal | `%X`        | Imprime un entero en hexadecimal utilizando letras mayúsculas.                                           |
| Formato     | `%%`        | Imprime el carácter `%`.                                                                                 |

### Funciones auxiliares

He separado las diferentes operaciones de conversión y salida en funciones auxiliares en lugar de concentrar toda la implementación dentro de `ft_printf()`.

El proyecto también reutiliza funciones de mi implementación de Libft cuando es necesario.

Entre las funcionalidades internas se encuentran:

* Selección de la conversión correspondiente.
* Escritura de caracteres y cadenas.
* Gestión de enteros con y sin signo.
* Conversión decimal y hexadecimal.
* Formateo de punteros.
* Reserva y liberación de memoria para las cadenas generadas.

---

## Algoritmo y estructura de datos

La implementación se basa principalmente en un algoritmo de análisis secuencial de la cadena de formato.

`ft_printf()` recorre la cadena de formato carácter por carácter. Los caracteres normales se escriben directamente en la salida. Cuando encuentra un `%`, interpreta el carácter siguiente como un especificador de conversión.

Ese especificador se pasa a un selector de formato, que determina qué operación debe realizarse. Cada conversión se gestiona de manera independiente, manteniendo separadas las diferentes partes de la implementación y facilitando su comprensión y mantenimiento.

Para las conversiones que necesitan recibir argumentos, utilizo el mecanismo `va_list` proporcionado por `<stdarg.h>`. El argumento se obtiene mediante `va_arg()` utilizando el tipo correspondiente y posteriormente se procesa mediante la función encargada de esa conversión.

En las conversiones numéricas, el valor se transforma a la representación necesaria antes de escribirlo. Las conversiones decimal y hexadecimal utilizan funciones auxiliares específicas, mientras que los punteros se representan como direcciones en hexadecimal.

El proyecto no necesita una estructura de datos compleja. Las principales estructuras utilizadas son la propia cadena de formato, el objeto `va_list` empleado para acceder a los argumentos variables y las cadenas de caracteres reservadas dinámicamente cuando una conversión las necesita.

Elegí este enfoque porque el problema se basa naturalmente en la relación entre una cadena de formato y un número variable de argumentos. Separar el análisis de la cadena, la selección de la conversión y las operaciones individuales de salida permite mantener cada parte del código independiente.

Esta organización también facilita una posible ampliación de la función, ya que nuevas conversiones podrían integrarse en el selector y disponer de su propia lógica sin tener que modificar completamente el sistema de análisis.

---

## Estructura del proyecto

```text
ft_printf/
├── Makefile
├── ft_printf.h
├── ft_printf.c
├── ft_format_selector.c
├── ft_utoa.c
├── ft_xtoa.c
└── libft/
    └── ...
```

El proyecto utiliza mi implementación de Libft como dependencia de `ft_printf`. El código de Libft no está incluido en este archivo comprimido, pero sí está incluido y correctamente configurado en la entrega del proyecto.

La implementación está dividida en varios archivos para mantener separadas la lógica principal de impresión, la selección de conversiones y las conversiones numéricas.

---

## Instrucciones

### Compilación

Para compilar el proyecto:

```bash
make
```

Esto genera:

```text
libftprintf.a
```

en la raíz del proyecto.

El Makefile utiliza:

```text
cc -Wall -Wextra -Werror
```

y contiene las reglas requeridas:

```bash
make
make clean
make fclean
make re
```

La librería estática se genera utilizando `ar`, tal y como requiere el subject.

### Reglas del Makefile

| Regla    | Descripción                                          |
| -------- | ---------------------------------------------------- |
| `all`    | Compila `libftprintf.a`.                             |
| `clean`  | Elimina los archivos objeto.                         |
| `fclean` | Elimina los archivos objeto y la librería compilada. |
| `re`     | Limpia el proyecto y vuelve a compilarlo.            |

---

## Uso de ft_printf

Una vez compilado el proyecto, `libftprintf.a` puede enlazarse con otro proyecto de C.

Por ejemplo:

```bash
cc main.c -L. -lftprintf
```

El header correspondiente debe incluirse en el código:

```c
#include "ft_printf.h"
```

De esta forma, `ft_printf()` puede utilizarse desde otros proyectos como una función de una librería estática.

---

## Calidad y validación

He validado la implementación mediante diferentes comprobaciones durante el desarrollo.

* Norminette
* Tester de Tripouille
* Tester de sfabi28
* Compilación con `-Wall -Wextra -Werror`
* Comprobaciones de memory leaks
* Comparación de las conversiones implementadas con el comportamiento esperado del `printf()` original

La implementación incluye todas las conversiones obligatorias del subject y no incluye las funcionalidades opcionales del bonus.

---

## Gestión de memoria

Algunas conversiones necesitan memoria dinámica, especialmente cuando los valores numéricos se convierten en cadenas de caracteres.

Me he asegurado de liberar la memoria reservada cuando deja de ser necesaria. También he realizado comprobaciones de memoria durante las pruebas para detectar posibles leaks.

De esta forma, la implementación cumple los requisitos de gestión de memoria establecidos por el currículo de 42.

---

## Qué he aprendido

El principal concepto que he aprendido durante este proyecto ha sido el funcionamiento de las funciones variádicas en C.

He aprendido a utilizar `va_list`, `va_start`, `va_arg` y `va_end` para crear una función capaz de recibir un número indeterminado de argumentos.

También he aprendido cómo el tipo y el significado de cada argumento pueden depender de la información contenida en otro argumento, en este caso la cadena de formato.

Además de las funciones variádicas, el proyecto me ha permitido mejorar en:

* Análisis y procesamiento de cadenas de formato.
* Organización de un proyecto mediante funciones independientes.
* Gestión de diferentes representaciones numéricas.
* Uso de punteros y memoria dinámica.
* Separación entre lógica de conversión y salida.
* Comprobación y gestión de errores.
* Creación y enlazado de librerías estáticas.

La parte que más me costó fue comprender y utilizar correctamente `va_list` y todo el mecanismo relacionado con los argumentos variádicos.

---

## Uso de IA

He utilizado IA durante este proyecto principalmente como una herramienta didáctica.

Mi uso de IA se ha centrado principalmente en:

* Resolver dudas concretas sobre `va_list` y las funciones variádicas.
* Comprender cómo funcionan los argumentos variádicos en C.
* Aclarar conceptos aislados que no terminaba de comprender.
* Comprobar errores o comportamientos concretos en situaciones específicas.
* Revisar código que yo ya había escrito para comprender mejor posibles problemas.

No he utilizado IA para generar la implementación del proyecto.

**El 100% del código del proyecto ha sido escrito por mí.**

El objetivo del uso de IA ha sido apoyar mi comprensión de los conceptos y ayudarme a identificar y entender errores, no obtener una solución terminada.

Este enfoque me resultó especialmente útil al trabajar con las funciones variádicas, ya que comprender correctamente el funcionamiento de `va_list` fue uno de los principales retos del proyecto.

---

## Recursos

### Documentación y referencias

* `printf(3)` — documentación del manual de Linux.
* `stdarg(3)` — documentación relacionada con los argumentos variádicos.
* Documentación de la biblioteca estándar de C.
* Subject de `ft_printf` de 42 — versión 12.0.
* Mi implementación anterior de Libft y sus funciones asociadas.

### IA

He utilizado IA como herramienta de aprendizaje y apoyo durante la depuración, principalmente para comprender las funciones variádicas, `va_list` y errores concretos encontrados durante el desarrollo.

No he incorporado código generado por IA a la implementación final.

---

## Autor

**gecasas**

Estudiante de 42 Málaga.

GitHub: https://github.com/gecasas

</details>
