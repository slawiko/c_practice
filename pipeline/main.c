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

void exit_save(int code, char*** args, int n) {
    free_args(args, n);
    exit(code);
}

void close_exit_safe(int* fds, int size, char*** args, int n) {
    int exit_code = 0;
    for (int i = 0; i < size; ++i) {
        if (close(fds[i]) < 0) {
            exit_code = -1;
        }
    }
    if (exit_code < 0) {
        perror("close error\n");

        exit_save(1, args, n);
    }
}

void dup2_safe(int oldfd, int newfd, char*** args, int n) {
    if (dup2(oldfd, newfd) < 0) {
        perror("dup2 error\n");

        int fds[] = { oldfd, newfd };
        close_exit_safe(fds, 2, args, n);
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr, "Invalid argument\n");
        printf("This program works like pipe in bash\n");

        exit(2);    }

    char*** ARGS = malloc(sizeof(char**) * argc);
    if (ARGS == NULL) {
        fprintf(stderr, "Error during memory allocation\n");
        exit(1);
    }

    ARGS[0] = malloc(sizeof(char*) * argc);
    if (ARGS[0] == NULL) {
        fprintf(stderr, "Error during memory allocation\n");

        free(ARGS);
        exit(1);
    }

    int n = 0, pos = 0;
    bool is_in_command = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            if (!is_in_command || i == argc - 1) {
                fprintf(stderr, "Invalid argument\n");
                printf("This program works like pipe in bash\n");

                exit_save(2, ARGS, n);
            }

            ARGS[n][pos] = NULL;
            ARGS[++n] = malloc(sizeof(char*) * argc);
            if (ARGS[n] == NULL) {
                fprintf(stderr, "Error during memory allocation\n");

                free(ARGS);
                exit(1);
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
            perror("Error while creating child process\n");

            exit_save(1, ARGS, n);
        }

        if (pid == 0) {
            char** args = ARGS[0];
            if (execvp(args[0], args) != 0) {
                perror("execvp error. Probably one of pipes isn't a valid command\n");

                exit_save(1, ARGS, n);
            }
        } else {
            children[0] = pid;
        }
    } else {
        int fds_cnt = n - 1;
        int pipefds[fds_cnt][2];

        for (int i = 0; i < n; ++i) {
            if (i < fds_cnt && pipe(pipefds[i]) == -1) {
                perror("Pipe error\n");


                exit_save(1, ARGS, n);
            }

            pid = fork();
            if (pid < 0) {
                perror("Child process creation failed\n");

                close_exit_safe(pipefds[i], 2, ARGS, n);
            }

            if (pid == 0) {
                char** args = ARGS[i];
                if (i == 0) {
                    dup2_safe(pipefds[i][1], 1, ARGS, n);

                    close_exit_safe(pipefds[i], 2, ARGS, n);
                } else if (i == fds_cnt) {
                    dup2_safe(pipefds[i - 1][0], 0, ARGS, n);

                    close_exit_safe(pipefds[i - 1], 2, ARGS, n);
                } else {
                    dup2_safe(pipefds[i][1], 1, ARGS, n);
                    dup2_safe(pipefds[i - 1][0], 0, ARGS, n);

                    int fds[] = { pipefds[i - 1][0], pipefds[i - 1][1], pipefds[i][0], pipefds[i][1] };
                    close_exit_safe(fds, 4, ARGS, n);
                }

                if (execvp(args[0], args) != 0) {
                    perror("Execvp error. Probably one of pipes isn't a valid command\n");

                    exit_save(1, ARGS, n);
                }
            } else {
                children[i] = pid;
                if (i > 0) {
                    close_exit_safe(pipefds[i - 1], 2, ARGS, n);
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

    exit_save(exitCode, ARGS, n);
}