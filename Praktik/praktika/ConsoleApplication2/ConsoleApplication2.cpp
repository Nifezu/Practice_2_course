#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shakerSort(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    int swapped = 1;

    while (left < right&& swapped) {
        swapped = 0;

        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                swapped = 1;
            }
        }
        right--;

        for (int i = right; i > left; i--) {
            if (arr[i] < arr[i - 1]) {
                swap(&arr[i], &arr[i - 1]);
                swapped = 1;
            }
        }
        left++;

        if (!swapped) break;
    }
}

void printCentered(const char* text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    printf("+");
    for (int i = 0; i < width; i++) printf("-");
    printf("+\n");
    printf("|");
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
    for (int i = 0; i < width - len - padding; i++) printf(" ");
    printf("|\n");
    printf("+");
    for (int i = 0; i < width; i++) printf("-");
    printf("+\n");
}

void printArrayFromFile(const char* filename, const char* title, int width) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return;
    }

    printCentered(title, width - 2);

    int num, count = 0;
    while (fscanf(file, "%d", &num) == 1) {
        printf(" %5d", num);
        count++;
        if (count % 15 == 0) {
            printf("\n");
        }
    }

    if (count % 15 != 0) {
        printf("\n");
    }

    fclose(file);
}

void saveToFile(int arr[], int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
        if ((i + 1) % 15 == 0) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

int createArray(int** arr_ptr, int* n_ptr, int* min_val_ptr, int* max_val_ptr, int* generation_type) {
    int choice;
    printf("\n Выберите способ заполнения массива:\n");
    printf(" 1 - Случайная генерация\n");
    printf(" 2 - Ручной ввод\n");
    printf(" Ваш выбор: ");
    scanf("%d", &choice);

    if (choice != 1 && choice != 2) {
        printf("\nОшибка: неверный выбор!\n");
        return 0;
    }

    *generation_type = choice;

    int n;
    printf(" Введите размер массива: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("\nОшибка: размер массива должен быть положительным!\n");
        return 0;
    }

    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("\nОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ!\n");
        return 0;
    }

    if (choice == 1) {
        int min_val, max_val;
        printf(" Введите минимальное значение элемента: ");
        scanf("%d", &min_val);
        printf(" Введите максимальное значение элемента: ");
        scanf("%d", &max_val);

        if (min_val > max_val) {
            printf("\nОшибка: минимальное значение не может быть больше максимального!\n");
            free(arr);
            return 0;
        }

        srand(time(0));
        int range = max_val - min_val + 1;
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % range + min_val;
        }

        *min_val_ptr = min_val;
        *max_val_ptr = max_val;
    }
    else if (choice == 2) {
        printf("\n Введите элементы массива:\n");
        for (int i = 0; i < n; i++) {
            printf(" Элемент [%d]: ", i + 1);
            scanf("%d", &arr[i]);
        }

        int min_val = arr[0];
        int max_val = arr[0];
        for (int i = 1; i < n; i++) {
            if (arr[i] < min_val) min_val = arr[i];
            if (arr[i] > max_val) max_val = arr[i];
        }

        *min_val_ptr = min_val;
        *max_val_ptr = max_val;
    }


    *arr_ptr = arr;
    *n_ptr = n;
    saveToFile(arr, n, "input.txt");
    return 1;
}

int resizeArray(int** arr_ptr, int* n_ptr, int generation_type, int min_val, int max_val) {
    int new_n;
    printf(" Введите новый размер массива: ");
    scanf("%d", &new_n);

    if (new_n <= 0) {
        printf("\nОшибка: размер массива должен быть положительным!\n");
        return 0;
    }

    int* new_arr = (int*)malloc(new_n * sizeof(int));
    if (new_arr == NULL) {
        printf("\nОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ!\n");
        return 0;
    }

    if (generation_type == 1) {
        srand(time(0));
        int range = max_val - min_val + 1;
        for (int i = 0; i < new_n; i++) {
            new_arr[i] = rand() % range + min_val;
        }
    }
    else if (generation_type == 2) {
        printf("\n Введите новые элементы массива:\n");
        for (int i = 0; i < new_n; i++) {
            printf(" Элемент [%d]: ", i + 1);
            scanf("%d", &new_arr[i]);
        }
    }

    if (*arr_ptr) {
        free(*arr_ptr);
    }

    *arr_ptr = new_arr;
    *n_ptr = new_n;
    saveToFile(new_arr, new_n, "input.txt");
    return 1;
}

void changeRange(int* arr, int n, int* min_val_ptr, int* max_val_ptr) {
    int min_val, max_val;
    printf(" Введите новый минимальный элемент: ");
    scanf("%d", &min_val);
    printf(" Введите новый максимальный элемент: ");
    scanf("%d", &max_val);

    if (min_val > max_val) {
        printf("\nОшибка: минимальное значение не может быть больше максимального!\n");
        return;
    }

    srand(time(0));
    int range = max_val - min_val + 1;
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % range + min_val;
    }

    *min_val_ptr = min_val;
    *max_val_ptr = max_val;
    saveToFile(arr, n, "input.txt");
    printf(" Диапазон массива изменен\n");
}

void calculateMinMax(int arr[], int n, int* min_val, int* max_val) {
    if (n <= 0) return;

    *min_val = arr[0];
    *max_val = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < *min_val) *min_val = arr[i];
        if (arr[i] > *max_val) *max_val = arr[i];
    }
}

int main() {
    setlocale(LC_ALL, "");
    int n = 0;
    int* arr = NULL;
    int min_val = 0, max_val = 0;
    int generation_type = 0;
    const int consoleWidth = 90;
    clock_t start, end;
    double time_used = 0;

    printCentered("ШЕЙКЕРНАЯ СОРТИРОВКА", consoleWidth - 2);

    if (!createArray(&arr, &n, &min_val, &max_val, &generation_type)) {
        return 1;
    }

    start = clock();
    shakerSort(arr, n);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    saveToFile(arr, n, "sorted.txt");
    printf(" Сортировка выполнена за %.6f секунд\n", time_used);
    printf(" Диапазон значений: [%d, %d]\n", min_val, max_val);

    int menu_choice;
    while (1) {
        printf("\n");
        printCentered("ГЛАВНОЕ МЕНЮ", consoleWidth - 2);
        printf(" Текущий размер массива: %d\n", n);
        printf(" Текущий способ заполнения: %s\n", generation_type == 1 ? "Случайный" : "Ручной");
        if (generation_type == 1) {
            printf(" Текущий диапазон значений: [%d, %d]\n", min_val, max_val);
        }
        printf(" 1 - Изменить размер массива\n");
        printf(" 2 - Просмотреть исходный массив\n");
        printf(" 3 - Просмотреть отсортированный массив\n");
        printf(" 4 - Очистить файлы\n");
        printf(" 5 - Изменить диапазон массива\n");
        printf(" 6 - Показать время последней сортировки\n");
        printf(" 7 - Выйти из программы\n");
        printf(" Ваш выбор: ");
        scanf("%d", &menu_choice);


        switch (menu_choice) {
        case 1:
            if (!resizeArray(&arr, &n, generation_type, min_val, max_val)) {
                break;
            }
            calculateMinMax(arr, n, &min_val, &max_val);
            start = clock();
            shakerSort(arr, n);
            end = clock();
            time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            saveToFile(arr, n, "sorted.txt");
            printf(" Сортировка выполнена за %.6f секунд\n", time_used);
            printf(" Новый размер массива: %d\n", n);
            if (generation_type == 1) {
                printf(" Диапазон значений: [%d, %d]\n", min_val, max_val);
            }
            break;

        case 2:
            printArrayFromFile("input.txt", "ИСХОДНЫЙ МАССИВ", consoleWidth);
            break;

        case 3:
            printArrayFromFile("sorted.txt", "ОТСОРТИРОВАННЫЙ МАССИВ", consoleWidth);
            break;

        case 4:
            remove("input.txt");
            remove("sorted.txt");
            printf(" Файлы input.txt и sorted.txt успешно удалены\n");
            break;

        case 5:
            if (generation_type != 1) {
                printf(" Изменение диапазона доступно только для случайно сгенерированных массивов!\n");
                break;
            }
            changeRange(arr, n, &min_val, &max_val);
            start = clock();
            shakerSort(arr, n);
            end = clock();
            time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            saveToFile(arr, n, "sorted.txt");
            printf(" Сортировка выполнена за %.6f секунд\n", time_used);
            printf(" Новый диапазон: [%d, %d]\n", min_val, max_val);
            break;

        case 6:
            printf(" Время последней сортировки: %.6f секунд\n", time_used);
            break;

        case 7:
            if (arr) free(arr);
            return 0;

        default:
            printf(" Неверный выбор! Попробуйте снова.\n");
        }
    }
}
