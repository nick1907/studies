#include <iostream>
#include <mcrypt.h>
#include "Koder.h"
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    // argv[0] -- nazwa pliku uruchamianego
    // argv[1] -- typ algorytmu szyfrowania
    // argv[2] -- tryb szyfrowania
    // argv[3] -- nazwa pliku z kluczem (cala sciezka)
    // argv[4] -- plik do zaszyfrowania lub odszyfrowania
    // argv[5] -- plik wyjsciowy

    if (argc == 5 || argc == 6)
    {
        /*
        string keystore;

        FILE *plik = fopen(argv[3], "rb");
        char klucz[33];

        fread(klucz, sizeof(char), 32, plik);

        fclose(plik);

        // ukryj echo
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // pobierz klucz do keystore'a
        cout << "Podaj haslo do keystore'a:";
        getline(cin, keystore);

        // przywroc echo
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        if (keystore.length() < 32)
        {
            for (long i = 0; i < (long)(32 - keystore.length()); ++i)
                keystore += keystore[i];
        }

        Koder *koder_hasla = new Koder((char *)keystore.c_str(),
                                       (char *) "ecb", (char *)"aes");

        Dane *odczytane_haslo = koder_hasla -> szyfruj((char *)"klucz.txt", true);

        strcpy(klucz, odczytane_haslo -> tablica_danych());

        cout << "klucz to: \"" << klucz << "\"" << endl;
        Koder *koder = new Koder(klucz, argv[2], argv[1]);

        Dane *zaszyfrowane = koder->szyfruj(argv[4], false);

        zaszyfrowane -> do_pliku_z_paddingiem(argv[5]);
        koder ->oczysc();

         */

        Koder *koder_hasla = new Koder((char *)"ajlskfdjlakwer,ma.sd,fsa342ljfas",
                                       (char *) "cbc", (char *)"aes");

        Dane *zaszyfrowane = koder_hasla->szyfruj(argv[4], true);
        zaszyfrowane -> do_pliku(argv[5]);
        koder_hasla ->oczysc();
    }

    return 0;
}