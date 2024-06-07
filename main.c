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
void func_timeavg(double (*operation)(long long int, double*, double*), long long int n, double* a, double* b, int loops, double* sdot, double* avgtime) {
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
    printf("Dot product of A and B is: %lf\n", *sdot);
}

double percent_err(double a, double b) {
    return fabs(a - b) / a * 100;
}

int main() {
    long long int ARR_SIZE = 1 << 26;
    double* a = (double*)malloc(ARR_SIZE * sizeof(double));
    double* b = (double*)malloc(ARR_SIZE * sizeof(double));

    srand(time(NULL)); // Random number seed

    // Generate random values between 1.0 - 10.0 per element
    for (int i = 0; i < ARR_SIZE; i++) {
        a[i] = random_double(1.0, 10.0);
        b[i] = random_double(1.0, 10.0);
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

    // Summary
    printf("\nDot product summary:\n");
    printf("  C function answer = %lf | Average time taken: %lf ms\n", sdot[0], avgtimes[0]);
    printf("x86 function answer = %lf | Average time taken: %lf ms\n", sdot[1], avgtimes[1]);
    printf("xmm function answer = %lf | Average time taken: %lf ms\n", sdot[2], avgtimes[2]);
    printf("ymm function answer = %lf | Average time taken: %lf ms\n", sdot[3], avgtimes[3]);

    // Percent errors 
    printf("\nPercentage error:\n");
    printf("x86 function error = %.3lf\n", percent_err(sdot[0], sdot[1]));
    printf("xmm function error = %.3lf\n", percent_err(sdot[0], sdot[2]));
    printf("ymm function error = %.3lf\n", percent_err(sdot[0], sdot[3]));

    // Boundary element test
    printf("\nBoundary element test:\n");
    printf("Initialize new arrays with size of 8:\n");

    double c[8], d[8];
    for (int i = 0; i < 8; i++) {
        c[i] = d[i] = i + 1;
        printf("c[%d] = %lf, d[%d] = %lf\n", i, c[i], i, d[i]);
    }

    printf("\nArr size 5:\n");
    printf("xmm and ymm would have 1 extra element\n");
    printf("  C function answer = %lf\n", dotproduct_C(5, c, d));
    printf("xmm function answer = %lf\n", dotproduct_xmm(5, c, d));
    printf("ymm function answer = %lf\n", dotproduct_ymm(5, c, d));

    printf("\nArr size 6:\n");
    printf("ymm would have 2 extra elements\n");
    printf("  C function answer = %lf\n", dotproduct_C(6, c, d));
    printf("xmm function answer = %lf\n", dotproduct_xmm(6, c, d));
    printf("ymm function answer = %lf\n", dotproduct_ymm(6, c, d));

    printf("\nArr size 7:\n");
    printf("xmm would have 1 extra element while ymm would have 3 extra elements\n");
    printf("  C function answer = %lf\n", dotproduct_C(7, c, d));
    printf("xmm function answer = %lf\n", dotproduct_xmm(7, c, d));
    printf("ymm function answer = %lf\n", dotproduct_ymm(7, c, d));

    return 0;
}