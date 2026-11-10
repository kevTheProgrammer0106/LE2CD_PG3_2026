#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <memory>

template <typename DataType>
DataType Add(DataType a, DataType b)
{
    return static_cast< DataType >( a + b );
}

class Enemy
{
public:
    virtual ~Enemy() = default;

    virtual void Attack()
    {
        finalDamage = Add<int>(damage * 1.5, damage);
    }

    int GetFinalDamage() const
    {
        return finalDamage;
    }

protected:
    int damage = 50;
    int finalDamage = 0;
};

class EnemyMini : public Enemy
{
public:
    void Attack() override
    {
        finalDamage = Add<int>(damage * 1.2, damageElemental);
    }

protected:
    int damageElemental = 5;
};

int main()
{
    // コンソールをUTF-8にする設定
    system("chcp 65001 > nul");

    std::list<std::unique_ptr<Enemy>> enemies;

    const int maxEnemyCount = 9;

    for ( int index = 0; index < maxEnemyCount; ++index )
    {
        std::unique_ptr<Enemy> enemy;

        // 指定されたインデックスならEnemyMiniを生成
        if ( index == 3 || index == 5 )
        {
            enemy = std::make_unique<EnemyMini>();
        } else
        {
            enemy = std::make_unique<Enemy>();
        }

        // 敵の生成時に攻撃する
        enemy->Attack();

        printf("敵の[%d]番 TruEダメージ: %d\n",
            index, enemy->GetFinalDamage());

        enemies.push_back(std::move(enemy));
    }

    return 0;
}