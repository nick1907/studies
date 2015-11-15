//
// Created by brzydki on 15/11/15.
//

#ifndef LISTA3_ZAD1_MCRYPT_DANE_H
#define LISTA3_ZAD1_MCRYPT_DANE_H

#include <cstdio>

class Dane
{
private:
        char *plaintext;
        long dlugosc_paddingu;
        long dlugosc_pliku;

public:
        Dane(char *tekst, long dopelnienie, long rozmiar_pliku);
        void do_pliku_z_paddingiem(char *nazwa_pliku);
        void do_pliku_z_paddingiem(FILE *plik);
        char * tablica_danych();
        void do_pliku(char *nazwa_pliku);
        void do_pliku(FILE *plik);
        void clear();
};

#endif //LISTA3_ZAD1_MCRYPT_DANE_H
