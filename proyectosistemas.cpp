#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 4096

void statsss(pid_t pid) {
struct stat stat_info;
    char proc_path[MAX_PATH];
    sprintf(proc_path, "/proc/%d/status", pid);

    FILE *fp = fopen(proc_path, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo de estado del proceso");
        return;
    }

    char line[256];
    char name[256];
    int process_id = -1;

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Buscar la l�nea que comienza con "Name:"
        if (sscanf(line, "Name:\t%s", name) == 1) {
            printf("Nombre del proceso: %s\n", name);
        }

        // Buscar la l�nea que comienza con "Pid:"
        if (sscanf(line, "Pid:\t%d", &process_id) == 1) {
            printf("ID del proceso: %d\n", process_id);
        }
    }
    
    // Obtener la informaci�n del proceso
    if (stat(proc_path, &stat_info) == 0) {
        // Verificar si el proceso est� en modo kernel o usuario
        if (stat_info.st_uid == 0) {
            printf("El proceso est� en modo kernel.\n");
        } else {
            printf("El proceso est� en modo usuario.\n");
        }
    }
    fclose(fp);
}

int main() {
    DIR *dir;
    struct dirent *entry;

    // Abrir el directorio /proc
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error al abrir el directorio /proc");
        return 1;
    }

    // Leer los directorios en /proc
    while ((entry = readdir(dir)) != NULL) {
        // Verificar si el nombre del directorio es un n�mero (PID)
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            pid_t pid = atoi(entry->d_name);
            statsss(pid);
           
          
        }
    }
    
    closedir(dir);

    return 0;
}

