#include <stdio.h>

template <typename Type1, typename Type2>
class Minimum
{
private:
    Type1 value1_;
    Type2 value2_;

public:
    // 2つの値を受け取るコンストラクタ
    Minimum(Type1 a, Type2 b)
        : value1_(a), value2_(b)
    {
    }

    // 2つの値を比較して、小さい方を返す
    auto Min() const
    {
        return ( value1_ < value2_ ) ? value1_ : value2_;
    }
};

int main()
{
    printf("===== Minimum Comparison =====\n");

    // int型同士
    Minimum<int, int> minimumIntInt(4, 5);
    printf("Min<int, int>(4, 5) = %g\n",
        static_cast< double >( minimumIntInt.Min() ));

    // int型とfloat型
    Minimum<int, float> minimumIntFloat(4, 1.5f);
    printf("Min<int, float>(4, 1.5) = %g\n",
        static_cast< double >( minimumIntFloat.Min() ));

    // int型とdouble型
    Minimum<int, double> minimumIntDouble(4, 2.0);
    printf("Min<int, double>(4, 2.0) = %g\n",
        minimumIntDouble.Min());

    // float型同士
    Minimum<float, float> minimumFloatFloat(4.5f, 1.5f);
    printf("Min<float, float>(4.5, 1.5) = %g\n",
        static_cast< double >( minimumFloatFloat.Min() ));

    // float型とdouble型
    Minimum<float, double> minimumFloatDouble(4.5f, 2.0);
    printf("Min<float, double>(4.5, 2.0) = %g\n",
        minimumFloatDouble.Min());

    // double型同士
    Minimum<double, double> minimumDoubleDouble(3.6, 2.0);
    printf("Min<double, double>(3.6, 2.0) = %g\n",
        minimumDoubleDouble.Min());

    return 0;
}