#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// x86 Function to calculate the dot product of two vectors
extern double dotproduct_x86(double* a, double* b, size_t n);
// rcx = a, rdx = b, r8 = n

extern double dotproduct_xmm(double* a, double* b, size_t n);

extern double dottest(int n, double* a, double* b);

// C Function to calculate the dot product of two vectors
double dotproduct_C(double* a, double* b, size_t n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

void func_timeavg(double (*operation)(double*, double*, size_t), double* a, double* b, size_t n, int loops){
    // Variables related to timing
    LARGE_INTEGER freq, start, end;
    double time_used, sum_times = 0, avg_time, sdot; //sdot is the func output
    QueryPerformanceFrequency(&freq);

    for (int i = 0; i < loops; i++) {
        QueryPerformanceCounter(&start); // Record start time
        sdot = operation(a, b, n);
        QueryPerformanceCounter(&end); // Record end time
        time_used = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart; // Calculate elapsed time
        printf("Run %02d: Dot product of A and B is: %.2lf, Time taken: %lf ms\n", i + 1, sdot, time_used);
        sum_times += time_used;
    }

    avg_time = sum_times / loops; // Calculate average time
    printf("Average time taken for 30 runs: %lf seconds\n", avg_time);
}

int main() {
    const size_t ARR_SIZE = 8;
    double* a = (double*)malloc(ARR_SIZE * sizeof(double));
    double* b = (double*)malloc(ARR_SIZE * sizeof(double));

    srand(time(NULL)); // Random number seed

    for (int i = 0; i < ARR_SIZE; i++) {
        //a[i] = random_double(0.0, 20.0);
        //b[i] = random_double(0.0, 20.0);
        a[i] = i + 1;
        b[i] = i + 1;
    }

    //dotproduct_xmm(a, b, ARR_SIZE);

    int loops = 30;

    //dottest(6, a, b);

    // Run the dot product functions 30 times and measure the time

    // C
    func_timeavg(dotproduct_C, a, b, 7, loops);

    // X86
    func_timeavg(dotproduct_x86, a, b, 7, loops);

    // XMM
    func_timeavg(dotproduct_xmm, a, b, 7, loops);

    return 0;
}