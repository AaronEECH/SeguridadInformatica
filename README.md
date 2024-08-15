# Keylogger en C

Este programa es un keylogger simple en C que captura eventos del teclado y registra la duración de cada tecla presionada en un archivo de log. El archivo de log se guarda en una ruta específica con un nombre basado en la fecha y hora actuales.

## Descripción del Código

El programa se encarga de realizar las siguientes tareas:

1. **Obtener el nombre del archivo de log**:
   - La función `get_log_filename` genera un nombre de archivo basado en la fecha y hora actuales para guardar los registros.

2. **Convertir códigos de tecla a nombres**:
   - La función `keycode_to_string` convierte los códigos de tecla en sus nombres correspondientes para facilitar la lectura del archivo de log.

3. **Leer eventos del teclado**:
   - El programa abre el dispositivo de entrada del teclado (por defecto `/dev/input/event10`) y escucha los eventos de teclado.

4. **Registrar eventos de teclado**:
   - Cuando una tecla es presionada, se registra el código de la tecla y el tiempo de la presión.
   - Cuando la tecla es soltada, se calcula la duración de la presión y se escribe en el archivo de log.

## Instrucciones de Uso

1. **Compilar el Programa**:
   - Navega al directorio del código fuente y compila el programa con `gcc`:
     ```bash
     gcc -o keylogger keylogger.c
     ```

2. **Ejecutar el Keylogger**:
   - Ejecuta el programa con permisos de superusuario para acceder al dispositivo de entrada del teclado:
     ```bash
     sudo ./keylogger
     ```

3. **Revisar el Archivo de Log**:
   - El archivo de log se guardará en la ruta `/home/surfacego2/Documentos/keylogger` con un nombre basado en la fecha y hora actuales. Revisa este archivo para ver los registros de teclas.

## Comentarios del Código

- **Función `get_log_filename`**:
  ```c
  char* get_log_filename() {
      time_t t = time(NULL);
      struct tm *tm_info = localtime(&t);
      static char filename[128];
      strftime(filename, 128, "/home/surfacego2/Documentos/keylogger/%d-%m-%Y_%H-%M.txt", tm_info);
      return filename;
  }
