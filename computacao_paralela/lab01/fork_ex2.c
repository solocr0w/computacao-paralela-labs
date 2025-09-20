#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int filhos = 4;

    for(int i = 0; i < filhos; i++){
         pid_t pid = fork();

        if (pid < 0) { // Erro
            fprintf(stderr, "Fork falhou!\n");
            return 1;
        } else if (pid == 0) { // Processo Filho
            printf("Eu sou o filho! Meu PID e %d, meu pai e %d.\n", getpid(), getppid());
            return 0;
        }
    }

    printf("Pai: Terminei de criar os %d filhos. Meu PID e %d.\n", filhos, getpid());

    for (int i = 0; i < filhos; i++) {
        wait(NULL);
    }

    printf("Pai: Todos os filhos terminaram, agora eu posso terminar.\n");

}