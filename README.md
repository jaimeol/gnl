*Este proyecto ha sido creado como parte del currículo de 42 por jolivare.*

# 📜 get_next_line

## 📝 Descripción
El objetivo del proyecto **get_next_line** es escribir una función en C que devuelva una línea leída desde un descriptor de archivo (file descriptor), una llamada a la vez.

Este proyecto introduce el concepto de **variables estáticas** en C y profundiza en la gestión de memoria dinámica (`malloc` y `free`), la manipulación de punteros, la lectura con búfer dinámico y la gestión eficiente de flujos de entrada/salida (I/O) a bajo nivel.

### Objetivos principales:
- Comprender el funcionamiento y ciclo de vida de las **variables estáticas**.
- Manipular correctamente los descriptores de archivos con la llamada al sistema `read()`.
- Gestionar buffers de lectura dinámicos mediante un tamaño definido en tiempo de compilación (`BUFFER_SIZE`).
- Garantizar la ausencia de fugas de memoria (memory leaks) en escenarios de lectura parcial, final de archivo (EOF) o errores de lectura.

---

## 🛠️ Instrucciones

### Requisitos previos
- Compilador de C (como `gcc` o `clang`).
- Herramientas estándar de desarrollo de POSIX (para la cabecera `unistd.h`).

### Compilación
Dado que `get_next_line` no se compila como un binario independiente sino como parte de otro programa, debes compilar tus archivos de código junto con el archivo que contenga tu función `main`.

Es obligatorio definir el tamaño del búfer de lectura en tiempo de compilación mediante el flag `-D BUFFER_SIZE=N`.

#### Compilación básica:
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=32 src/get_next_line.c src/get_next_line_utils.c main.c -o gnl
```

#### Compilación de la versión Bonus (soporte para múltiples file descriptors):
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=32 bonus/get_next_line_bonus.c bonus/get_next_line_utils_bonus.c main.c -o gnl_bonus
```

### Ejemplo de uso
Para probar la función, crea un archivo `main.c` con el siguiente contenido:

```c
#include "src/get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Error al abrir el archivo");
        return (1);
    }
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line); // Es fundamental liberar la línea devuelta para evitar leaks
    }
    close(fd);
    return (0);
}
```

Crea un archivo de texto de prueba `test.txt` y ejecuta el binario compilado:
```bash
echo -e "Línea 1\nLínea 2\nLínea 3 sin salto final" > test.txt
./gnl
```

---

## 🧮 Explicación y Justificación del Algoritmo

### El Algoritmo
El algoritmo implementado se divide en 4 fases lógicas:

1. **Lectura iterativa (`ft_read_file`)**:
   - Se reserva memoria para un buffer temporal de tamaño `BUFFER_SIZE + 1`.
   - Se realiza un bucle en el que se llama a `read(fd, buffer, BUFFER_SIZE)`.
   - Tras cada lectura con éxito, el contenido leído se añade (utilizando `ft_strjoin`) a nuestro acumulador estático (`mem`).
   - El bucle termina cuando `read()` devuelve `0` (EOF), `-1` (error), o cuando se detecta un carácter de salto de línea (`\n`) en el búfer temporal.
   
2. **Extracción de la línea actual (`ft_line`)**:
   - Se analiza el acumulador estático buscando la posición del primer salto de línea `\n`.
   - Se reserva el tamaño de memoria exacto necesario para almacenar la subcadena que va desde el índice `0` hasta el carácter `\n` (inclusive) o final de cadena, y se copia en un nuevo puntero `line`.

3. **Actualización de los datos restantes (`ft_next`)**:
   - El acumulador estático debe conservar los bytes que se leyeron pero que están después del salto de línea `\n` procesado.
   - Se reserva memoria para almacenar esta porción sobrante (el remanente).
   - Se copia el remanente en una nueva dirección de memoria, se libera el antiguo acumulador estático y se le asigna esta nueva dirección. Si no quedan datos sobrantes, se libera el puntero estático y se establece a `NULL`.

4. **Retorno**:
   - Se devuelve la línea extraída.

### Justificación de las Decisiones de Diseño

- **Uso de Variables Estáticas**:
  Dado que `read()` lee bytes en bloques arbitrarios de tamaño `BUFFER_SIZE`, es muy probable que una lectura traiga datos pertenecientes a la siguiente línea o a múltiples líneas posteriores. Como la función debe devolver únicamente *una línea por llamada*, necesitamos persistir ese exceso de datos leídos. Las variables estáticas se almacenan en el segmento de datos del programa, lo que les permite conservar su valor e integridad entre llamadas sucesivas a la función `get_next_line()`.

- **Justificación del Bonus (Múltiples File Descriptors)**:
  Para permitir lecturas simultáneas de distintos archivos (por ejemplo, leer una línea del `fd 3`, luego una del `fd 4`, y luego volver al `fd 3` sin perder el hilo de lectura), se reemplazó la variable estática simple por un array de punteros estáticos: `static char *buffer[1024]`.
  - **Eficiencia O(1)**: Usar el valor numérico del `fd` como índice en el array permite acceder de inmediato a la memoria persistente de ese descriptor de archivo específico.
  - **Simplicidad**: Evita la sobrecarga de implementar una lista enlazada (que requeriría buscar nodos secuencialmente, liberarlos y crearlos dinámicamente) manteniendo la legibilidad y rendimiento del código bajo los límites normales de archivos abiertos en procesos Unix (generalmente limitados a 1024 descriptores).

- **Gestión de Memoria y Robustez**:
  Cada función auxiliar (`ft_free`, `ft_next`, `ft_line`) está diseñada para liberar sistemáticamente la memoria obsoleta inmediatamente después de reasignar los punteros. En caso de error de lectura (`read` retornando `-1`), el acumulador estático y el buffer temporal se liberan de inmediato para garantizar que no haya fugas de memoria en escenarios fallidos.

---

## 📚 Recursos

### Referencias y Documentación
- **Manuales de Unix**:
  - `man 2 read` - Documentación oficial de la llamada al sistema de lectura de archivos.
  - `man 3 malloc` y `man 3 free` - Documentación sobre gestión de memoria en C.
- **Conceptos de Programación en C**:
  - [Variables estáticas en C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/) - Explicación detallada del ciclo de vida y alcance de las variables estáticas.
  - [File Descriptors (Descriptores de Archivo)](https://en.wikipedia.org/wiki/File_descriptor) - Concepto de entrada/salida en sistemas operativos POSIX.

### Uso de Inteligencia Artificial (IA)
En cumplimiento con las directrices de transparencia en el uso de herramientas de asistencia:
- **Tareas realizadas por la IA**:
  - Redacción, maquetación y traducción a formato Markdown de este archivo `README.md`.
  - Estructuración de las explicaciones conceptuales y redacción técnica del algoritmo.
  - Diseño y formateo del ejemplo práctico de código de prueba (`main.c`).
- **Partes del proyecto exentas de IA**:
  - Todo el código fuente de la lógica principal y de utilidad en `src/` y `bonus/` ha sido desarrollado íntegramente de forma manual por el autor, aplicando los conocimientos adquiridos en la escuela 42 sobre algoritmos, gestión de memoria libre de fugas y cumplimiento de la Norma (Norminette).
