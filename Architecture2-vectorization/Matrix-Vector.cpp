#include <immintrin.h>
#include <iostream>
#include <stdio.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
using namespace std;
const int size = 512;
float __attribute__((aligned(16))) A[size][size];
float __attribute__((aligned(16))) B[size];
float c[size] = {0};
float cc[size] = {0};
double seconds()
{

  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void initialMatrix(float A[][size], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            A[i][j] = rand()%9;
        }
    }
}
void initialArray(float A[], int size)
{
    for (int i = 0; i < size; i++)
    {
        A[i] =rand()%5;
    }
}
static void matvec_unrolled(int n, float vec_c[], float mat_a[][size], float vec_b[])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j += 4)
            vec_c[i] += mat_a[i][j + 0] * vec_b[j + 0] + mat_a[i][j + 1] * vec_b[j + 1] + mat_a[i][j + 2] * vec_b[j + 2] + mat_a[i][j + 3] * vec_b[j + 3];
}
void matvec_sse()
{
    __m128 x, y, z;

    float b;

    for (int i = 0; i < size; i++)
    {
        b = 0;
        z[0] = z[1] = z[2] = z[3] = 0;
        for (int j = 0; j < size; j += 4)
        {
            x = _mm_load_ps(&A[i][j]);
            y = _mm_load_ps(&B[j]);
            x = _mm_mul_ps(x, y);
            z = _mm_add_ps(x, z);
        }
        for (int k = 0; k < 4; k++)
        {
            b += z[k];
        }
        c[i] = b;
        
    }
}
int main()
{
    double before, after;
    bool match = true;
    initialMatrix(A, size);
    initialArray(B, size);
    before=seconds();
    matvec_sse();
    after=seconds();
    printf("SSE Version Time:%f\n",after-before);
    
    before=seconds();
    matvec_unrolled(size, cc, A, B);
    after=seconds();

    printf("Non-SSE Version Time:%f\n",after-before);
    for (int i = 0; i < size; i++)
    {
        if (c[i] != cc[i])
        {   printf( "Match?");
            printf(ANSI_COLOR_RED " doesn't match \n");
            match = false;
            break;
        }
    }
    
    
    if (match)
    {
        printf( "Match?");
        printf( ANSI_COLOR_GREEN  " Match \n    ");
    }
}
