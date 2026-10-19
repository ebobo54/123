#include <iostream>
#include <vector>
#include <omp.h>

// 1

// int main() {
//     // Устанавливаем принудительно 4 потока
//     // omp_set_num_threads(4);

//     #pragma omp parallel
//     {
//         int id = omp_get_thread_num();

//         //сообщения от разных потоков 
//         #pragma omp critical
//         {
//             if (id % 2 == 0) {
//                 std::cout << "Even-numbered thread: " << id << std::endl;
//             } else {
//                 std::cout << "Odd-numbered thread: " << id << std::endl;
//             }
//         }
//     }

//     return 0;
// }


// 2

int main() {
    // Количество потоков и размер массива (должен нацело делиться на число потоков)
    const int num_threads = 4;
    const int N = 100; // 100 нацело делится на 4 (по 25 элементов на поток)
    
    omp_set_num_threads(num_threads);

    // 1) Инициализация массива A произвольными числами
    std::vector<int> A(N);
    for (int i = 0; i < N; ++i) {
        A[i] = i + 1; // Заполним числами от 1 до 100
    }

    // Массив для сохранения частичных сумм каждого потока
    std::vector<int> B(num_threads, 0);

    // 2) Создание параллельного региона
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        // Вычисление границ непрерывного блока для текущего потока
        int chunk_size = N / total_threads;
        int start_idx = id * chunk_size;
        int end_idx = start_idx + chunk_size;

        // Вычисление частичной суммы
        int partial_sum = 0;
        for (int i = start_idx; i < end_idx; ++i) {
            partial_sum += A[i];
        }

        // Сохранение результата в массив B
        B[id] = partial_sum;

        // Вывод информации от потока (используем critical для читаемости вывода)
        #pragma omp critical
        {
            std::cout << "Thread " << id << " computed partial sum: " << partial_sum 
                      << " (elements from index " << start_idx << " to " << end_idx - 1 << ")" << std::endl;
        }
    }

    // 3) Суммирование элементов массива B после завершения параллельного региона
    int total_sum = 0;
    for (int i = 0; i < num_threads; ++i) {
        total_sum += B[i];
    }

    std::cout << "\nTotal sum of array A elements: " << total_sum << std::endl;

    return 0;
}