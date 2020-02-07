#include <iostream>
#include <vector>
using namespace std;

class Chromosome
{
public:
	vector<int> genes;
	int cost;
	double fitness;

	Chromosome()
	{

	}

	Chromosome(vector<int> path, int** graph)
	{
		genes = path;
		cost = getCost(graph);
		fitness = getFitness();
	}

	int getCost(int** graph)
	{
		int cost = 0;
		cost += graph[0][genes[0]];
		for (size_t i = 0; i < genes.size() - 1; i++)
			cost += graph[genes[i]][genes[i + 1]];
		cost += graph[genes[genes.size() - 1]][0];
		return cost;
	}

	double getFitness()
	{
		double fitness = 1.0 / cost;
		return fitness;
	}

	void print()
	{
		cout << "0->";
		for (size_t i = 0; i < genes.size(); i++)
			cout << genes[i] << "->";
		cout << "0";
	}

	bool operator< (const Chromosome &other) const {
		return cost < other.cost;
	}


};