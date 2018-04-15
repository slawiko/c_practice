#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

char* ARGS[3][5] = {
        {"ls", "-l", "/etc/", NULL},
        {"grep", "p", NULL},
        {"wc", "-l", NULL}
};

int main() {
    int n = 3;
    int fds_cnt = n - 1;

    int pipefds[fds_cnt][2];

    pid_t children[n];

    for (int i = 0; i < n; ++i) {
        if (i < fds_cnt && pipe(pipefds[i]) == -1) {
            perror("pipe");
            return 1;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Child process creation failed");
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

    return exitCode;
}