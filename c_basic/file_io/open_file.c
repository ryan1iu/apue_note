#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd;

    fd = open("/home/ryan/LearnSubject/linux_system_programing/file_io/open_file.c", O_RDONLY);
    if (fd == -1) {
        printf("error");
    }
}
