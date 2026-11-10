#include <stdio.h>
#include <Windows.h>

class Enemy
{
public:
    void Update();

    void Approach(); // 接近
    void Attack();   // 攻撃
    void Retreat();  // 離脱

    // 関数ポインタテーブル
    static void ( Enemy::*pDecision[] )( );
private:
    int index = 0;
};

void Enemy::Approach()
{
    printf("敵が接近 !\n");
}

void Enemy::Attack()
{
    printf("敵が攻撃！\n");
}

void Enemy::Retreat()
{
    printf("敵が離脱!\n");
}

void Enemy::Update()
{

// 関数テーブルから関数を実行
    ( this->*pDecision[index] )( );

    printf("次の状態に移行 (0: はい、 他: いいえ): ");
    int input;
    scanf_s("%d", &input);

    if ( input == 0 )
    {
        index = ( index + 1 ) % 3;
    }
}

// メンバ関数ポインタテーブル
void ( Enemy::* Enemy::pDecision[] )( ) = {
	&Enemy::Approach,
	&Enemy::Attack,
	&Enemy::Retreat
};

int main()
{
    // コンソールをUTF-8にする設定
    system("chcp 65001 > nul");

    Enemy enemy;
    while ( 1 ) enemy.Update();
    return 0;
}
