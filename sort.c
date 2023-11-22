#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int leftArr[n1], rightArr[n2];
    int i, j, k;
    int inversions = 0; // 记录逆序对数量

    for (i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
            inversions += (n1 - i); // 更新逆序对数量
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    return inversions;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// 快速排序实现
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 选择排序实现
void selectionSort(int arr[], int n) {
    int i, j, minIndex;

    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        int temp = arr[minIndex];
        arr[minIndex] = arr[i];
        arr[i] = temp;
    }
}

// 插入排序实现
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


// 堆排序实现
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

// 基数排序实现
void countingSort(int arr[], int n, int exp) {
    int output[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixSort(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
    }
}

// 归并排序中计算逆序对数量的实现
int mergeAndCount(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        int inversionsLeft = mergeAndCount(arr, left, mid);
        int inversionsRight = mergeAndCount(arr, mid + 1, right);
        int inversionsMerge = merge(arr, left, mid, right);

        return inversionsLeft + inversionsRight + inversionsMerge;
    }
    return 0;
}
int main() {
    // 读取数据
    FILE *file = fopen("C:\\Users\\Fanful\\DSlab4Rank\\input.txt", "r");
    int n;
    fscanf(file, "%d", &n);
    int arr[n];
    for (int i = 0; i < n; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    clock_t start, end;

    // 归并排序
    int mergeSortArr[n];
    for (int i = 0; i < n; i++) {
        mergeSortArr[i] = arr[i];
    }
    start = clock();
    int inversions = mergeAndCount(mergeSortArr, 0, n - 1);
    end = clock();
    double mergeSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Merge Sort: %.6f seconds\n", mergeSortTime);
    printf("Number of Inversions: %d\n", inversions);

    // 快速排序
    int quickSortArr[n];
    for (int i = 0; i < n; i++) {
        quickSortArr[i] = arr[i];
    }
    start = clock();
    quickSort(quickSortArr, 0, n - 1);
    end = clock();
    double quickSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 选择排序
    int selectionSortArr[n];
    for (int i = 0; i < n; i++) {
        selectionSortArr[i] = arr[i];
    }
    start = clock();
    selectionSort(selectionSortArr, n);
    end = clock();
    double selectionSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 插入排序
    int insertionSortArr[n];
    for (int i = 0; i < n; i++) {
        insertionSortArr[i] = arr[i];
    }
    start = clock();
    insertionSort(insertionSortArr, n);
    end = clock();
    double insertionSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 堆排序
    int heapSortArr[n];
    for (int i = 0; i < n; i++) {
        heapSortArr[i] = arr[i];
    }
    start = clock();
    heapSort(heapSortArr, n);
    end = clock();
    double heapSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 基数排序
    int radixSortArr[n];
    for (int i = 0; i < n; i++) {
        radixSortArr[i] = arr[i];
    }
    start = clock();
    radixSort(radixSortArr, n);
    end = clock();
    double radixSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Merge Sort: %.6f seconds\n", mergeSortTime);
    printf("Quick Sort: %.6f seconds\n", quickSortTime);
    printf("Selection Sort: %.6f seconds\n", selectionSortTime);
    printf("Insertion Sort: %.6f seconds\n", insertionSortTime);
    printf("Heap Sort: %.6f seconds\n", heapSortTime);
    printf("Radix Sort: %.6f seconds\n", radixSortTime);

    return 0;
}


