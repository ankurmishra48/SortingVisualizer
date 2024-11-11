#include <windows.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 100
#define BAR_WIDTH 10
#define WINDOW_WIDTH (ARRAY_SIZE * BAR_WIDTH)
#define WINDOW_HEIGHT 600
#define DELAY 10  // Delay in milliseconds

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int arr[ARRAY_SIZE];

// Function to draw the array as colored bars in the window
void drawArray(HDC hdc) {
    RECT rect;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int barHeight = (arr[i] * WINDOW_HEIGHT) / ARRAY_SIZE;
        rect.left = i * BAR_WIDTH;
        rect.right = rect.left + BAR_WIDTH;
        rect.top = WINDOW_HEIGHT - barHeight;
        rect.bottom = WINDOW_HEIGHT;

        // Set color for the bars
        HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));  // Green bars
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
}

void redraw(HWND hwnd) {
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
    Sleep(DELAY);
}

// Bubble Sort
void bubbleSort(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                redraw(hwnd);
                drawArray(hdc);
            }
        }
    }
    ReleaseDC(hwnd, hdc);
}

// Selection Sort
void selectionSort(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < ARRAY_SIZE; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            int temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;

            redraw(hwnd);
            drawArray(hdc);
        }
    }
    ReleaseDC(hwnd, hdc);
}

// Insertion Sort
void insertionSort(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    for (int i = 1; i < ARRAY_SIZE; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;

            redraw(hwnd);
            drawArray(hdc);
        }
        arr[j + 1] = key;
        redraw(hwnd);
    }
    ReleaseDC(hwnd, hdc);
}

// Merge Sort
void merge(HWND hwnd, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    HDC hdc = GetDC(hwnd);
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        redraw(hwnd);
        drawArray(hdc);
    }
    while (i < n1) {
        arr[k++] = L[i++];
        redraw(hwnd);
        drawArray(hdc);
    }
    while (j < n2) {
        arr[k++] = R[j++];
        redraw(hwnd);
        drawArray(hdc);
    }
    ReleaseDC(hwnd, hdc);
}

void mergeSort(HWND hwnd, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(hwnd, left, mid);
        mergeSort(hwnd, mid + 1, right);
        merge(hwnd, left, mid, right);
    }
}

// Quick Sort
int partition(HWND hwnd, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    HDC hdc = GetDC(hwnd);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            redraw(hwnd);
            drawArray(hdc);
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    redraw(hwnd);
    drawArray(hdc);
    ReleaseDC(hwnd, hdc);
    return (i + 1);
}

void quickSort(HWND hwnd, int low, int high) {
    if (low < high) {
        int pi = partition(hwnd, low, high);
        quickSort(hwnd, low, pi - 1);
        quickSort(hwnd, pi + 1, high);
    }
}

void resetArray() {
    srand(time(0));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % ARRAY_SIZE + 1;
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    resetArray();

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SortingVisualizer";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"Sorting Visualizer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        drawArray(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        resetArray();
        InvalidateRect(hwnd, NULL, TRUE);
        switch (wParam) {
            case 'B': bubbleSort(hwnd); break;
            case 'S': selectionSort(hwnd); break;
            case 'I': insertionSort(hwnd); break;
            case 'M': mergeSort(hwnd, 0, ARRAY_SIZE - 1); break;
            case 'Q': quickSort(hwnd, 0, ARRAY_SIZE - 1); break;
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
