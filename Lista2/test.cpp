#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		if (!strcmp(argv[2], "utworz"))
		{
			FILE *plik = fopen(argv[1], "wb");
			
			long x = 312;
			long y = 512;
			
			fwrite(&x, sizeof(long), 1, plik);
			fwrite(&y, sizeof(long), 1, plik);
			fwrite(argv[0], sizeof(char), strlen(argv[0]), plik);
			
			cout << endl;
			fwrite(&x, sizeof(long), 1, stdin);
			cout << endl;
			fwrite(&y, sizeof(long), 1, stdin);
			cout << endl;
			fwrite(argv[0], sizeof(char), strlen(argv[0]), stdin);
			
			fclose(plik);
		}
		else
		{
			FILE * plik = fopen(argv[1], "rb");
			long x;
			long y;
			char *tablica = new char[40];
			
			fread((long *)&x, sizeof(long), 1, plik);
			fread((long *)&y, sizeof(long), 1, plik);
			fread(tablica, sizeof(char), strlen(argv[0]), plik);
			
			fclose(plik);
			
			cout << "y wynosi: " << y << endl;
			cout << "x wynosi: " << x << endl;
			cout << "napis: " << tablica << endl;
			
			delete [] tablica;
		}
	}
	
	return 0;
}
