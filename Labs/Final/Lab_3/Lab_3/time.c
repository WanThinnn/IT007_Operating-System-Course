#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Sử dụng: %s <lệnh_shell>\n", argv[0]);
        return 1;
    }

    struct timeval start_time, end_time;
    pid_t pid;
    int status;

    // Lấy thời gian bắt đầu
    gettimeofday(&start_time, NULL);

    // Tạo tiến trình con
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Tiến trình con: thực thi lệnh shell
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        perror("execl");
        return 1;
    } else {
        // Tiến trình cha: đợi tiến trình con kết thúc
        waitpid(pid, &status, 0);

        // Lấy thời gian kết thúc
        gettimeofday(&end_time, NULL);

        // Tính thời gian thực thi
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                               (end_time.tv_usec - start_time.tv_usec) / 1e6;

        printf("Thời gian thực thi: %.5f giây\n", elapsed_time);
    }

    return 0;
}
