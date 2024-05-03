#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

// Struct để lưu trữ dữ liệu trong buffer
typedef struct {
    int sequence[BUFFER_SIZE];
    int length;
} CollatzData;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Sử dụng: ./collatz <số nguyên dương>\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Vui lòng nhập số nguyên dương\n");
        return 1;
    }

    int shmid;
    key_t key = ftok("collatz", 65); // Tạo key
    CollatzData *collatzData;

    // Tạo shared memory segment
    if ((shmid = shmget(key, sizeof(CollatzData), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Gắn kết vùng nhớ shared với vùng nhớ của tiến trình
    if ((collatzData = (CollatzData *)shmat(shmid, NULL, 0)) == (CollatzData *) -1) {
        perror("shmat");
        exit(1);
    }

    // Tiến trình con (Producer) tính toán chuỗi Collatz
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        int length = 0;
        while (n != 1 && length < BUFFER_SIZE) {
            collatzData->sequence[length++] = n;
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = 3 * n + 1;
            }
        }
        collatzData->sequence[length++] = 1;
        collatzData->length = length;
        exit(0);
    } else {
        // Tiến trình cha (Consumer) in chuỗi kết quả
        wait(NULL);
        printf("Chuỗi Collatz: ");
        for (int i = 0; i < collatzData->length; i++) {
            printf("%d ", collatzData->sequence[i]);
        }
        printf("\n");

        // Xóa shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}


/*
Trong chương trình này:

    Chúng ta đảm bảo rằng chương trình được gọi với một đối số là số nguyên dương. Nếu không, chúng ta sẽ hiển thị một thông báo lỗi và thoát.
    Tiến trình cha tạo một shared memory segment và gắn kết nó vào vùng nhớ của tiến trình. Sau đó, nó gọi fork() để tạo một tiến trình con.
    Tiến trình con tính toán chuỗi Collatz cho số nguyên được cung cấp và lưu trữ nó trong shared memory segment.
    Tiến trình cha đợi cho tiến trình con kết thúc, sau đó in ra chuỗi kết quả từ shared memory segment.
    Cuối cùng, chúng ta xóa shared memory segment.
*/