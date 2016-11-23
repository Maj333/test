// Metoda Gaussa-Seidela
// www.algorytm.org

#include <iostream>
using namespace std;
#include <math.h>

#define eps 0.000001
#define iteracje 200

int main()
{
	int n, i, j, k, stop;
	float dzielnik;
	float norma1, norma2;
	float **A, **U, **L, **D, **I, *B, *B2, *x, *x2;
	bool koniec;

	// rozmiar macierzy
	cout << "Podaj rozmiar macierzy: ";
	cin >> n; 
	
	// macierz glowna
	A = new float * [n];
	// macierz dolnotrojkatna
	L = new float * [n];
	// macierz gornotrojkatna
	U = new float * [n];
	// przekatna
	D = new float * [n];
	// macierz jednostkowa
	I = new float * [n];

	// tworzenie tablicy tablic - macierzy kwadratowych
	for (i = 0; i < n; i++)
	{
		A[i] = new float[n];
		L[i] = new float[n];
		U[i] = new float[n];
		D[i] = new float[n];
		I[i] = new float[n];
	}

	// stworzenie wektora wyrazow wonych i wektora wyników
	B = new float [n];
	B2 = new float [n];
	x = new float [n];
	x2 = new float [n];
	// wczytanie elementow rownan i wyzerowanie pozostalych macierzy
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			cout << "Podaj element macierzy[" << i << "][" << j << "]: ";
			cin >> A[i][j];
			L[i][j] = 0;
			U[i][j] = 0;
			D[i][j] = 0;
			I[i][j] = 0;
		}

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			// utworzenie macierzy dolnotrojkatnej
			if (i > j) 
				L[i][j] = A[i][j];
			// utworzenie macierzy gornotrojkatnej
			else if (i < j) 
				U[i][j] = A[i][j];
			else
			{
				// wczytanie wyrazow wolnych
				cout << "Podaj " << i << " wyraz wolny: ";
				cin >> B[i];
				// utworzenie macierzy z przekatnej macierzy glownej
				D[i][i] = A[i][i];
				// jedynki na przekatnej
				I[i][i] = 1;
			}
		}
	}

	// wyznaczenie p = min{||-(L+D)^-1 * N||:||-(L+D)^-1 * N||}

	// obliczenie D + L (wynik w L)
	for (i = 0; i < n; i++)
		L[i][i] = D[i][i];
			// obliczenie (D + L)^-1 (wynik w I)
	for (i = 0; i < n; i++)
	{
		dzielnik = L[i][i];
		for (j = 0; j < n; j++)
		{
			L[i][j] /= dzielnik;
			I[i][j] /= dzielnik;
		}
		for (k = 0; k < n; k++)
		{
			if (k == i) continue;
			dzielnik = L[k][i];
			for (j = 0; j < n; j++)
			{
				L[k][j] -= (L[i][j]*dzielnik);
				I[k][j] -= (I[i][j]*dzielnik);
			}
		}
	}
	// obliczenie (L + D)^-1 * N (wynik w L)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			L[i][j] = 0;
			for (k = 0; k < n; k++)
				L[i][j] += (I[i][k]*U[k][j]);
		}
	}

	// obliczenie -(L+D)^-1 * N
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			L[i][j] *= (-1);
	// norma "jeden"
	norma1 = 0; 
	for (i = 0; i < n; i++)
	{
		dzielnik = 0;
		for (j = 0; j < n; j++)
			dzielnik += fabs(L[j][i]);
		if (dzielnik > norma1) norma1 = dzielnik;
	}
	// norma "nieskonczonosc"
	norma2 = 0;
	for (i = 0; i < n; i++)
	{
		dzielnik = 0;
		for (j = 0; j < n; j++)
			dzielnik += fabs(L[i][j]);
		if (dzielnik > norma2) norma2 = dzielnik;
	}
	
	// p = min(norma1, norma2)
	if (norma1 > norma2) norma1 = norma2;

	// ciag nie jest zbiezny do rozwiazania ukladu rownan
	if (norma1 >= 1) 
		norma1 = 0.5;

	// inicjalizacja wektora wynikow
	for (i = 0; i < n; i++)
		x[i] = 0;
	
	koniec = true;
	stop = iteracje;
	do
	{
		// przepisanie x - aktualnych wynikow do x2 - wynikow z poprzedniej iteracji
		for (i = 0; i < n; i++)
			x2[i] = x[i];
		
		// wykonanie kolejnej iteracji
		for (i = 0; i < n; i++)
		{
			B2[i] = B[i];
			
			for (int j = 0; j < i; j++)
				B2[i] -= (A[i][j]*x[j]);

			for (int j = i + 1; j < n; j++)
				B2[i] -= A[i][j] * x2[j];

			x[i] = B2[i]/A[i][i];
		}

		// sprawdzenie warunku zakonczenia: ||x(k)-x(k-1)|| <= epsilon
		for (i = 0; i < n; i++)
		{
			if (fabs(x[i] - x2[i]) > eps) { koniec = true; break; }
			else koniec = false;
		}
		stop--;
   }
   while (koniec && stop > 0);

   // wyswietlenie wyniku
   cout << "WYNIK:\n";
   for (i = 0; i < n; i++)
	   cout << x[i] << " ";
   cout << endl;

	return 0;
}