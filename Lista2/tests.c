#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		FILE *plik_wej = fopen(argv[1], "rb");
		FILE *plik_wyj = fopen(argv[2], "wb");

		char *linia = (char *)malloc(4096 * sizeof(char));

		int pobrane;
		while ((pobrane = fread(linia, sizeof(char), 4096, plik_wej)) > 0)
		{
			fwrite(linia, sizeof(char), pobrane, plik_wyj);
		}

		fclose(plik_wej);
		fclose(plik_wyj);
	
	}

	return 0;
}
