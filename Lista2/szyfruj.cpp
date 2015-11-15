#include <iostream>
#include <mcrypt.h>
#include "Koder.h"
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace std;
void menu_glowne();

int main(int argc, char *argv[])
{
	string haslo;
	string plik;
	string metoda;
	string algorytm;
	string wyjsciowy;
	int opcja = 1;
	
	menu_glowne();
	
	while ((cin>> opcja) && (opcja == 1 || opcja == 2))
	{
		getchar();
		if (opcja == 1)
			cout << "SZYFROWANIE:\n";
		else
			cout << "DESZYFROWANIE:\n";
		
		cout << "Podaj nazwe pliku: ";
		cin >> plik;
		
		cout << "Podaj nazwe pliku wyjsciowego:";
		cin >> wyjsciowy;
		
		getchar();
		cout << "Podaj haslo: ";
		getline(cin, haslo);
		
		cout << "haslo: \"" << haslo << "\"\n";
		cout << "Podaj algorytm: ";
		cin >> algorytm;
		
		cout << "Podaj metode: ";
		cin >> metoda;
		
		if (haslo.length() < 32)
        {
            for (long i = 0; i < (long)(32 - haslo.length()); ++i)
                haslo += haslo[i];
        }
        
		Koder *koder = new Koder((char *)haslo.c_str(), (char *)metoda.c_str(), (char *)algorytm.c_str());
		Dane *dane;
		if (opcja == 2)
		{
			dane = koder -> szyfruj((char *)plik.c_str(), true);
			dane -> do_pliku((char *)wyjsciowy.c_str());
		}
		else
		{
			dane = koder -> szyfruj((char *)plik.c_str(), false);
			dane -> do_pliku_z_paddingiem((char *)wyjsciowy.c_str());
		}	
		
		koder -> oczysc();
		
		menu_glowne();
	} 

    return 0;
}

void menu_glowne()
{
	cout << "Podaj odpowiednia opcje menu:\n";
	cout << "1) szyfrowanie pliku\n";
	cout << "2) deszyfrowanie pliku\n";
	cout << "inna --wyjscie\n";
}
