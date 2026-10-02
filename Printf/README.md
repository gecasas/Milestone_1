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

The project is built as a static library named `libftprintf.a` and can be reused in other C projects.

I did not implement the optional bonus part of the project.

---

## Table of Contents

* [Description](#description)
* [Function Reference](#function-reference)
* [Algorithm and Data Structure](#algorithm-and-data-structure)
* [Technical Decisions](#technical-decisions)
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

### Internal functions

| Function             | File                | Role                                                                                   |
| -------------------- | ------------------- | -------------------------------------------------------------------------------------- |
| `ft_parse_format`    | `ft_printf.c`       | Walks the format string, prints text and conversions, and returns the total or `-1`.   |
| `ft_format_selector` | `ft_printf.c`       | Receives the character after `%` and calls the matching handler.                       |
| `ft_handle_p`        | `ft_printf.c`       | Prints a pointer as `0x` + lowercase hexadecimal, or `(nil)` for `NULL`.               |
| `ft_handle_c`        | `ft_printf_utils.c` | Prints one character (including `\0`).                                                 |
| `ft_handle_s`        | `ft_printf_utils.c` | Prints a string, or `(null)` for `NULL`.                                               |
| `ft_handle_d`        | `ft_printf_utils.c` | Prints a signed integer using `ft_itoa` from my Libft. Used for both `%d` and `%i`.    |
| `ft_handle_u`        | `ft_printf_utils.c` | Prints an unsigned integer using `ft_utoa`.                                            |
| `ft_handle_x`        | `ft_printf_utils.c` | Prints a hexadecimal number; a parameter selects lowercase (`%x`) or uppercase (`%X`). |
| `ft_utoa`            | `ft_toas.c`         | Converts an `unsigned int` into a newly allocated decimal string.                      |
| `ft_xtoa`            | `ft_toas.c`         | Converts an `unsigned long` into a newly allocated hexadecimal string.                 |
| `ft_hexlen`          | `ft_toas.c`         | Returns the number of hexadecimal digits of a number.                                  |
| `ft_handle_percent`  | `ft_toas.c`         | Prints `%`. It does not consume any variadic argument.                                 |

---

## Algorithm and Data Structure

The core of my implementation is a sequential parsing algorithm with a dispatcher.

`ft_printf()` only checks that the format is not `NULL`, starts the `va_list`, delegates the work to `ft_parse_format()` and closes the `va_list` with `va_end()`.

`ft_parse_format()` traverses the format string character by character. Normal characters are written directly to the output. When a `%` character is found, the index moves to the next character, which is interpreted as a conversion specifier and passed to `ft_format_selector()`.

The selector only receives that single character, not the whole string. This keeps all the index logic inside `ft_parse_format()`, so there is only one place where the position in the format string is moved.

The selector calls one handler per conversion. Every handler follows the same contract: it extracts its argument with `va_arg()`, writes the output with `write()`, and returns the number of characters written, or `-1` if something failed. Because all handlers return the same kind of value, `ft_parse_format()` can check for errors in a single place after each step, and add the result to the total count otherwise.

For numerical conversions, the value is first converted into a dynamically allocated string (`ft_itoa`, `ft_utoa` or `ft_xtoa`) and then written with a single `write()` call.

The project does not need a complex data structure. The main elements are the format string, the `va_list` object used to access the variable arguments, and the temporary strings allocated by numerical conversions.

I chose this approach because it separates three responsibilities: walking the format string, choosing the conversion, and producing each conversion's output. Each part can be understood, tested and modified independently.

---

## Technical Decisions

* **Handlers receive `va_list *` instead of `va_list`.** If a `va_list` is passed by value to a function that calls `va_arg()` on it, the C standard leaves the caller's copy in an indeterminate state. It happens to work on Linux x86_64, where `va_list` is an array, but it is not portable. Passing a pointer guarantees that every handler advances the same `va_list` owned by `ft_printf()`.
* **Error propagation.** Every call to `write()` or to a function that allocates memory is checked. A failure returns `-1`, which is propagated up through `ft_parse_format()` to `ft_printf()`, which returns `-1` like the original `printf()`. `va_end()` is called in a single place, in `ft_printf()`, on both the success and the error path. A return of `0` is not an error (for example, `%s` with an empty string).
* **`NULL` format.** The C standard does not allow a `NULL` format, but glibc does not crash: `printf(NULL)` prints nothing and returns `-1`. `ft_printf()` does the same, checking the format before calling `va_start()`, so no `va_end()` is needed on that path.
* **`%c` reads an `int`.** A `char` passed to a variadic function is promoted to `int`, so the handler asks `va_arg()` for an `int` and converts it to `unsigned char`.
* **`%d` and `%i` share one handler**, because they produce the same output in `printf()`.
* **`ft_xtoa` uses `unsigned long`.** The same conversion is shared by `%x`, `%X` and `%p`. `%x` receives a 32-bit `unsigned int`, but a pointer on Linux x86_64 is 64 bits, so the conversion uses the wider type to avoid truncating addresses.
* **`NULL` values** follow glibc: `%s` prints `(null)` and `%p` prints `(nil)`.
* **Unknown specifiers and a trailing `%`.** The C standard defines both as undefined behaviour. My selector returns `0` for any unknown character: nothing is printed and it is not treated as an error. When `%` is the last character of the string, the index stops on the terminating `\0` instead of skipping it, so the loop never reads outside the string.

---

## Project Structure

```text
ft_printf/
├── Makefile
├── README.md
├── ft_printf.h
├── ft_printf.c
├── ft_printf_utils.c
├── ft_toas.c
└── libft/
    ├── Makefile
    ├── libft.h
    └── ft_*.c
```

| File                | Contents                                                        |
| ------------------- | --------------------------------------------------------------- |
| `ft_printf.c`       | `ft_printf`, `ft_parse_format`, `ft_format_selector` and `ft_handle_p`. |
| `ft_printf_utils.c` | Handlers for `%c`, `%s`, `%d`/`%i`, `%u` and `%x`/`%X`.         |
| `ft_toas.c`         | Number-to-string conversions, `ft_hexlen` and the `%%` handler. |
| `ft_printf.h`       | Prototypes and includes.                                        |

The project uses my Libft, which is included in the `libft/` directory with its own source files and Makefile. The main Makefile compiles Libft first and then adds the `ft_printf` objects to the same library.

---

## Instructions

### Compilation

The project can be compiled using:

```bash
make
```

This first builds `libft/libft.a` and then creates:

```text
libftprintf.a
```

in the root directory.

The Makefile uses:

```text
cc -Wall -Wextra -Werror
```

The static library is created with `ar`, as required by the subject. Running `make` a second time does not recompile or relink anything.

### Makefile rules

| Rule     | Description                                                        |
| -------- | ------------------------------------------------------------------ |
| `all`    | Builds Libft and `libftprintf.a`.                                  |
| `clean`  | Removes object files (including Libft's).                          |
| `fclean` | Removes object files and the compiled libraries.                   |
| `re`     | Cleans the project and recompiles it.                              |

---

## Using ft_printf

After compiling the project, `libftprintf.a` can be linked into another C project:

```bash
cc main.c -L. -lftprintf
```

The header must also be included in the source file:

```c
#include "ft_printf.h"
```

---

## Quality and Validation

* Norminette on all source and header files.
* Compilation with `-Wall -Wextra -Werror`.
* Tripouille tester.
* sfabi28 tester (all 161 mandatory tests, including `ft_printf(NULL)`).
* Output and return value compared with the original `printf()` for edge cases: `%c` with `0`, `%s` with `NULL` and `""`, `INT_MIN`/`INT_MAX`, `%u` and `%x` with `-1`, `%p` with `NULL` and with the maximum address, and a `NULL` format.
* `write()` failure: with standard output closed (`./a.out >&-`), `ft_printf()` returns `-1`.
* Memory leaks checked with `valgrind --leak-check=full`.

---

## Memory Management

`%d`, `%u`, `%x`, `%X` and `%p` use dynamically allocated strings. Each handler frees them on every exit path, both when `write()` succeeds and when it fails. If an allocation fails, the handler frees anything it had already allocated and returns `-1`.

---

## What I Learned

The main concept I learned from this project was how variadic functions work in C: how to use `va_list`, `va_start`, `va_arg` and `va_end`, how default argument promotions affect the type requested with `va_arg`, and why a `va_list` must be passed by pointer when several functions share it.

Beyond variadic functions, the project helped me improve my ability to:

* Parse and process formatted strings.
* Organize a project into independent functions with a common contract.
* Handle signed, unsigned and hexadecimal representations, including edge cases like `INT_MIN`.
* Work with pointers and dynamically allocated memory without leaks.
* Propagate errors through several layers of functions.
* Build and link a static library.

The most challenging part for me was understanding and correctly using `va_list` and the mechanisms surrounding variadic arguments.

---

## AI Usage

I used AI during this project as a tutor. I asked it to follow strict rules: explain concepts and give only function prototypes and expected behaviour, and review my code by pointing out the line of each error and why it was wrong, without giving me the fix.

With that approach, I used AI to:

* Understand `va_list`, variadic arguments and argument promotions.
* Understand the expected behaviour and edge cases of each conversion.
* Review the functions I wrote, locating errors such as lost `write()` return values, memory leaks and out-of-bounds writes, which I then fixed myself.

100% of the code has been written by me, and its ussage has been only didactic.


---

## Resources

### Documentation and references

* `printf(3)` — Linux manual page.
* `stdarg(3)` — documentation for variadic argument handling.
* C standard library documentation.
* 42 ft_printf subject — Version 12.0.
* My previous Libft implementation.

### AI

Used as a tutor and code reviewer during development, as described in [AI Usage](#ai-usage).

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
* [Decisiones técnicas](#decisiones-técnicas)
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

### Funciones internas

| Función              | Archivo             | Función que cumple                                                                              |
| -------------------- | ------------------- | ----------------------------------------------------------------------------------------------- |
| `ft_parse_format`    | `ft_printf.c`       | Recorre la cadena de formato, imprime texto y conversiones y devuelve el total o `-1`.          |
| `ft_format_selector` | `ft_printf.c`       | Recibe el carácter que sigue al `%` y llama al handler correspondiente.                         |
| `ft_handle_p`        | `ft_printf.c`       | Imprime un puntero como `0x` + hexadecimal en minúsculas, o `(nil)` si es `NULL`.               |
| `ft_handle_c`        | `ft_printf_utils.c` | Imprime un carácter (incluido `\0`).                                                            |
| `ft_handle_s`        | `ft_printf_utils.c` | Imprime una cadena, o `(null)` si es `NULL`.                                                    |
| `ft_handle_d`        | `ft_printf_utils.c` | Imprime un entero con signo usando `ft_itoa` de mi Libft. Se usa para `%d` y para `%i`.         |
| `ft_handle_u`        | `ft_printf_utils.c` | Imprime un entero sin signo usando `ft_utoa`.                                                   |
| `ft_handle_x`        | `ft_printf_utils.c` | Imprime un número en hexadecimal; un parámetro elige minúsculas (`%x`) o mayúsculas (`%X`).     |
| `ft_utoa`            | `ft_toas.c`         | Convierte un `unsigned int` en una cadena decimal reservada dinámicamente.                      |
| `ft_xtoa`            | `ft_toas.c`         | Convierte un `unsigned long` en una cadena hexadecimal reservada dinámicamente.                 |
| `ft_hexlen`          | `ft_toas.c`         | Devuelve el número de dígitos hexadecimales de un número.                                       |
| `ft_handle_percent`  | `ft_toas.c`         | Imprime `%`. No consume ningún argumento variádico.                                             |

---

## Algoritmo y estructura de datos

La implementación se basa en un algoritmo de análisis secuencial con un selector de conversiones.

`ft_printf()` solo comprueba que el formato no sea `NULL`, inicia el `va_list`, delega el trabajo en `ft_parse_format()` y cierra el `va_list` con `va_end()`.

`ft_parse_format()` recorre la cadena de formato carácter por carácter. Los caracteres normales se escriben directamente en la salida. Cuando encuentra un `%`, el índice avanza al carácter siguiente, que se interpreta como especificador de conversión y se pasa a `ft_format_selector()`.

El selector solo recibe ese carácter, no la cadena completa. Así, toda la lógica del índice vive dentro de `ft_parse_format()` y solo hay un lugar donde se mueve la posición en la cadena de formato.

El selector llama a un handler por conversión. Todos los handlers siguen el mismo contrato: extraen su argumento con `va_arg()`, escriben la salida con `write()` y devuelven el número de caracteres escritos, o `-1` si algo ha fallado. Como todos devuelven el mismo tipo de valor, `ft_parse_format()` comprueba los errores en un único punto después de cada paso y, si no hay error, suma el resultado al total.

En las conversiones numéricas, el valor se convierte primero en una cadena reservada dinámicamente (`ft_itoa`, `ft_utoa` o `ft_xtoa`) y después se escribe con una sola llamada a `write()`.

El proyecto no necesita una estructura de datos compleja. Los elementos principales son la cadena de formato, el objeto `va_list` que da acceso a los argumentos variables y las cadenas temporales que reservan las conversiones numéricas.

Elegí este enfoque porque separa tres responsabilidades: recorrer la cadena de formato, elegir la conversión y producir la salida de cada una. Cada parte puede entenderse, probarse y modificarse por separado.

---

## Decisiones técnicas

* **Los handlers reciben `va_list *` en lugar de `va_list`.** Si se pasa un `va_list` por valor a una función que usa `va_arg()` sobre él, el estándar de C deja la copia del llamador en un estado indeterminado. En Linux x86_64 funciona por casualidad, porque `va_list` es un array, pero no es portable. Pasar un puntero garantiza que todos los handlers avanzan el mismo `va_list`, el de `ft_printf()`.
* **Propagación de errores.** Se comprueba cada llamada a `write()` y a funciones que reservan memoria. Un fallo devuelve `-1`, que se propaga a través de `ft_parse_format()` hasta `ft_printf()`, que devuelve `-1` como hace el `printf()` original. `va_end()` se llama en un único sitio, en `ft_printf()`, tanto en el camino de éxito como en el de error. Un retorno de `0` no es un error (por ejemplo, `%s` con una cadena vacía).
* **Formato `NULL`.** El estándar de C no permite un formato `NULL`, pero glibc no crashea: `printf(NULL)` no imprime nada y devuelve `-1`. `ft_printf()` hace lo mismo, comprobando el formato antes de llamar a `va_start()`, así que en ese camino no hace falta `va_end()`.
* **`%c` lee un `int`.** Un `char` pasado a una función variádica se promociona a `int`, así que el handler pide un `int` a `va_arg()` y lo convierte a `unsigned char`.
* **`%d` y `%i` comparten handler**, porque en `printf()` producen la misma salida.
* **`ft_xtoa` usa `unsigned long`.** La misma conversión la comparten `%x`, `%X` y `%p`. `%x` recibe un `unsigned int` de 32 bits, pero un puntero en Linux x86_64 ocupa 64 bits, así que la conversión usa el tipo más ancho para no truncar las direcciones.
* **Valores `NULL`**: siguen a glibc. `%s` imprime `(null)` y `%p` imprime `(nil)`.
* **Especificadores desconocidos y `%` al final.** El estándar de C los define como comportamiento indefinido. Mi selector devuelve `0` para cualquier carácter desconocido: no imprime nada y no lo trata como error. Cuando el `%` es el último carácter, el índice se detiene sobre el `\0` final en lugar de saltárselo, así que el bucle nunca lee fuera de la cadena.

---

## Estructura del proyecto

```text
ft_printf/
├── Makefile
├── README.md
├── ft_printf.h
├── ft_printf.c
├── ft_printf_utils.c
├── ft_toas.c
└── libft/
    ├── Makefile
    ├── libft.h
    └── ft_*.c
```

| Archivo             | Contenido                                                            |
| ------------------- | -------------------------------------------------------------------- |
| `ft_printf.c`       | `ft_printf`, `ft_parse_format`, `ft_format_selector` y `ft_handle_p`. |
| `ft_printf_utils.c` | Handlers de `%c`, `%s`, `%d`/`%i`, `%u` y `%x`/`%X`.                 |
| `ft_toas.c`         | Conversiones de número a cadena, `ft_hexlen` y el handler de `%%`.   |
| `ft_printf.h`       | Prototipos e includes.                                               |

El proyecto utiliza mi Libft, que está incluida en el directorio `libft/` con sus archivos fuente y su propio Makefile. El Makefile principal compila primero Libft y después añade los objetos de `ft_printf` a la misma librería.

---

## Instrucciones

### Compilación

Para compilar el proyecto:

```bash
make
```

Esto compila primero `libft/libft.a` y después genera:

```text
libftprintf.a
```

en la raíz del proyecto.

El Makefile utiliza:

```text
cc -Wall -Wextra -Werror
```

La librería estática se genera con `ar`, tal y como requiere el subject. Ejecutar `make` por segunda vez no recompila ni reenlaza nada.

### Reglas del Makefile

| Regla    | Descripción                                                       |
| -------- | ----------------------------------------------------------------- |
| `all`    | Compila Libft y `libftprintf.a`.                                  |
| `clean`  | Elimina los archivos objeto (incluidos los de Libft).             |
| `fclean` | Elimina los archivos objeto y las librerías compiladas.           |
| `re`     | Limpia el proyecto y vuelve a compilarlo.                         |

---

## Uso de ft_printf

Una vez compilado el proyecto, `libftprintf.a` puede enlazarse con otro proyecto de C:

```bash
cc main.c -L. -lftprintf
```

El header debe incluirse en el código:

```c
#include "ft_printf.h"
```

---

## Calidad y validación

* Norminette sobre todos los archivos fuente y headers.
* Compilación con `-Wall -Wextra -Werror`.
* Tester de Tripouille.
* Tester de sfabi28 (las 161 pruebas obligatorias, incluida `ft_printf(NULL)`).
* Salida y valor de retorno comparados con el `printf()` original en casos límite: `%c` con `0`, `%s` con `NULL` y `""`, `INT_MIN`/`INT_MAX`, `%u` y `%x` con `-1`, `%p` con `NULL` y con la dirección máxima, y un formato `NULL`.
* Fallo de `write()`: con la salida estándar cerrada (`./a.out >&-`), `ft_printf()` devuelve `-1`.
* Memory leaks comprobados con `valgrind --leak-check=full`.

---

## Gestión de memoria

`%d`, `%u`, `%x`, `%X` y `%p` utilizan cadenas reservadas dinámicamente. Cada handler las libera en todos los caminos de salida, tanto si `write()` funciona como si falla. Si una reserva falla, el handler libera lo que ya hubiera reservado y devuelve `-1`.

---

## Qué he aprendido

El principal concepto que he aprendido con este proyecto ha sido el funcionamiento de las funciones variádicas en C: cómo usar `va_list`, `va_start`, `va_arg` y `va_end`, cómo afecta la promoción de argumentos al tipo que se pide a `va_arg`, y por qué un `va_list` debe pasarse por puntero cuando varias funciones lo comparten.

Además de las funciones variádicas, el proyecto me ha permitido mejorar en:

* Análisis y procesamiento de cadenas de formato.
* Organización de un proyecto en funciones independientes con un contrato común.
* Gestión de representaciones con signo, sin signo y hexadecimales, incluidos casos límite como `INT_MIN`.
* Uso de punteros y memoria dinámica sin leaks.
* Propagación de errores a través de varias capas de funciones.
* Creación y enlazado de librerías estáticas.

La parte que más me costó fue comprender y utilizar correctamente `va_list` y todo el mecanismo relacionado con los argumentos variádicos.

---

## Uso de IA

He utilizado la IA durante este proyecto como tutor. Le pedí que siguiera reglas estrictas: explicarme conceptos y darme solo prototipos y el comportamiento esperado de cada función, y revisar mi código indicándome la línea de cada error y por qué fallaba, sin darme la solución.

Con ese enfoque, la he usado para:

* Comprender `va_list`, los argumentos variádicos y la promoción de argumentos.
* Entender el comportamiento esperado y los casos límite de cada conversión.
* Revisar las funciones que yo escribía, localizando errores como retornos de `write()` perdidos, memory leaks o escrituras fuera de memoria, que después corregía yo.

El 100% del código ha sido escrito por mí, y el uso de esta ha sido completamente didáctico.

---

## Recursos

### Documentación y referencias

* `printf(3)` — documentación del manual de Linux.
* `stdarg(3)` — documentación sobre los argumentos variádicos.
* Documentación de la biblioteca estándar de C.
* Subject de `ft_printf` de 42 — versión 12.0.
* Mi implementación anterior de Libft.

### IA

Utilizada como tutor y revisor de código durante el desarrollo, tal y como se describe en [Uso de IA](#uso-de-ia).

---

## Autor

**gecasas**

Estudiante de 42 Málaga.

GitHub: https://github.com/gecasas

</details>