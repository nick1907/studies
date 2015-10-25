import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

class ZnakKlucza
{
    public String znaki = "";

    public ZnakKlucza()
    {
        znaki = "";
    }

    public void wstaw(String mozliwosci)
    {
        if (!znaki.isEmpty())
        {
            String temp = "";

            // znajdz czesc wspolna
            for (int i = 0; i < znaki.length(); ++i)
            {
                for (int j = 0; j < mozliwosci.length(); ++j)
                {
                    if (mozliwosci.codePointAt(j) == znaki.codePointAt(i))
                        temp += znaki.charAt(i);
                }
            }

            if (!temp.isEmpty())
                znaki = temp;
        }
        else
            znaki = mozliwosci;
    }
}

public class Main
{
    public static void main(String[] args)
    {
        if (args.length == 2) // jesli podano nazwe pliku
        {
            int szyfrogramy[][] = null;
            int ilosc_szyfrogramow = Integer.parseInt(args[1]);

            szyfrogramy = pobierz_dane(args[0], ilosc_szyfrogramow, ilosc_szyfrogramow);

            if (szyfrogramy != null)
                zgadnij_klucz(szyfrogramy);
        } else
        {
            System.err.println("Podaj nazwe pliku jako parametr programu.\n" +
                    "Poprawne uzycie to: java Main nazwa_pliku ilosc_szyfrogramow");
        }
    }

    private static boolean czy_poprawny_znak(int znak)
    {
        // String poprawne = "abcdefghijklmnoprstuwvqyzABCDEFGHIJKLMNOPRSTUQWYVZ-,.()!? ";
        String poprawne = "aąbcćdeęfghijklłmnńoóprsśtuwvqyzżźAĄBCĆDEĘFGHIJKLŁMNOÓPRSŚTUQWYVZŻŹ-,.()!? ";

        for (int i = 0; i < poprawne.length(); ++i)
        {
            if (znak == poprawne.codePointAt(i))
                return true;
        }

        return false;
    }

    public static void zgadnij_klucz(int tablica[][])
    {
        String poprawne = "abcdefghijklmnoprstuwvqyzABCDEFGHIJKLMNOPRSTUQWYVZ-,.()!? ";

        int max_kolumna = 0;

        for (int i = 0; i < tablica.length; ++i)
        {
            if (max_kolumna < tablica[i].length)
                max_kolumna = tablica[i].length;
        }

        ZnakKlucza klucz[] = new ZnakKlucza[max_kolumna];

        for (int i = 0; i < max_kolumna; ++i)
                klucz[i] = new ZnakKlucza();

        int kolumna = 0;

        // poszukaj klucza
        while (kolumna < max_kolumna)
        {
            for (int wiersz = 0; wiersz < tablica.length; ++wiersz)
            {
                if (kolumna < tablica[wiersz].length)
                {
                    String mozliwosci = "";

                    for (int i = 0; i < 256; ++i)
                    {
                        if (czy_poprawny_znak((tablica[wiersz][kolumna] ^ i)))
                            mozliwosci += Character.toString((char)(i));
                    }

                    if (!mozliwosci.isEmpty())
                        klucz[kolumna].wstaw(mozliwosci);
                }
            }

            ++kolumna;
        }

        for (int wiersz = 0; wiersz < tablica.length; ++wiersz)
        {
            char poprzedni = 0;

            System.out.println("Komunikat nr: " + (wiersz + 1));
            for (kolumna = 0; kolumna < tablica[wiersz].length; ++kolumna)
            {
                if (klucz[kolumna].znaki.length() > 1)
                {
                    String wynik = "{";

                    for (int i = 0; i < klucz[kolumna].znaki.length(); ++i) {
                        if (czy_poprawny_znak(tablica[wiersz][kolumna] ^ klucz[kolumna].znaki.charAt(i)))
                        {
                            char temp = (char)(tablica[wiersz][kolumna] ^ klucz[kolumna].znaki.charAt(i));

                            if (Character.isLowerCase(poprzedni) && Character.isLowerCase(temp))
                                wynik += Character.toString(temp);
                            else if (Character.isUpperCase(poprzedni) && Character.isLowerCase(temp))
                                wynik += Character.toString(temp);
                            else if (poprzedni == ' ')
                                wynik += Character.toString(temp);
                            else if (poprzedni == '.' || poprzedni == ',' || poprzedni == '?' ||
                                     poprzedni == '!' || poprzedni == ';' && temp == ' ')
                                wynik += Character.toString(temp);
                        }
                    }
                    wynik += "}";

                    if (wynik.length() > 3)
                        System.out.print(wynik);
                    else if (wynik.length() == 2)
                        System.out.print("_");
                    else
                        System.out.print(wynik.charAt(1));
                }
                else if (klucz[kolumna].znaki.length() == 1) {
                    if (czy_poprawny_znak(tablica[wiersz][kolumna] ^ klucz[kolumna].znaki.charAt(0))) {
                        poprzedni = (char) (tablica[wiersz][kolumna] ^ klucz[kolumna].znaki.charAt(0));
                        System.out.print((char) (tablica[wiersz][kolumna] ^ klucz[kolumna].znaki.charAt(0)));
                    }
                }
            }

            System.out.println("\n");
        }
    }

    private static int[][] pobierz_dane(String nazwa_pliku, int ilosc_szyfrogramow, int dlugosc)
    {
        File plik = null;
        Scanner skaner = null;
        plik = new File(nazwa_pliku);

        if (ilosc_szyfrogramow < 0)
        {
            return null;
        }

        int tablica[][] = new int[ilosc_szyfrogramow][];

        if (plik.exists() && plik.canRead())
        {
            try
            {
                int indeks = 0;
                skaner = new Scanner(plik);
                String bajty[] = null;
                int i = 0;

                while (indeks < dlugosc && skaner.hasNext())
                {
                    String linia = skaner.nextLine();

                    if (!linia.isEmpty())
                        bajty = linia.split(" ");

                    if (bajty == null)
                        break;

                    tablica[i] = new int[bajty.length];

                    for (int j = 0; j < bajty.length; ++j)
                        tablica[i][j] = Integer.parseInt(bajty[j], 2);

                    ++indeks;
                    ++i;
                }

                if (skaner != null)
                    skaner.close();
            }
            catch (FileNotFoundException e)
            {
                System.out.println("Skaner nie mogl odczytac pliku.");
            }
        }
        else
        {
            System.err.println("Plik " + nazwa_pliku + " albo nie istnieje albo brak Ci "
                                + "praw dostepu.");
        }

        return tablica;
    }
}
