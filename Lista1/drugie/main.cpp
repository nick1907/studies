#include <iostream>
#include <fstream>
#include <mcrypt.h>
#include <cstring>
#include <cctype>

using namespace std;

class Klucz
{
private:
        unsigned int indeks_znaku;
        unsigned int odkryty_znak;
        string klucz;
        string suffix;

        bool czy_nalezy(char znak)
        {
            if (znak >= '0' && znak <= '9')
                return true;
            else if (znak >= 'a' && znak <= 'f')
                return true;
            else if (znak >= 'A' && znak <= 'F')
                return true;

            return false;
        }
public:
        Klucz(string klucz)
        {
            this->suffix = klucz;
            this->klucz = "________";
            this->klucz += klucz;
            indeks_znaku = 0;
            odkryty_znak = 0;
        }

        string zwroc_suffix()
        {
            return suffix;
        }

        char zwroc_znak()
        {
            if (indeks_znaku < 8) // jesli to nie odkryta czesc klucza
            {
                indeks_znaku = (indeks_znaku + 1) % 16;
                return ' ';
            }
            else // jesli to odkryta czesc klucza
            {
                char znak = klucz[indeks_znaku];
                indeks_znaku = (indeks_znaku + 1) % 16;

                return znak;
            }
        }

        bool odkryj_znak(char znak) // jesli juz odkryto znaki klucza zwraca false
        {
            if (odkryty_znak < 8)
            {
                klucz[odkryty_znak] = (czy_nalezy(znak) ? znak : '_');
                return true;
            }

            return false;
        }
};

bool sprawdz_string(string napis, unsigned long dlugosc);
int rc4(char *strumien_klucza, char *klucz);
void brute_force(int *znaki, Klucz klucz, unsigned long dlugosc);
int bin_to_ascii(string kod_binarny);
string * podziel_string(string napis, char rozdzielacz, unsigned long *liczba_elem = NULL);
bool czy_dozwolony_znak(char znak);
void kopiuj(char *cel, string zrodlo, unsigned long indeks);
string moj_rc4(int *wiadomosc, char *klucz, unsigned long dlugosc);
bool sprawdz_char(char *napis, unsigned long dlugosc);

int main(int argc, char *argv[])
{
    // jesli podano plik
    if (argc == 2)
    {
        ifstream plik(argv[1]);
        string linia;
        string suffix_hasla;


        if (plik.is_open()) // jesli udalo sie otworzyc plik
        {
            getline(plik, linia); // pobierz wiadomosc
            getline(plik, suffix_hasla); // pobierz koncowke hasla

            Klucz klucz = Klucz(suffix_hasla);

            unsigned long liczba_znakow = 0;

            // znaki wiadomosci
            string *kody_binarne = podziel_string(linia, ' ', &liczba_znakow);
            int *znaki = new int[(int)liczba_znakow];
            linia.clear();

            // kody ASCII znakow z zadania
            for (unsigned long i = 0; i < liczba_znakow; ++i)
            {
                znaki[i] = bin_to_ascii(kody_binarne[i]);
                kody_binarne[i].clear();
            }

            // kasowanie zaalokowanej pamieci
            delete [] kody_binarne;

            brute_force(znaki, klucz, liczba_znakow);

            delete [] znaki;
        }
        else // jesli nie udalo sie otworzyc pliku
        {
            perror("Nie udalo sie otworzyc pliku, wiec koncze.");
            return 2;
        }
    }
    else
    {
        perror("Niestety, niepoprawne uzycie."
               "\nPoprawne to: ./nazwa_programu nazwa_pliku");
        return 1;
    }

    return 0;
}

// funkcja zamienia podany kod binarny na znak w kodzie ASCII
int bin_to_ascii(string kod_binarny)
{
//    char znak = 0;
    int liczba = 0;
    int potega = 1;

    for (long i = kod_binarny.length() - 1; i >= 0; --i)
    {
        if (kod_binarny[i] == '1')
            liczba += potega;

        potega *= 2;
    }

    return liczba;
}

// dzieli string wedlug podanego rozdzielacza
string * podziel_string(string napis, char rozdzielacz, unsigned long *liczba_elem)
{
    string *tablica;
    unsigned long liczba_elementow = 1;

    for (unsigned long i = 0; i < napis.length(); ++i)
    {
        if (napis[i] == rozdzielacz)
            ++liczba_elementow;
    }

    tablica = new string[liczba_elementow];

    for (unsigned long i = 0, j = 0; i < napis.length(); ++i)
    {
        if (napis[i] == rozdzielacz)
        {
            ++j;
            continue;
        }
        else
            tablica[j] += napis[i];
    }

    if (liczba_elem)
        *liczba_elem = liczba_elementow;

    return tablica;
}

void brute_force(int *znaki, Klucz klucz, unsigned long dlugosc)
{
//    char dozwolone[] = "3489abd";
    char dozwolone[] = "0123456789abcdef";
    unsigned long zakres = strlen(dozwolone);
    unsigned long pierwszy = 0, drugi = 0, trzeci = 0, czwarty = 0, piaty = 0, szosty = 0, siodmy = 0, osmy = 0;
    char key[17];
//    char strumien_klucza[256];

    kopiuj(key, klucz.zwroc_suffix(), 8);

    cout << endl << endl;

    while (osmy < zakres)
    {
        if (pierwszy >= zakres)
        {
            ++drugi;
            pierwszy = 0;
        }

        if (drugi >= zakres)
        {
            ++trzeci;
            drugi = 0;
        }

        if (trzeci >= zakres)
        {
            ++czwarty;
            trzeci = 0;
        }

        if (czwarty >= zakres)
        {
            ++piaty;
            czwarty = 0;
        }

        if (piaty >= zakres)
        {
            ++szosty;
            piaty = 0;
        }
        if (szosty >= zakres)
        {
            ++siodmy;
            szosty = 0;
        }

        if (siodmy >= zakres)
        {
            ++osmy;
            siodmy = 0;
        }

        if (osmy >= zakres)
        {
            break;
        }

        key[0] = dozwolone[osmy];
        key[1] = dozwolone[siodmy];
        key[2] = dozwolone[szosty];
        key[3] = dozwolone[piaty];
        key[4] = dozwolone[czwarty];
        key[5] = dozwolone[trzeci];
        key[6] = dozwolone[drugi];
        key[7] = dozwolone[pierwszy];

        /*
        // implementacja z mcrypt
        rc4(strumien_klucza, key);

        for (unsigned long x = 0; x < dlugosc; ++x)
            strumien_klucza[x] = (char)(znaki[x] ^ strumien_klucza[x]);

 //       cout << "klucz to: " << key << endl;

        if (sprawdz_string(strumien_klucza, dlugosc) && strumien_klucza[0] != '\0')
        {
            for (unsigned long x = 0; x < dlugosc; ++x)
                cout << strumien_klucza[x];
        }

        */

        // moja implementacja
        string temp = moj_rc4(znaki, key, dlugosc);

        if (sprawdz_string(temp, dlugosc) && temp.length() > 1)
        {
            for (unsigned long x = 0; x < dlugosc; ++x)
                cout << temp[x];

            break;
        }

        if (key[0] == '5')
            break;

        temp.clear();
        ++pierwszy;
        break;
    }
}

int rc4(char *strumien_klucza, char *klucz)
{
    int wynik;
    int iv[256];

    for (int i = 0; i < 256; ++i)
        iv[i] = i;

    MCRYPT td = mcrypt_module_open((char*)"arcfour", NULL, (char*)"stream", NULL);
    mcrypt_generic_init(td, klucz, 16, iv);
    wynik = mdecrypt_generic(td, strumien_klucza, mcrypt_enc_get_block_size(td));
    mcrypt_generic_deinit(td);
    mcrypt_module_close(td);

    return wynik;
}

bool sprawdz_string(string napis, unsigned long dlugosc)
{
    unsigned int zgodnosc = 0;

    for (unsigned long i = 0; i < dlugosc; ++i)
    {
        if (czy_dozwolony_znak(napis[i]))
            ++zgodnosc;
    }

    if (zgodnosc == dlugosc)
        return true;

    return false;
}

bool sprawdz_char(char *napis, unsigned long dlugosc)
{
    unsigned int zgodnosc = 0;

    for (unsigned long i = 0; i < dlugosc; ++i)
    {
        if (czy_dozwolony_znak(napis[i]))
            ++zgodnosc;
    }

    if (zgodnosc == dlugosc)
        return true;

    return false;
}

// sprawdz czy podany znak miesci sie w dozwolonym przedziale
bool czy_dozwolony_znak(char znak)
{
    if (isgraph(znak) || znak == ' ')
        return true;

    return false;
}


void kopiuj(char *cel, string zrodlo, unsigned long indeks)
{
    unsigned long i;
    for (i = 0; i < 8; ++i) {
        cel[indeks++] = zrodlo.at(i);
    }

    cel[indeks] = '\0';
}

string moj_rc4(int *wiadomosc, char *klucz, unsigned long dlugosc)
{
    string zwracana;
    unsigned long dlugosc_klucza = strlen(klucz);
    int tablica[256];
    int i;

    for (i = 0; i < 256; ++i)
        tablica[i] = i;

    int j = 0;

    for (i = 0; i < 256; ++i)
    {
        j = (j + tablica[i] + (int)klucz[i % dlugosc_klucza]) % 256;
        int temp = tablica[i];
        tablica[i] = tablica[j];
        tablica[j] = temp;
    }

    j = 0;
    i = 0;

    for (unsigned long indeks = 0; indeks < 256; ++indeks)
    {
        i = (i + 1) % 256;
        j = (j + tablica[i]) % 256;
        int temp = tablica[i];
        tablica[i] = tablica[j];
        tablica[j] = temp;

        zwracana += (char)(wiadomosc[indeks] ^ tablica[(tablica[i] + tablica[j]) % 256]);
    }

    return zwracana;
}
