#include "GA.cpp"
#include "TSP.cpp"
using namespace std;
int main()
{
	srand(time(NULL));
	TSP salesman;
	int choice;
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
			std::cout << "1. Rozwiaz problem metoda genetyczna" << endl;
			std::cout << "2. Wypisz wczytany graf" << endl;
			std::cout << "0. Cofnij" << endl;
			std::cout << "Wybor: ";
			cin >> choice;
			switch (choice)
			{
			case 1:
			{
				GA genetics;
				int pop_size, max_gen;
				cout << "Podaj liczebnosc populacji: ";
				cin >> pop_size;
				cout << "Podaj dopuszczalna liczbe pokolen bez poprawy: ";
				cin >> max_gen;
				cout << "Podaj szanse krzyzowania: ";
				double cross_chance;
				cin >> cross_chance;
				cout << "Podaj szanse mutacji: ";
				double mut_chance;
				cin >> mut_chance;
				cout << "Wybierz typ mutacji (1 - inversion, 2 - transposition)";
				int mut_type;
				cin >> mut_type;
				Chromosome winner = genetics.findSolution(salesman.graph, salesman.size, pop_size, cross_chance, mut_chance, max_gen, mut_type);
				cout << endl;
				winner.print();
				cout << endl << "Koszt: " << winner.cost << endl;
				continue;
				break;
			}
			case 2:
				salesman.PrintGraph();
				continue;
				break;
			}
		}
	}
}