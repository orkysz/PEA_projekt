#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

class SA
{
private:
	int n;
	int **tab;
	int *path;

	void swap(int i, int j)
	{
		int tmp = path[i];
		path[i] = path[j];
		path[j] = tmp;
	}

	int countCost()
	{
		int cost = 0;
		for (int i = 0; i < n; i++)
			cost += tab[path[i]][path[i + 1]];
		return cost;
	}

	double getProbability(int cost1, int cost2, double T)
	{
		int delta = cost2 - cost1;
		return exp(-delta / T);
	}

	double getChance()
	{
		return rand() / (RAND_MAX);
	}

public:
	int best_cost;

	void initialize(int** graph, int size)
	{
		n = size;
		tab = graph;
		best_cost = INT_MAX;
		path = new int[n + 1];
		for (int i = 0; i < n; i++)
			path[i] = i;
		path[n] = 0;
	}

	~SA()
	{
		if (path != NULL)
			delete[] path;
	}

	int* findSolution(double T_start, double T_end, double cooling_rate)
	{
		int costA, costB, index1, index2;
		int counter = 0;
		double temperature = T_start;

		int j;
		for (int i = n - 2; i > 1; i--)
		{
			j = rand() % (i + 1) + 1;
			swap(i, j);
		}
 
		while (temperature > T_end)
		{
			costA = countCost();
			index1 = rand() % (n - 1) + 1;
			index2 = rand() % (n - 1) + 1;
			swap(index1, index2);
			costB = countCost();
			if (costB >= costA && getChance() >= getProbability(costA, costB, temperature))
				swap(index1, index2);
			temperature *= cooling_rate;
			counter++;
		}
		best_cost = costA;
		cout << "Ilosc potrzebnych iteracji: " << counter << endl;
		return path;
	}
};