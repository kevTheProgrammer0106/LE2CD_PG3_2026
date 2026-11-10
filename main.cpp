
#include <stdio.h>
#include <stdlib.h>

// インターフェース
class IShape
{
public:
    virtual ~IShape() = default;

    virtual void Size() = 0;
    virtual void Draw() = 0;
};

// 円
class Circle : public IShape
{
public:
    void Size() override
    {
        printf("Circleの半径を入力: ");
        scanf_s("%d", &radius);
    }

    void Draw() override
    {
        // 円の形をコンソールに描画する
        for ( int y = -radius; y <= radius; ++y )
        {
            for ( int x = -radius * 2; x <= radius * 2; ++x )
            {
                if ( x * x + 4 * y * y <= 4 * radius * radius )
                {
                    printf("*");
                } else
                {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }

private:
    int radius = 3;
};

// 四角形
class Rectangle : public IShape
{
public:
    void Size() override
    {
        printf("Rectangleの幅を入力: ");
        scanf_s("%d", &width);

        printf("Rectangleの高さを入力: ");
        scanf_s("%d", &height);
    }

    void Draw() override
    {
        // 幅と高さに合わせて四角形を描画する
        for ( int y = 0; y < height; ++y )
        {
            for ( int x = 0; x < width; ++x )
            {
                printf("*");
            }
            printf("\n");
        }
    }

private:
    int width = 5;
    int height = 3;
};

int main()
{
    // コンソールをUTF-8にする設定
    system("chcp 65001 > nul");

    Circle circle;
    Rectangle rectangle;

    // インターフェースを通して図形を管理する
    IShape* shapes[] = { &circle, &rectangle };

    printf("===== オブジェクトのサイズ設定 =====\n");

    // それぞれの図形のサイズを設定する
    for ( IShape* shape : shapes )
    {
        shape->Size();
        printf("\n");
    }

    printf("===== オブジェクトの描画 =====\n");

    // それぞれの図形を描画する
    for ( IShape* shape : shapes )
    {
        shape->Draw();
        printf("\n");
    }

    return 0;
}
