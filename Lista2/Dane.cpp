//
// Created by brzydki on 15/11/15.
//

#include "Dane.h"
#include <iostream>

using namespace std;

void Dane::do_pliku_z_paddingiem(char *nazwa_pliku)
{
    FILE *plik = fopen(nazwa_pliku, "wb");

    do_pliku_z_paddingiem(plik);
}

void Dane::do_pliku_z_paddingiem(FILE *plik)
{
    if (plik) // jesli udalo sie otworzyc plik
    {
        fwrite(&this -> dlugosc_pliku, sizeof(long), 1, plik);
        fwrite(&this -> dlugosc_paddingu, sizeof(long), 1, plik);
        fwrite(this -> plaintext, sizeof(char), (size_t)(this -> dlugosc_pliku + this->dlugosc_paddingu), plik);

        fclose(plik);
    }
    else
    {
        perror("Zapis do pliku nie powiodl sie!\n");
    }
}

char * Dane::tablica_danych()
{
    return this -> plaintext;
}

Dane::Dane(char *tekst, long dopelnienie, long rozmiar_pliku)
{
    this -> plaintext = NULL;
    this -> dlugosc_pliku = rozmiar_pliku;
    this -> plaintext = tekst;
    this -> dlugosc_paddingu = dopelnienie;
}

void Dane::do_pliku(char *nazwa_pliku)
{
    FILE *plik = fopen(nazwa_pliku, "wb");

    do_pliku(plik);
}

void Dane::do_pliku(FILE *plik)
{
    if (plik) // jesli udalo sie otworzyc plik
    {
        fwrite(this -> plaintext, sizeof(char), (size_t)this -> dlugosc_pliku, plik);

        fclose(plik);
    }
    else
    {
        perror("Zapis do pliku nie powiodl sie!\n");
    }
}

void Dane::clear()
{
    if (plaintext) {
        delete[] plaintext;
        plaintext = NULL;
    }
}