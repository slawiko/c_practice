#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int n = 100000;
    pid_t pid;
    int status;

    for (int i = 0; i < n; ++i) {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Unable to fork\n");
            exit(1);
        } else if (pid > 0) {
            waitpid(pid, &status, 0);
        } else {
            if (execlp("/bin/true", NULL) == -1) {
                fprintf(stderr, "Unable to exec\n");
                exit(1);
            }
        }
    }

    exit(0);
}