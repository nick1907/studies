import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class Main
{
	static int dlugosc = 21;
	
	//dostepna pula znakow
	static String tab = "aąbcćdeęfghijklłmnńoprsśtuwyzżźABCĆDEFGHIJKLMNOPRSTUWYZŻŹ/\"-,.()!? ";
	
	private static boolean czyMalaLitera(int znak)
	{
		if (znak >= (int)'a' && znak <= (int)'ź')// && znak != 'v' && znak != 'q' && znak != 'x')
			return true;
		
		return false;
	}
	
	private static boolean czyDuzaLitera(int znak)
	{
		if (znak >= (int)'A' && znak <= (int)'Ź' && znak != (int)'Y')// && znak != 'V' && znak != 'Q' && znak != 'X')
			return true;
		
		return false;
	}
	
	private static boolean czyInny(int znak)
	{
		if (znak >= (int)',' && znak <= (int)'.')
			return true;
		else if (znak == (int)'(' || znak == (int)')' || znak == (int)'!' || znak == (int)' ')
			return true;
		
		return false;
	}
	
	private static boolean czyDopuszczony(int znak)
	{
		for (int i = 0; i < tab.length(); ++i)
			if (znak == (int)(tab.charAt(i)))
				return true;
		
		return false;
	}
	
	private static void skasujNieodpowiednie(List<Integer> lista, int liczba, String typ)
	{
		List<Integer> doSkasowania = new LinkedList<Integer>();
		for (int i = 0; i < lista.size(); ++i)
		{
			if (typ.equals("male") && !czyInny(liczba ^ lista.get(i)))
			{
				if (!czyMalaLitera(liczba ^ lista.get(i)))
					doSkasowania.add(i);
//					lista.remove(liczba ^ lista.get(i));
			}
			else if (typ.equals("duze"))
			{
				if (!czyDuzaLitera(liczba ^ lista.get(i)) && !czyInny(liczba ^ lista.get(i)))
					doSkasowania.add(i);
//					lista.remove(liczba ^ lista.get(i));
			}
			else if (typ.equals("inne"))
				if (!czyInny(liczba ^ lista.get(i)))
					doSkasowania.add(i);
//					lista.remove(liczba ^ lista.get(i));
		}
		
		for (int j = 0; j < doSkasowania.size(); ++j)
			lista.remove(doSkasowania.get(j));		
	}

	private static void zgadnijKlucz(int tablica[][])
	{
		int zakres = 0;
		for (int i = 0; i < tablica.length; ++i)
			if (zakres < tablica[i].length)
				zakres = tablica[i].length;

		int wynik[] = new int[zakres];
//		int i = 0;
		int indeks = 0;
		int j = 0;
		int m = 0;
//		do
//		{
			List<Integer> klucze = new LinkedList<Integer>();
			for (int i = 0; i < zakres /*tablica[j].length*/; ++i)
			{
				m = 0;
				klucze.clear();
				if (tablica[j].length > i)
					klucze = brute_force(tablica[j][i]);
//				klucze = skasujpowtorzenia(klucze);
				
				while (m < tablica.length)
				{
					if (tablica[m].length > i)
						klucze = czyKluczSiePowtarza(klucze, tablica[m][i]);
					if (i < tablica[m].length)
					if (i - 2 >= 0)
					{
						if (tablica[m][i-2] == (int)'.' || tablica[m][i-2] == (int)'!' || tablica[m][i-2] == (int)'?')
							if (tablica[m][i-1] == ' ')
								skasujNieodpowiednie(klucze, tablica[m][i], "duze");
					}
					else if (i - 1 >= 0)
					{
						if (tablica[m][i-1] == (int)' ')
							skasujNieodpowiednie(klucze, tablica[m][i], "male");
						else if (czyDuzaLitera(tablica[m][i - 1]) || czyMalaLitera(tablica[m][i-1]))
							skasujNieodpowiednie(klucze, tablica[m][i], "male");
					}
					++m;
				}
				if (klucze.size() == 1)
				{
					wynik[indeks] = klucze.get(0);
					++indeks;
/*					System.out.println("indeks: " + i);
					System.out.println("Klucz: " + Integer.toString(klucze.get(0),2));
					for (int u = 0; u < 16; ++u)
						if (tablica[u].length > i)
							System.out.println((char) (tablica[u][i] ^ klucze.get(0))); */
				}
				else
				{
//					________________
//					|___|___|___|___|
//					|___|___|___|___|
//					|___|___|___|___|
//					|___|___|___|___|
//					|___|___|___|___|
//
					wynik[indeks] = -1;
					++indeks;
				}
				
//				System.out.println("indeks: " + i);
//				System.out.println("");
			//		} while (!czyZakonczyc(wynik));
			}
			
			for (int u = 0; u < tablica.length; ++u)
			{
//				for (int k = 0; k < klucze.size(); ++k)
					for (int l = 0; l < indeks; ++l)
					{
						if (wynik[l] >= 0 && tablica[u].length > l)
							System.out.print((char) (tablica[u][l] ^ wynik[l]));
						else if (wynik[l] == -1 && tablica[u].length > l)
							System.out.print("_");
					}
				System.out.println();
			}
			
			/*
			System.out.println("klucz: ");
			for (int i = 0; i < zakres; ++i)
				System.out.println(wynik[i]);
				*/
	}
	
	private static boolean czyZakonczyc(char[] wynik)
	{
		
		return false;
	}

	//funkcja zwracajaca wszystkie mozliwe klucze
	private static List<Integer> brute_force(int znak)
	{
		List<Integer> lista = new LinkedList<Integer>();
		for (int liczba = 0; liczba < 256/*tablica.length()*/; ++liczba)
//		for (int litera = tablica.charAt(0); litera <= tablica.charAt(tablica.length() -1); ++litera)
		{
			if (czyDopuszczony(znak ^ liczba))
				lista.add(liczba);/*(znak ^ tablica.charAt(liczba)*///);
		}
		
		return lista;
	}
	
	//funkcja zwraca powtarzajace sie klucze
	private static List<Integer> czyKluczSiePowtarza(List<Integer> klucz, int sprawdzany)
	{
		List<Integer> lista = new LinkedList<Integer>();
		for (int i = 0; i < klucz.size(); ++i)
		{
			if (czyDopuszczony(klucz.get(i) ^ sprawdzany))
				lista.add(klucz.get(i));
		}
		
		return lista;
	}
	
	private static List<Integer> skasujpowtorzenia(List<Integer> lista)
	{
		List<Integer> temp = new LinkedList<Integer>();
		temp = lista;
		for (int i = 0; i < lista.size(); ++i)
			for (int j = 0; j < lista.size(); ++j)
				if (i != j && lista.get(i) == lista.get(j))
					temp.remove(j);
		return temp;
	}
	
	private static int [][] rozdzielNaKryptogramy(String nazwaPliku) throws FileNotFoundException
	{
    	File plik = null;
    	String szyfrogram[][] = new String [dlugosc][];
    	int szyfrogramy[][] = /*null;//*/ new int [dlugosc][];
    	Scanner skaner = null;
    	
    	if (nazwaPliku.length() > 0)
    	{
    		plik = new File(nazwaPliku);
    		skaner = new Scanner(plik);
    	}
    	
    	if (plik.canRead())
    	{	
    		//pobranie wszystkich szyfrogramow
    		for (int i = 0; i < dlugosc; ++i)
    		{
    			szyfrogram[i] = skaner.nextLine().split(" ");
    		}
    		
    		for (int i = 0; i < dlugosc; ++i)
    		{
    			szyfrogramy[i] = new int[szyfrogram[i].length];
    			
    			for (int j = 0; j < szyfrogram[i].length; ++j) //przekopiowanie wartosci ze stringu do byte[][]
    			{
    				szyfrogramy[i][j] = Integer.parseInt(szyfrogram[i][j], 2);//Integer.parseInt(szyfrogram[i][j]);
    				System.out.print(szyfrogramy[i][j] + " ");
    			}
    		}
    	}
    	
    	if (skaner != null)
    		skaner.close();
    	
    	return szyfrogramy;
	}
	
	//na wejsciu otrzymuje 16 kryptogramow zaszyfrowanych jakims kluczem
	public static void main(String[] args)
	{
		int szyfrogramy[][];
		if (args.length == 1)
			try
			{
				szyfrogramy = rozdzielNaKryptogramy(args[0]);
				//zgadnijKlucz(szyfrogramy);

			} catch (FileNotFoundException e)
			{
				e.printStackTrace();
			}
	}
}
