#define PI 3.14

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/*
1. M1:填充[1, 30]具有均匀规律的随机实数
2. M2:填充[30, 10*30]内的随机实数
3. M1[i] = (M1[i] / pai) ^ 3
4. M2[i] = |tan(M2[i] + M2[i - 1])|
5. M2[i] = M1[i] ^ M2[i]
6. M2：Gnome Sort
7. 计算M2中元素的正弦和，除以最小非零元素时给出一个偶数
    （确定奇偶性时只计算数字的整数部分）
8. 结果：单一的数字X，并行前后应在误差范围内不发生变化
    对不同的N，报告X的值
*/
void generateStage(double *a, size_t n, unsigned int min, unsigned int max, unsigned int seed)
{
    for(int i = 0; i < n; i++)
    {
        a[i] = min + 1.0 * rand_r(&seed) / RAND_MAX * (max - min);
    }
}

void mapStage(double *a1, double *a2, int n)
{
    for(int i = 0; i < n; i++)
    {
        a1[i] = pow(a1[i] / PI, 3);
    }

    a2[0] = fabs(tan(a2[0]));
    for(int i = 0; i < n / 2; i++)
    {
        a2[i] = fabs(tan(a2[i] + a2[i - 1]));
    }
}

void mergeStage(double *a1, double *a2, int n)
{
    for(int i = 0; i < n; i++)
    {
        a2[i] = pow(a1[i], a2[i]);
    }
}

void gnomeSort(double a[], int n)
{
    int k = 1;
    while (k < n)
    {
        if (a[k - 1] <= a[k])
        {
            k++;
        }
        else
        {
            double tmp = a[k - 1];
            a[k - 1] = a[k];
            a[k] = tmp;
            k--;
        }
    }
}

double reduceStage(double *a, size_t n)
{
    double res = 0;
    double min = a[0];

    for(int i = 0; i < n; i++)
    {
        if(a[i] < min && a[i] != 0)
        {
            min = a[i];
        }
    }

    for(int i = 0; i < n; i++)
    {
        if((int)(a[i] / min) % 2 == 0)
        {
            res += sin(a[i]);
        }
    }
    return res;
}


int main(int argc, char *argv[])
{
    //char *ch = "50000";
    unsigned int i;
    int N;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]);
    //N = atoi(ch);
    double *M1 = malloc(sizeof(double) * N);
    double *M2 = malloc(sizeof(double) * N / 2);
    gettimeofday(&T1, NULL);
    for (i = 0; i < 50; i++)
    {
       generateStage(M1, N, 1, 30, i);
       generateStage(M2, N / 2, 30, 30 * 10, i);

       mapStage(M1, M2, N);

       mergeStage(M1, M2, N / 2);

       gnomeSort(M2, N / 2);

       double X = reduceStage(M2, N / 2);
       printf("Stage: %d, X: %f\n", i + 1, X);
    }
    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN = %d. Milliseconds passed: %ld\n", N, delta_ms);
    return 0;
}