#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret < 0) {
        perror("pipe error");
        return -1;
    }
    int pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], 1);
        execl("/usr/bin/ps", "ps", "-ef", NULL);
        exit(0);
    }
    int pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        execl("/usr/bin/grep", "grep", "sshd", NULL);
        exit(0);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
