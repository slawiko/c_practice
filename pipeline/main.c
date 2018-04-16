#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void free_args(char*** args, int n) {
    for (int i = 0; i < n; ++i) {
        free(args[i]);
    }

    free(args);
}

int main(int argc, char** argv) {
    char*** ARGS = malloc(sizeof(char**) * argc);
    if (ARGS == NULL) {
        fprintf(stderr, "Error during memory allocation");
        return 1;
    }

    ARGS[0] = malloc(sizeof(char*) * argc);
    if (ARGS[0] == NULL) {
        fprintf(stderr, "Error during memory allocation");

        free(ARGS);
        return 1;
    }

    int n = 0, pos = 0;
    bool is_in_command = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            if (!is_in_command || i == argc - 1) {
                fprintf(stderr, "Invalid argument");
                // help

                free_args(ARGS, n);
                return 2;
            }

            ARGS[n][pos] = NULL;
            ARGS[++n] = malloc(sizeof(char*) * argc);
            if (ARGS[n] == NULL) {
                fprintf(stderr, "Error during memory allocation");

                free(ARGS);
                return 1;
            }

            is_in_command = false;
            pos = 0;
        } else {
            is_in_command = true;
            ARGS[n][pos++] = argv[i];
        }
    }
    ARGS[n][pos] = NULL;
    n++;

    pid_t children[n];
    pid_t pid;

    if (n == 1) {
        pid = fork();
        if (pid < 0) {
            perror("Error while creating child process");

            free_args(ARGS, n);
            return 1;
        }

        if (pid == 0) {
            char** args = ARGS[0];
            if (execvp(args[0], args) != 0) {
                perror("Execvp error. Probably one of pipes isn't a valid command");

                free_args(ARGS, n);
                return 1;
            }
        } else {
            children[0] = pid;
        }
    } else {
        int fds_cnt = n - 1;
        int pipefds[fds_cnt][2];

        for (int i = 0; i < n; ++i) {
            if (i < fds_cnt && pipe(pipefds[i]) == -1) {
                perror("Pipe error");
                free_args(ARGS, n);
                return 1;
            }

            pid = fork();

            if (pid < 0) {
                perror("Child process creation failed");
                free_args(ARGS, n);
                return 1;
            }
            if (pid == 0) {
                char** args = ARGS[i];
                if (i == 0) {
                    dup2(pipefds[i][1], 1);

                    close(pipefds[i][0]);
                } else if (i == fds_cnt) {
                    dup2(pipefds[i - 1][0], 0);

                    close(pipefds[i - 1][1]);
                } else {
                    dup2(pipefds[i][1], 1);
                    dup2(pipefds[i - 1][0], 0);

                    close(pipefds[i][0]);
                    close(pipefds[i - 1][1]);
                }
                execvp(args[0], args);
            } else {
                children[i] = pid;
                if (i > 0) {
                    close(pipefds[i - 1][0]);
                    close(pipefds[i - 1][1]);
                }
            }
        }
    }

    int exitCode = 0;
    int status;
    for (int i = 0; i < n; ++i) {
        waitpid(children[i], &status, 0);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                exitCode = 1;
            }
        }
    }

    free_args(ARGS, n);

    return exitCode;
}