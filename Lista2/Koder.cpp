//
// Created by brzydki on 14/11/15.
//

#include <iostream>
#include <cstring>
#include <mcrypt.h>
#include "Koder.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Dane.h"

using namespace std;

Koder::Koder(char *key, char *option, char *algorithm)
{
    this -> klucz = key;

    // jesli to szyfr blokowy
    if (option && (!strcmp(option, "ecb") || !strcmp(option, "cbc")))
        z_paddingiem = true;
    else if (option && !strcmp(option, "cfb"))
        z_paddingiem = false;

    this -> tryb = option;
    this -> algorytm = algorithm;
    dane = NULL;
}

Dane * Koder::szyfruj(char *nazwa_pliku, bool deszyfrowac)
{
    oczysc();
    char *zaszyfrowany = NULL;
    FILE *plik = fopen(nazwa_pliku, "rb");
    long rozmiar_pliku = 0;
    char *zawartosc_pliku;
    long dopelnienie = 0;
    MCRYPT td = NULL;
    char *IV = NULL;

    if (plik)
    {
        // pobierz rozmiar pliku
        fseek(plik, 0L, SEEK_END);
        rozmiar_pliku = ftell(plik);

        // pobierz zawartosc pliku
        fseek(plik, 0L, SEEK_SET);

        if (!deszyfrowac)
        {
            cout << "szyfrowanie:\n";
            zawartosc_pliku = new char[rozmiar_pliku];
            fread(zawartosc_pliku, sizeof(char), rozmiar_pliku, plik);
        }
        else
        {
            cout << "deszyfrowanie:\n";
            fread(&rozmiar_pliku, sizeof(long), 1, plik);
            cout << "rozmiar pliku: " << rozmiar_pliku << endl;
            fread(&dopelnienie, sizeof(long), 1, plik);
            cout << "rozmiar dopelnienia: " << dopelnienie << endl;
            zawartosc_pliku = new char [rozmiar_pliku + dopelnienie];
            fread(zawartosc_pliku, sizeof(char), rozmiar_pliku + dopelnienie, plik);
        }

        // jesli to szyfr z paddingiem
        if (z_paddingiem)
        {
            if (this->tryb && !strcmp(this->tryb, "cbc"))
            {
                td = mcrypt_module_open((char *) MCRYPT_RIJNDAEL_256, NULL,
                                        (char *) MCRYPT_CBC, NULL);

                IV = utworz_iv(td);
            }
            else if (this->tryb && !strcmp(this->tryb, "ecb"))
                td = mcrypt_module_open((char *) MCRYPT_RIJNDAEL_256, NULL,
                                        (char *) MCRYPT_ECB, NULL);

            // pobierz rozmiar bloku
            long rozmiar_bloku = mcrypt_enc_get_block_size(td);

            cout << "rozmiar bloku to: " << rozmiar_bloku << endl;
            // wykonaj padding
            if (!deszyfrowac)
                zaszyfrowany = wykonaj_padding(td, zawartosc_pliku, rozmiar_pliku, &dopelnienie);
            else
                zaszyfrowany = zawartosc_pliku;

            // inicjalizuj mcrypt z odpowiednim kluczem i IV
            mcrypt_generic_init(td, klucz, (int)strlen(klucz), IV);

            char *temp = new char[(int)rozmiar_bloku];

            cout << "operacja glowna\n";
            // wykonaj szyfrowanie
            for (long i = 0; i < rozmiar_pliku + dopelnienie; i += rozmiar_bloku)
            {
                for (long j = 0; j < rozmiar_bloku; ++j)
                    temp[j] = zaszyfrowany[i + j];

                if (!deszyfrowac)
                    mcrypt_generic(td, temp, (int)rozmiar_bloku);
                else
                    mdecrypt_generic(td, temp, (int)rozmiar_bloku);

                for (long j = 0; j < rozmiar_bloku; ++j)
                    zaszyfrowany[i + j] = temp[j];
            }

            delete [] temp;

            if (deszyfrowac)
                zaszyfrowany = usun_padding(zaszyfrowany, rozmiar_pliku);

            if (dane)
                dane -> clear();

            cout << "zapis danych.\n";

            dane = new Dane(zaszyfrowany, dopelnienie, rozmiar_pliku);
        }
        else if (!z_paddingiem)
        {
            if (this -> tryb && !strcmp(this -> tryb, "cfb"))
            {
                td = mcrypt_module_open((char *) MCRYPT_RIJNDAEL_256, NULL,
                                        (char *) MCRYPT_CFB, NULL);

                IV = utworz_iv(td);
            }

            // inicjalizuj mcrypt z odpowiednim kluczem i IV
                mcrypt_generic_init(td, klucz, (int)strlen(klucz), IV);

            zaszyfrowany = zawartosc_pliku;

            for (long i = 0; i < rozmiar_pliku; ++i)
            {
                if (!deszyfrowac)
                    mcrypt_generic(td, &zaszyfrowany[i], 1);
                else
                    mdecrypt_generic(td, &zaszyfrowany[i], 1);
            }

            if (dane)
                dane -> clear();
            dane = new Dane(zaszyfrowany, dopelnienie, rozmiar_pliku);
        }

        // zamknij mcrypt
        mcrypt_generic_deinit (td);
        mcrypt_module_close(td);

        if (IV)
            delete [] IV;
    }

    return dane;
}

char * Koder::padding(char *wiadomosc, long rozmiar, long dopelnienie)
{
    char *dopelniony = new char [rozmiar + dopelnienie];

    if (!wiadomosc)
        return NULL;

    long i;

    for (i = 0; i < rozmiar; ++i)
        dopelniony[i] = wiadomosc[i];

    for (i = rozmiar; i < rozmiar + dopelnienie; ++i)
        dopelniony[i] = 0;

    delete [] wiadomosc;

    return dopelniony;
}

// zwraca wiadomosc odpowiednio dopelniona
// a do zmiennej padding wstawia obliczone dopelnienie
char *Koder::wykonaj_padding(MCRYPT td, char *zawartosc_pliku, long rozmiar_pliku,
                            long *dopelnienie)
{
    char *dopelniony;

    int rozmiar_bloku = mcrypt_enc_get_block_size(td);

    dopelniony = zawartosc_pliku;

    // jesli potrzeba dopelnienia
    if (rozmiar_bloku != 0 && rozmiar_pliku % rozmiar_bloku != 0)
    {
        *dopelnienie = ((rozmiar_pliku / rozmiar_bloku) + 1) * rozmiar_bloku;
        *dopelnienie -= rozmiar_pliku;

        dopelniony = padding(zawartosc_pliku, rozmiar_pliku, *dopelnienie);
    }

    return dopelniony;
}

char * Koder::utworz_iv(MCRYPT td)
{
    char *iv = NULL;
    srand(time(0));

    if (td)
    {
        int rozmiar = mcrypt_enc_get_iv_size(td);

        iv = new char[rozmiar];

        for (int i = 0; i < rozmiar; ++i)
            iv[i] = (char) (rand() % 128);
    }

    return iv;
}

char * Koder::usun_padding(char *zawartosc_pliku, long rozmiar_pliku)
{
    char *dane_bez_paddingu = new char[rozmiar_pliku];

    for (long i = 0; i < rozmiar_pliku; ++i)
        dane_bez_paddingu[i] = zawartosc_pliku[i];

    delete [] zawartosc_pliku;

    return dane_bez_paddingu;
}

void Koder::oczysc()
{
    if (dane)
    {
        dane -> clear();

        delete dane;
        dane = NULL;
    }
}

void Koder::oczysc_wszystko()
{
    oczysc();

    if (this->algorytm) {
        delete[] algorytm;
        algorytm = NULL;
    }

    if (this->klucz) {
        delete[] klucz;
        klucz = NULL;
    }

    if (this -> tryb) {
        delete[] tryb;
        tryb = NULL;
    }
}