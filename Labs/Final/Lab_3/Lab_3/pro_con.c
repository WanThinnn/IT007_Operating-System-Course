#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define BUFFER_SIZE 10

// Struct để lưu trữ dữ liệu trong buffer
typedef struct {
    int data;
    int count;
} Item;

int main() {
    int shmid;
    key_t key = ftok("shared_memory", 65); // Tạo key
    Item *buffer;

    // Tạo shared memory segment
    if ((shmid = shmget(key, BUFFER_SIZE * sizeof(Item), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Gắn kết vùng nhớ shared với vùng nhớ của tiến trình
    if ((buffer = (Item *)shmat(shmid, NULL, 0)) == (Item *) -1) {
        perror("shmat");
        exit(1);
    }

    // Khởi tạo count trong buffer
    buffer->count = 0;

    // Fork để tạo tiến trình con
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con (Consumer)
        int sum = 0;
        while (1) {
            // Đợi buffer có dữ liệu
            while (buffer->count <= 0);

            // Đọc dữ liệu từ buffer
            int data = buffer->data;
            printf("Consumer: %d\n", data);
            sum += data;

            // Tính tổng và kiểm tra điều kiện kết thúc
            if (sum > 100) {
                printf("Tổng vượt quá 10000, kết thúc tiến trình con\n");
                break;
            }

            buffer->count--;
        }
    } else {
        // Tiến trình cha (Producer)
        srand(time(NULL)); // Khởi tạo seed cho hàm rand()
        while (1) {
            // Đợi buffer trống
            while (buffer->count >= BUFFER_SIZE);

            // Tạo số ngẫu nhiên trong khoảng [10, 20]
            int data = rand() % 11 + 10;
            printf("Producer: %d\n", data);

            // Ghi dữ liệu vào buffer
            buffer->data = data;
            buffer->count++;

            // Kiểm tra điều kiện kết thúc
            if (data > 100) {
                printf("Tổng vượt quá 100, kết thúc tiến trình cha\n");
                break;
            }
        }

        // Đợi tiến trình con kết thúc
        wait(NULL);

        // Xóa shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
