#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    /*
    printf("printf");
    fwrite("fwrite", 6, 1, stdout);
    fprintf(stdout, "%s", "fprintf");
    write(1, "write", 5);
    sleep(3);
    */
    //close(1);
    int fd = open("./test.txt", O_RDWR);
    if (fd < 0) {
        perror("open error");
        return -1;
    }
    dup2(fd, 1);
    printf("fd:%d\n", fd);
    fflush(stdout);
    close(fd);
    return 0;
}
