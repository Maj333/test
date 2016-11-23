/*
Copyright (c) 2011, 2012 Mateusz Kacprzak <mateusz.kacprzak@yandex.ru>
www.algorytm.org

Niniejszy program jest wolnym oprogramowaniem; można go rozprowadzać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU,
wydanej przez Fundację Wolnego Oprogramowania (Free Software Foundation) - według wersji trzeciej tej Licencji lub którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej gwarancji
PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. W celu uzyskania bliższych informacji należy zapoznać się z Powszechną Licencją Publiczną GNU.
*/
/hjkhjkh**---------------------------------------------------------------------//
// Implementacja algorytmu szyfrowania metoda Vigenere'a                //
// Program obsluguje szyfrowanie i deszyfrowanie dla zadanego alfabetu, //
// czyli zarowno tekst jak i klucz powinny zawierac sie w alfabecie     //
// w przeciwnym wypadku tekst wyjsciowy moze zawierac niepozadane znaki	//
// *alfabet - przedzial (nie zbior)                                     //
// W przypadku problemow prosze kontaktowac sie z autorem mailowo       //
//----------------------------------------------------------------------*/
#include<cstdio>
#include<cstring>
#define ENCODEDFILE "zakodowane.txt" // nazwa pliku wyjsciowego dla zakodowanej wiadomosci
#define DECODEDFILE "odkodowane.txt" // nazwa pliku wyjsciowego dla odkodowanej wiadomosci
#define KEYLENGTH 256 // dlugosc klucza wyrazona w ilosci liter
#define FILENAMELENGTH 256 // dlugosc nazwy pliku wyrazona w ilosci liter
#define FIRSTLETTER 'A' // pierwsza litera alfabetu
#define LASTLETTER 'Z' // ostatnia litera alfabetu
using namespace std;

void ManualMode(FILE* pFile, FILE* oFile); // tryb uruchamiany domyslnie
void AutomaticMode(FILE* pFile, FILE* oFile, char** argv); // tryb uruchamiany z parametrami startowymi
void PrintError(char** argv); // komunikat pomocniczy
void CipherEngine(bool encoding, FILE* pFile, FILE* oFile, char* cipherKey); // silnik kodujacy

int main(int argc, char** argv)
{
    FILE* pFile;
    FILE* oFile;
    switch(argc)
    {
        case 1: ManualMode(pFile, oFile); break;
        case 4: AutomaticMode(pFile, oFile, argv); break;
        default: PrintError(argv);
    }
    return 0;
}

void ManualMode(FILE* pFile, FILE* oFile)
{
    char filename[FILENAMELENGTH+1];
    char cipherKey[KEYLENGTH+1];
	char option = '0';
	bool encoding;
	while(option != 'e' && option != 'd')
	{
		printf("Podaj opcje: kodowanie - \'e\', dekodowanie - \'d\' (\'q\' aby zakonczyc program).\n");
		scanf("%c", &option);
		if(option == 'q')
			return;
	}
	option == 'e' ? encoding = true : encoding = false;
    while(!pFile)
    {
        printf("Podaj nazwe pliku wejsciowego (bez spacji).\n");
        scanf("%s", filename, FILENAMELENGTH);
        if(filename[0] == 'q' && strlen(filename) == 1)
            return;
        pFile = fopen(filename, "r");
        if(!pFile)
            printf("Nie powiodlo sie otworzenie pliku: %s. Sprobuj ponownie.\n", filename);
    }
    printf("Podaj klucz szyfrujacy (nie dluzszy niz %d znakow)\n", KEYLENGTH);
    scanf("%s", cipherKey, KEYLENGTH);
    CipherEngine(encoding, pFile, oFile, cipherKey);
}

void AutomaticMode(FILE* pFile, FILE* oFile, char** argv)
{
    bool encoding;
    pFile = fopen(argv[2], "r");
    if(!pFile)
    {
        printf("Nie powiodlo sie otworzenie pliku: %s.\n", argv[2]);
        return;
    }
    if(strlen(argv[1]) != 1 || ((*(argv[1]) != 'd') && *(argv[1]) != 'e'))
	{
        PrintError(argv);
		return;
	}
    *(argv[1]) == 'e' ? encoding = true : encoding = false;
    CipherEngine(encoding, pFile, oFile, argv[3]);
}

void PrintError(char** argv)
{
    printf("Program \'%s\' zostal uruchomiony z nieprawidlowym/brakujacym parametrem.\n", argv[0]);
    printf("Nalezy podac tylko trzy parametry (bez nawiasow):\nopcja, nazwa pliku wejsciowego i klucz szyfrujacy.\n");
    printf("np.: \'%s [ed] [tekst_jawny.txt] [HASLO]\'.\n", argv[0]);
    printf("Alternatywnie program moze zostac uruchomiony bez zadnego parametru.\n");
    printf("Nacisnij \'Enter\', aby zakonczyc dzialanie programu");
    getchar();
}

void CipherEngine(bool encoding, FILE* pFile, FILE* oFile, char* cipherKey)
{
    unsigned int charCounter = 0; // licznik znakow wejsciowych
	unsigned short int keyLength = strlen(cipherKey); // dlugosc klucza szyfrujacego
    char inputChar; // litera tekstu wejsciowego do szyfrowania
	char outChar; // litera tekstu wyjsciowego
    const char* OUTPUTFILE;
    encoding ? OUTPUTFILE = ENCODEDFILE : OUTPUTFILE = DECODEDFILE;
    oFile = fopen(OUTPUTFILE, "w");
    if(!oFile)
    {
        printf("Nie powiodlo sie utworzenie pliku wyjsciowego.\n");
        return;
    }
    while((inputChar = fgetc(pFile)) != EOF)
    {
        if(encoding)
		{
		    // do litery wejscia dodajemy wyznaczona litere klucza i od wszystkiego odejmujemy pierwsza litere alfabetu
			outChar = inputChar + cipherKey[charCounter%keyLength] - FIRSTLETTER;
			// jezeli wyjdzie poza alfabet, to przeliczamy od poczatku alfabetu
			outChar > LASTLETTER	? fprintf(oFile, "%c", FIRSTLETTER + outChar - LASTLETTER - 1)
									: fprintf(oFile, "%c", outChar);
		}
		else
		{
		    // od litery wejscia odejmujemy wyznaczona litere klucza i do wszystkiego dodajemy pierwsza litere alfabetu
			char outChar = inputChar - cipherKey[charCounter%keyLength] + FIRSTLETTER;
			// jezeli wyjdzie przed alfabet, to przeliczamy od konca alfabetu
			outChar < FIRSTLETTER	? fprintf(oFile, "%c", LASTLETTER + outChar - FIRSTLETTER + 1)
									: fprintf(oFile, "%c", outChar);
		}
        charCounter++;
    }
    fclose(pFile); // obviously closing the file
    fclose(oFile);
    printf("Zapisano w pliku %s.", OUTPUTFILE);
}
