#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// x86 Function to calculate the dot product of two vectors
extern double dotproduct_x86(long long int n, double* a, double* b);

// xmm Function to calculate the dot product of two vectors
extern double dotproduct_xmm(long long int n, double* a, double* b);

// ymm Function to calculate the dot product of two vectors
extern double dotproduct_ymm(long long int n, double* a, double* b);

// C Function to calculate the dot product of two vectors
double dotproduct_C(long long int n, double* a, double* b) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// Generate random double value between 'min' and 'max'
double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Time averaging function
void func_timeavg(double (*operation)(long long int, double*, double*), long long int n, double* a, double* b, int loops, double* sdot, double* avgtime){
    // Variables related to timing
    LARGE_INTEGER freq, start, end;
    double time_used, sum_times = 0;
    QueryPerformanceFrequency(&freq);

    for (int i = 0; i < loops; i++) {
        QueryPerformanceCounter(&start); // Record start time
        *sdot = operation(n, a, b);
        QueryPerformanceCounter(&end); // Record end time
        time_used = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart; // Calculate elapsed time
        printf("Run %02d Time taken: %lf ms\n", i + 1, time_used);
        sum_times += time_used;
    }

    *avgtime = sum_times / loops; // Calculate average time
    printf("Average time taken for 30 runs: %lf ms\n", *avgtime);
    printf("Dot product of A and B is: %.2lf\n", *sdot);
}

int main() {
    long long int ARR_SIZE = 1<<20;
    double* a = (double*)malloc(ARR_SIZE * sizeof(double));
    double* b = (double*)malloc(ARR_SIZE * sizeof(double));

    srand(time(NULL)); // Random number seed

    for (int i = 0; i < ARR_SIZE; i++) {
        a[i] = random_double(0.0, 20.0);
        b[i] = random_double(0.0, 20.0);
        //a[i] = i + 1;
        //b[i] = i + 1;
    }

    int loops = 30;
    double sdot[4], avgtimes[4];

    // Run the dot product functions 30 times and measure the time

    // C
    printf("\nDot product function in C:\n");
    func_timeavg(dotproduct_C, ARR_SIZE, a, b, loops, &sdot[0], &avgtimes[0]);

    // X86
    printf("\nDot product function in x86:\n");
    func_timeavg(dotproduct_x86, ARR_SIZE, a, b, loops, &sdot[1], &avgtimes[1]);

    // XMM
    printf("\nDot product function in xmm:\n");
    func_timeavg(dotproduct_xmm, ARR_SIZE, a, b, loops, &sdot[2], &avgtimes[2]);

    // YMM
    printf("\nDot product function in ymm:\n");
    func_timeavg(dotproduct_ymm, ARR_SIZE, a, b, loops, &sdot[3], &avgtimes[3]);

    printf("\nDot product summary:\n");
    printf("  C function answer = %.2lf | Average time taken: %lf ms\n", sdot[0], avgtimes[0]);
    printf("x86 function answer = %.2lf | Average time taken: %lf ms\n", sdot[1], avgtimes[1]);
    printf("xmm function answer = %.2lf | Average time taken: %lf ms\n", sdot[2], avgtimes[2]);
    printf("ymm function answer = %.2lf | Average time taken: %lf ms\n", sdot[3], avgtimes[3]);

    return 0;
}