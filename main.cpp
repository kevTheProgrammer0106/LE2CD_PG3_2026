#include <stdio.h>
#include <Windows.h>

typedef void ( *PFunc )( int* );

// コールバック関数
void DispResult(int* s)
{
	printf("%d seconds waited before execution\n", *s);
}

void SetTimeout(PFunc p, int second)
{
	//コールバック関数を呼び出す
	Sleep(second * 1000);
	p(&second);
}

int main()
{
	printf("start\n");

	PFunc p;
	p = DispResult;
	SetTimeout(p, 5);

	return 0;
}