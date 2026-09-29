#include <stdio.h>
#include <list>

using namespace std;

int main()
{
	//山手線1970年
	std::list<const char*> stations{
		"Tokyo", "Kanda", "Akihabara", "Okachimachi", "Ueno", 
		"Uguisudani", "Nippori", "Tabata", "Komagome", "Sugamo",
		"Otsuka", "Ikebukuro", "Mejiro", "Takadanobaba", "Shin-Okubo", 
		"Shinjuku", "Yoyogi", "Harajuku", "Shibuya", "Ebisu", "Meguro", "Gotanda",
		"Osaki", "Shinagawa", "Tamachi", "Hamamatsucho", "Shimbashi", "Yurakucho",
	};

	printf("Yamanote Line Station History\n");
	printf("Year 1970\n");
	for ( list<const char*>::iterator it = stations.begin(); it != stations.end(); ++it )
	{
		if ( std::next(it) == stations.end() )
		{
			printf("%s\n", *it);
		} else
		{
			printf("%s, ", *it);
		}
	}

	printf("Year 2019\n");
	list<const char*>::iterator it = find(stations.begin(), stations.end(), "Tabata");
	stations.insert(it, "Nishi-Nippori");
	for ( list<const char*>::iterator it = stations.begin(); it != stations.end(); ++it )
	{
		if ( next(it) == stations.end() )
		{
			printf("%s\n", *it);
		} else
		{
			printf("%s, ", *it);
		}
	}

	printf("Year 2022\n");
	it = find(stations.begin(), stations.end(), "Tamachi");
	stations.insert(it, "Takanawa Gateway");
	for ( list<const char*>::iterator it = stations.begin(); it != stations.end(); ++it )
	{
		if ( next(it) == stations.end() )
		{
			printf("%s\n", *it);
		} else
		{
			printf("%s, ", *it);
		}
	}
	return 0;
}