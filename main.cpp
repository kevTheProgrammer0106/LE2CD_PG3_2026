#include <stdio.h>
#include <iostream.>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>



void DelayReveal(void ( *fn )( int, int ), unsigned int delayMs, int roll, int userGuess)

{
	Sleep(delayMs);
	//コールバック関数を呼び出す
	fn(roll, userGuess);
}
void ShowResult(int roll, int userGuess)

{
	if ( roll % 2 == 1 )
	{
		printf("Face %d is odd number\n", roll);
	} else if ( roll % 2 == 0 )
	{
		printf("Face %d is even number\n", roll);
	}

	if ( ( userGuess == 0 && roll % 2 == 0 ) || ( userGuess == 1 && roll % 2 == 1 ) )
	{
		printf("correct");
	} else
	{
		printf("incorrect");
	}
}

int main()
{
	//コンソールをUTF-8にする設定
	system("chcp 65001 > nul");

	srand(( unsigned ) time(NULL));
	int userInput = -1;
	while ( userInput != 0 && userInput != 1 )
	{
		printf("Pick a number from 0 to 1: ");
		scanf_s("%d", &userInput);
		if ( userInput != 0 && userInput != 1 )
		{
			printf("Invalid.\n");
		} else
		{
			printf("Results In.\n");
			break;
		}
	}
	int roll = rand() % 6 + 1;
	DelayReveal(ShowResult, 3000, roll, userInput);

	return 0;
}