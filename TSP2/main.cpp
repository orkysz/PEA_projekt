#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iomanip>
#include "TSP.cpp"
#include "SA.cpp"

#include <fstream>
#include <string>
using namespace std;

//Do pomiaru czasu
long long int read_QPC()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);

}

int main()
{
	srand(time(NULL));
	TSP salesman;
	SA annealing;
	int choice;
	int* result;
	while (true)
	{
		std::cout << "----PROBLEM KOMIWOJAZERA----" << endl;
		std::cout << "Wybierz opcje zaladowania grafu: " << endl;
		std::cout << "1. Losowy graf" << endl;
		std::cout << "2. Zaladuj z pliku" << endl;
		std::cout << "Wybor: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			int n;
			std::cout << "Podaj rozmiar grafu: " << endl;
			cin >> n;
			salesman.GenerateRandomGraph(n);
			break;
		case 2:
			salesman.GenerateFromFile();
			break;
		}
		while (choice != 0)
		{
			std::cout << "Wybierz co chesz wykonac: " << endl;
			std::cout << "1. Rozwiaz problem metoda symulowanego wyzarzania" << endl;
			std::cout << "2. Wypisz wczytany graf" << endl;
			std::cout << "0. Cofnij" << endl;
			std::cout << "Wybor: ";
			cin >> choice;
			switch (choice)
			{
			case 1:
				double temp_start, temp_end, temp_rate;
				cout << "Podaj temperature poczatkowa: ";
				cin >> temp_start;
				cout << "Podaj temperature koncowa: ";
				cin >> temp_end;
				cout << "Podaj wspolczynnik chlodzenia: ";
				cin >> temp_rate;
				annealing.initialize(salesman.graph, salesman.size);

				long long int frequency, start, elapsed;
				QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
				start = read_QPC();

				result = annealing.findSolution(temp_start, temp_end, temp_rate);

				elapsed = read_QPC() - start;
				cout << "Czas wykonania w ms: " << setprecision(10) << (1000.0 * elapsed) / frequency << endl;

				std::cout << "Sciezka: ";
				for (int i = 0; i < salesman.size; i++)
					std::cout << result[i] << "->";
				std::cout << result[salesman.size] << endl;
				std::cout << "Najmnieszy koszt: " << annealing.best_cost << endl;
				continue;
				break;
			case 2:
				salesman.PrintGraph();
				continue;
				break;
			}
		}
	}

	system("pause");
	return 0;
}