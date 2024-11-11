#include <GLUT/glut.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BAR_WIDTH (WINDOW_WIDTH / ARRAY_SIZE)
#define DELAY 10000 // Delay in microseconds

int arr[ARRAY_SIZE];

// Function to reset and randomize the array
void resetArray()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = rand() % ARRAY_SIZE + 1;
    }
}

// Function to draw the array as colored bars
void drawArray()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        float x1 = (float)i * BAR_WIDTH / WINDOW_WIDTH * 2.0f - 1.0f;
        float x2 = (float)(i + 1) * BAR_WIDTH / WINDOW_WIDTH * 2.0f - 1.0f;
        float y = (float)arr[i] / ARRAY_SIZE * 2.0f - 1.0f;

        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f); // Green bars
        glVertex2f(x1, -1.0f);
        glVertex2f(x2, -1.0f);
        glVertex2f(x2, y);
        glVertex2f(x1, y);
        glEnd();
    }
    glutSwapBuffers();
    usleep(DELAY);
}

// Bubble Sort
void bubbleSort()
{
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                drawArray();
            }
        }
    }
}

// Selection Sort
void selectionSort()
{
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < ARRAY_SIZE; j++)
        {
            if (arr[j] < arr[minIdx])
            {
                minIdx = j;
            }
        }
        if (minIdx != i)
        {
            int temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
            drawArray();
        }
    }
}

// Insertion Sort
void insertionSort()
{
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
            drawArray();
        }
        arr[j + 1] = key;
        drawArray();
    }
}

// Merge function for Merge Sort
void merge(int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
        drawArray();
    }
    while (i < n1)
    {
        arr[k++] = L[i++];
        drawArray();
    }
    while (j < n2)
    {
        arr[k++] = R[j++];
        drawArray();
    }
}

// Merge Sort
void mergeSort(int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(left, mid, right);
    }
}

// Partition function for Quick Sort
int partition(int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            drawArray();
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    drawArray();
    return i + 1;
}

// Quick Sort
void quickSort(int low, int high)
{
    if (low < high)
    {
        int pi = partition(low, high);
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

// Keyboard callback for sorting actions
void keyboard(unsigned char key, int x, int y)
{
    resetArray();
    drawArray();
    switch (key)
    {
    case 'b':
        bubbleSort();
        break;
    case 's':
        selectionSort();
        break;
    case 'i':
        insertionSort();
        break;
    case 'm':
        mergeSort(0, ARRAY_SIZE - 1);
        break;
    case 'q':
        quickSort(0, ARRAY_SIZE - 1);
        break;
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    resetArray();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Sorting Visualizer");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(drawArray);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}