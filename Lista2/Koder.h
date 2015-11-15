//
// Created by brzydki on 14/11/15.
//

#ifndef LISTA3_ZAD1_MCRYPT_KODER_H
#define LISTA3_ZAD1_MCRYPT_KODER_H

#include "Dane.h"

class Koder
{
private:
        char *klucz;
        char *tryb;
        char *algorytm;
        bool z_paddingiem;

        char *padding(char *wiadomosc, long rozmiar, long dopelnienie);
        char *wykonaj_padding(MCRYPT td, char *zawartosc_pliku,
                                     long rozmiar_pliku, long *dopelnienie);
        char *usun_padding(char *zawartosc_pliku, long rozmiar_pliku);
        char *utworz_iv(MCRYPT td);

public:
        Koder(char *key, char *option, char *algorithm);
        Dane * szyfruj(char *nazwa_pliku, bool deszyfrowac);
        Dane *dane;
        void oczysc();
        void oczysc_wszystko();
};


#endif //LISTA3_ZAD1_MCRYPT_KODER_H
