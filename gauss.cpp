//Rozwiazywanie ukladu rownan liniowych metoda Gaussa
//www.algorytm.org

#include <iostream>
#include <fstream>

#include <algorithm> // swap
#include <cstddef> // size_t

using std::swap;

// a[r][c]
// r - liczba wierszy
// c - liczba kolumn
template<typename Number> void printMatrix(const Number** a, const std::size_t r, const std::size_t c) {
	if(a == nullptr) {
		return;
	}

	for(std::size_t ir = 0; ir < r; ++ir) {
		if(a[ir] != nullptr) {
			for(std::size_t ic = 0; ic < c; ++ic) {
				std::cout << a[ir][ic];
				if(ic + 1 < c) {
					std::cout << " ";
				}
			}
		}
		std::cout << std::endl;
	}
}

// x[n]
template<typename Number> void swapElements(Number* x, const std::size_t n, const std::size_t a, const std::size_t b) {
	if(x == nullptr) {
		return;
	}

	if(a >= n || b >= n) {
		return;
	}
	swap(x[a], x[b]);
}

// a = a + bx
// a[n]
// b[n]
template<typename Number> void addMultiply(Number* a, const Number* b, const std::size_t n, const Number& x) {
	if(a == nullptr || b == nullptr) {
		return;
	}

	for(std::size_t i = 0; i < n; ++i) {
		a[i] += b[i] * x;
	}
}

enum LinearEquationsSystemSolutionsCount {
	LESSC_NoSolutions = 0,
	LESSC_UniqueSolution,
	LESSC_InfinitelyManySolutions
};

enum LinearEquationsSolverException {
	LESE_NullPointer,
	LESE_MemAllocError
};

// ax = b
// a - macierz NxN gdzie piwrewszy indeks to numer wiersza/równania a drugi indeks to numer kolumny/zmiennej
// b - wektor
// x - wynik - wektor zmiennych
// a[n][n]
// b[n]
// x[n]
template<typename Number> LinearEquationsSystemSolutionsCount solveLinearEquationsSystem(
	Number** a, Number* b, Number* x, const std::size_t n) {
		std::size_t* rowsPerm = new std::size_t[n];
		std::size_t* colsPerm = new std::size_t[n];

		// SprawdŸ czy alokacja pamiêci powiod³a siê.
		if(rowsPerm == nullptr) {
			throw LESE_MemAllocError;
		}
		if(colsPerm == nullptr) {
			delete[] rowsPerm;
			throw LESE_MemAllocError;
		}

		// SprawdŸ czy nie ma zerowych wskaŸników.
		if(a == nullptr) {
			delete[] rowsPerm;
			delete[] colsPerm;
			throw LESE_NullPointer;
		}
		for(std::size_t r = 0; r < n; ++r) {
			if(a[r] == nullptr) {
				delete[] rowsPerm;
				delete[] colsPerm;
				throw LESE_NullPointer;
			}
		}

		// Wartoœci pocz¹tkowe w wektorach permutacji
		for(std::size_t i = 0; i < n; ++i) {
			rowsPerm[i] = i;
			colsPerm[i] = i;
		}

		// Eliminacja Gaussa
		for(std::size_t i = 0; i < n; ++i) {
			// ZnajdŸ niezerowy element macierzy.
			bool nonZeroFound = false;
			for(std::size_t r = i; r < n; ++r) {
				for(std::size_t c = i; c < n; ++c) {
					if(a[rowsPerm[r]][colsPerm[c]] != Number(0)) {
						swapElements<std::size_t>(rowsPerm, n, r, i);
						swapElements<std::size_t>(colsPerm, n, c, i);
						nonZeroFound = true;
						break;
					}
				}
				if(nonZeroFound) {
					break;
				}
			}
			if(!nonZeroFound) {
				// Zakoñcz eliminacjê Gaussa poniewa¿ pozosta³a czêœæ macierzy jest zerowa.
				break;
			}

			// Wyzeruj kolumnê.
			if(i + 1 < n) {
				for(std::size_t r = i + 1; r < n; ++r) {
					if(a[rowsPerm[r]][colsPerm[i]] != Number(0)) {
						Number q = -(a[rowsPerm[r]][colsPerm[i]]) / (a[rowsPerm[i]][colsPerm[i]]);
						addMultiply<Number>(a[rowsPerm[r]], a[rowsPerm[i]], n, q);
						b[rowsPerm[r]] += b[rowsPerm[i]] * q;
					}
				}
			}
		}

		// Wyznacz rozwi¹zania.
		LinearEquationsSystemSolutionsCount result;
		bool infSol = false, resultDefined = false;
		for(std::size_t i_1 = n; i_1 > 0; --i_1) {
			std::size_t i = i_1 - 1;
			Number b_s = b[rowsPerm[i]];
			for(std::size_t j = i + 1; j < n; ++j) {
				b_s -= a[rowsPerm[i]][colsPerm[j]] * x[colsPerm[j]];
			}
			const Number& a_ii = a[rowsPerm[i]][colsPerm[i]];
			if(a_ii == Number(0)) {
				if(b_s == Number(0)) {
					infSol = true;
				} else {
					result = LESSC_NoSolutions;
					resultDefined = true;
					break;
				}
			} else if(!infSol) {
				x[colsPerm[i]] = b_s / a_ii;
			}
		}

		if(!resultDefined) {
			if(infSol) {
				result = LESSC_InfinitelyManySolutions;
			} else {
				result = LESSC_UniqueSolution;
			}
		}

		delete[] rowsPerm;
		delete[] colsPerm;
		return result;
}

// wynik - tablica[r][c]
template<typename T> T** create2DArray(const std::size_t r, const std::size_t c) {
	T** tmp = new T*[r];
	if(tmp == nullptr) {
		return nullptr;
	}
	for(std::size_t i = 0; i < r; ++i) {
		tmp[i] = new T[c];
		if(tmp[i] == nullptr) {
			for(std::size_t j = 0; j < i; ++j) {
				delete[] tmp[j];
			}
			delete[] tmp;
			return nullptr;
		}
	}
	return tmp;
}

// a[r][]
template<typename T> void destroy2DArray(T** a, const std::size_t r) {
	if(a != nullptr) {
		for(std::size_t i = 0; i < r; ++i) {
			if(a[i] != nullptr) {
				delete[] a[i];
			}
		}
		delete[] a;
	}
}

// ax = b
// n - liczba równañ/zmiennych
// a_ij - element macierzy w i-tym wierszy i j-tej kolumnie
// b_i - i-ty element wektora
// Format wejœcia:
// n
// a_11 a_12 ... a_1n
// ...
// a_n1 a_n2 ... a_nn
// b_1 b_2 ... b_n
template<typename Number> bool loadEquations(std::istream& input, Number**& a, Number*& b, std::size_t& n) {
	input >> n;
	if(!input) {
		return false;
	}
	input.ignore();

	b = new Number[n];
	if(b == nullptr) {
		return false;
	}
	a = create2DArray<Number>(n, n);
	if(a == nullptr) {
		delete[] b;
		return false;
	}

	for(std::size_t r = 0; r < n; ++r) {
		for(std::size_t c = 0; c < n; ++c) {
			input >> a[r][c];
			if(!input) {
				destroy2DArray(a, n);
				delete[] b;
				return false;
			}
			input.ignore();
		}
	}
	for(std::size_t i = 0; i < n; ++i) {
		input >> b[i];
		if(!input) {
			destroy2DArray(a, n);
			delete[] b;
			return false;
		}
		input.ignore();
	}

	return true;
};

int main(int, char**) {
	// Uk³ad równan NxN
	double** a = nullptr; // Lewa strona
	double* b = nullptr; // Prawa strona

	double* x = nullptr; // Wektor zmiennych
	std::size_t n; // Liczba równañ/zmiennych

	std::ifstream input("input.txt");
	if(!input.is_open()) {
		std::cout << "Nie udalo sie otworzyc pliku." << std::endl;
		getchar();
		return 0;
	}

	if(!loadEquations(input, a, b, n)) {
		std::cout << "Nie udalo sie wczytac ukladu rownan." << std::endl;
		getchar();
		return 0;
	}

	input.close();

	x = new double[n];
	if(x == nullptr) {
		std::cout << "Wystapil blad podczas alokacji pamieci." << std::endl;
		destroy2DArray(a, n);
		delete[] b;
		getchar();
		return 0;
	}

	std::cout << "Obliczanie..." << std::endl;
	LinearEquationsSystemSolutionsCount result;
	try {
		result = solveLinearEquationsSystem(a, b, x, n);
	} catch(LinearEquationsSolverException e) {
		std::cout << "Wystapil blad: ";
		switch(e) {
		case LESE_MemAllocError:
			std::cout << "Blad alokacji pamieci.";
			break;

		case LESE_NullPointer:
			std::cout << "Zerowy wskaznik w danych wejsciowych.";
			break;

		default:
			std::cout << "Nieznany blad.";
			break;
		}

		destroy2DArray(a, n);
		delete[] b;
		delete[] x;
		getchar();
		return 0;
	} catch(...) {
		std::cout << "Wystapil nieznany blad.";

		destroy2DArray(a, n);
		delete[] b;
		delete[] x;
		getchar();
		return 0;
	}

	std::cout << "Zakonczono obliczanie." << std::endl;
	if(result == LESSC_UniqueSolution) {
		std::cout << "Jedno rozwiazanie:" << std::endl;
		printMatrix((const double**)(&x), 1, n);
	} else if(result == LESSC_InfinitelyManySolutions) {
		std::cout << "Nieskonczenie wiele rozwiazan." << std::endl;
	} else {
		std::cout << "Brak rozwiazan." << std::endl;
	}

	destroy2DArray(a, n);
	delete[] b;
	delete[] x;
	getchar();
	return 0;
}
