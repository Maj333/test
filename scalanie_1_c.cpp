// Scalanie ciagow
// www.algorytm.org

#include <iostream>
using namespace std; 

int main()
{
	int tablica1[10] = { 1, 13, 43, 77, 88, 109, 121, 144, 165, 200 };
	int tablica2[10] = { 0, 23, 44, 77, 99, 101, 111, 150, 162, 199 }; 
	int tablica_wynikowa[20];
	int i, j, k; // zmienne pomocnicze

	// wyswietlenie wygenerowanych ciagow
	cout << "Ciag pierwszy: ";
	for (i = 0; i < 10; i++)
		cout << tablica1[i] << " ";
	cout << endl;
	cout << "Ciag drugi: ";
	for (i = 0; i < 10; i++)
		cout << tablica2[i] << " ";
	cout << endl;

	// scalanie ciagow
	for (i = j = k = 0; i < 10 && j < 10; k++)
	{
		if (tablica1[i] < tablica2[j]) // sprawdzamy, ktora liczba jest mniejsza
		{
			tablica_wynikowa[k] = tablica1[i]; // mniejsza wpisujemy do tablicy
			i++; // zwiekszamy indeks w tablicy, w ktorej byl mniejszy element
		}
		else
		{
			tablica_wynikowa[k] = tablica2[j];
			j++;
		}
	}
	// przepisanie reszty tablicy pierwszej lub drugiej w zaleznosci, od tego ktora juz przepisalismy w calosci do tablicy wynikowej
	if (i < 10)
		for ( ; i < 10; i++, k++)
			tablica_wynikowa[k] = tablica1[i];
	else
		for ( ; j < 10; j++, k++)
			tablica_wynikowa[k] = tablica2[j];

	// wypisanie tablicy wynikowej
	cout << "Ciag posortowany: ";
	for (i = 0; i < 20; i++)
		cout << tablica_wynikowa[i] << " ";
	cout << endl;

	return 0;
}