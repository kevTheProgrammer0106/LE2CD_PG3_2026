#include <stdio.h>
#include <iostream.>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

int Recursive(int salary, int hour, int fixedSalary)
{
	if ( salary >= fixedSalary )
	{
		printf("%d時間を満たした\n", hour);
		return hour;
	}

	printf("%d %d\n", salary, fixedSalary);

	salary = salary * 2 - 50;

	return Recursive(salary, hour + 1, fixedSalary + 1226);
}

int main()
{
		//コンソールをUTF-8にする設定
	system("chcp 65001 > nul");

	int salary = 100;
	int fixed = 1226;
	int hour = 0;
	int resullt1 = Recursive(salary, hour, fixed);

	return 0;
}