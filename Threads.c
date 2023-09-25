#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3

// Определение размера матриц
#define ROWS_A MATRIX_SIZE
#define COLS_A MATRIX_SIZE
#define COLS_B MATRIX_SIZE

// Матрицы для перемножения и результат
int A[ROWS_A][COLS_A];
int B[COLS_A][COLS_B];
int C[ROWS_A][COLS_B];

// Структура для передачи данных потоку
typedef struct {
    int startRow;
    int endRow;
} ThreadData;

// Функция для перемножения матрицы на определенном диапазоне строк
void* multiplyMatrix(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int i = data->startRow; i < data->endRow; ++i) {
        for (int j = 0; j < COLS_B; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < COLS_A; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MATRIX_SIZE];
    ThreadData threadData[MATRIX_SIZE];
    
    // Заполнение матриц A и B (пример)
    for (int i = 0; i < ROWS_A; ++i) {
        for (int j = 0; j < COLS_A; ++j) {
            A[i][j] = i * COLS_A + j + 1;
            B[i][j] = j * COLS_B + i + 1;
        }
    }
    
    // Создание потоков для перемножения матриц
    int numThreads = MATRIX_SIZE;
    for (int i = 0; i < numThreads; ++i) {
        threadData[i].startRow = i;
        threadData[i].endRow = i + 1;
        pthread_create(&threads[i], NULL, multiplyMatrix, &threadData[i]);
    }
    
    // Ожидание завершения всех потоков
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Вывод результата
    printf("Результат перемножения матриц:\n");
    for (int i = 0; i < ROWS_A; ++i) {
        for (int j = 0; j < COLS_B; ++j) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
