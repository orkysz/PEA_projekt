#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TSP
{
private:
	//Alokowanie pamieci
	void Initialize()
	{
		graph = new int *[size];
		for (int i = 0; i < size; i++)
			graph[i] = new int[size];
	}

public:
	int size = 0;	//rozmiar problamu
	int **graph = NULL;	//Macierzowa reprezentacja grafu

	//Generowanie losowego grafu
	void GenerateRandomGraph(int n)
	{
		size = n;
		Initialize();
		//srand(time(NULL));
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (i == j)
					graph[i][j] = -1;
				else
					graph[i][j] = rand() % 10 + 1;
			}
	}

	//Wypisywanie grafu w postaci macierzy
	void PrintGraph()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				cout << graph[i][j] << " ";
			cout << endl;
		}
	}

	//Generowanie grafu z pliku
	void GenerateFromFile()
	{
		string rozmiar;
		ifstream file;
		string file_name;
		cout << "Podaj nazwe pliku: ";
		cin >> file_name;
		file.open(file_name);
		if (file.good())
		{
			string line;
			for (int i = 0; i < 4; i++)
				getline(file, line);
			size = stoi(line.substr(11));
			Initialize();
			for (int i = 0; i < 3; i++)
				getline(file, line);
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					file >> graph[i][j];
			file.close();
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					if (i == j) graph[i][j] = -1;
		}
		else
			cout << "Nie udalo sie otworzyc pliku!";
	}

	//Zwalnianie pamieci
	~TSP()
	{
		if (graph != NULL)
		{
			for (int i = 0; i < size; i++)
				delete[] graph[i];
			delete[]graph;
		}
	}
};