#include <immintrin.h>
#include <iostream>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
using namespace std;
const int size =512;
float __attribute__((aligned(16)))  A[size][size];
float __attribute__((aligned(16)))  B[size][size];
float c[size][size] __attribute__((aligned(16)))= {0};
float cc[size][size] __attribute__((aligned(16))) = {0};
void matmat( float mat_c[][size], float mat_a[][size],  float mat_b[][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            for (int j = 0; j < size; j++)
            {
                mat_c[i][j] += mat_a[i][k] * mat_b[k][j];

            }
        }
    }
}
void matmul_sse(){

    __m128 x,y,z;

    for(int i=0;i<size;i++){
         for (int k = 0; k < size; k++){
               x=_mm_set1_ps(A[i][k]);
             z[0]=z[1]=z[2]=z[3]=0;
              for (int  j= 0; j < size; j+=4){
                y=_mm_load_ps(&B[k][j]);
                x=_mm_mul_ps(x,y);
                c[i][j+0]+=x[0];
                c[i][j+1]+=x[1];
                c[i][j+2]+=x[2];
                c[i][j+3]+=x[3];

            }


        }

    }

}

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
            A[i][j] =rand()%9;
        }
    }
}
int main(){
    bool match = true;
    double before,after;
    initialMatrix(A,size);
    initialMatrix(B,size);
    before=seconds();
    matmat(cc,A,B);
    after=seconds();
    printf("NON-SSE Version Time: %f\n",after-before);
    before=seconds();
    matmul_sse();
    after=seconds();
     printf("SSE Version Time: %f\n",after-before);
       for (int i = 0; i < size; i++)
    {
        for(int j=0;j<size;j++){
        if (c[i][j] != cc[i][j])
        {   printf( "Match?");
            printf(ANSI_COLOR_RED " doesn't match \n");
            match = false;
            break;
        }
        break;
        }
        break;
    }


    if (match)
    {
        printf( "Match?");
        printf( ANSI_COLOR_GREEN  " Match \n    ");
    }


}
