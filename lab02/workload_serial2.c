#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 200000000
#define NUM_PROCESSES 4

// Funcao que simula uma carga de trabalho pesada
void heavy_work(double* vector, int start, int end) {
    for (int i = start; i < end; ++i) {
        vector[i] = sin(vector[i]) * cos(vector[i]) + sqrt(vector[i]);
    }
}

int main() {
    double* vector = (double*)malloc(VECTOR_SIZE * sizeof(double));
    if (vector == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria!\n");
        return 1;
    }

    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = (double)i;
    }

    // Inicio da medicao de tempo da execucao paralela
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Calcula o tamanho da fatia para cada processo
    int chunk_size = VECTOR_SIZE / NUM_PROCESSES;

    // Loop para criar os processos filhos
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork falhou!\n");
            return 1;
        } else if (pid == 0) { // Codigo do processo filho
            int start_index = i * chunk_size;
            int end_index = (i == NUM_PROCESSES - 1) ? VECTOR_SIZE : (i + 1) * chunk_size;
            
            // O filho executa sua parte da carga de trabalho
            heavy_work(vector, start_index, end_index);
            
            // O filho termina sua execucao
            exit(0);
        }
    }

    // O pai espera que todos os filhos terminem
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    // Fim da medicao de tempo da execucao paralela
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Versao paralela executou em %f segundos com %d processos.\n", time_spent, NUM_PROCESSES);

    // Para verificar se o calculo foi feito, descomente a linha abaixo:
    printf("Resultado de verificacao: vector[10] = %f\n", vector[10]);

    free(vector);
    return 0;
}
//Versao sequencial executou em 6.863634 segundos
//Versao paralela executou em 6.358933 segundos com 4 processos.

//Speedup = T(sequencial) / T(paralelo)
//Speedup = 6.863634 / 6.358933
//Speedup ≈ 1.08