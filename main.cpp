#include <stdio.h>


int Recursive(int salary, int hour, int fixedSalary)
{
    if (salary >= fixedSalary)
    {
        printf("%d hours have exceeded\n", hour);
        return hour;
    }

    printf("%d %d\n", salary, fixedSalary);

    salary = salary * 2 - 50;

    return Recursive(salary, hour + 1, fixedSalary + 1226);
}

int main()
{

	int salary = 100;
	int fixed = 1226;
	int hour = 0;
	int resullt1 = Recursive(salary, hour, fixed);

	return 0;
}