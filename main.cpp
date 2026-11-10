#include <stdio.h>
#include <stdlib.h>

template <typename Type>
Type Min(Type a, Type b)
{
    // 2つの値を比較して、小さい方を返す
    return ( a < b ) ? a : b;
}

int main()
{
    // コンソールをUTF-8にする設定
    system("chcp 65001 > nul");

    int numberInt = 4;
    float numberFloat = 4.5f;
    double numberDouble = 3.6;

    printf("===== Minimum Comparison =====\n");

    // int型の比較
    printf("Min<int>(4, 5) = %d\n",
        Min<int>(numberInt, 5));

    // float型の比較
    printf("Min<float>(4.5, 1.5) = %.1f\n",
        Min<float>(numberFloat, 1.5f));

    // double型の比較
    printf("Min<double>(3.6, 2.0) = %.1f\n",
        Min<double>(numberDouble, 2.0));

    return 0;
}