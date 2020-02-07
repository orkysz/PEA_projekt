#include "Chromosome.cpp"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iomanip>
using namespace std;

class GA
{
public:
	int problem_size, population_size;
	int **graph;
	vector<Chromosome> population, mating_pool, elite;
	double mut_chance, cross_chance;

	Chromosome findSolution(int **graf, int rozmiar_problemu, int rozmiar_populacji, double szansa_krzyzowania, double szansa_mutacji, int max_gen, int typ_mutacji)
	{
		long long int frequency, start, elapsed;
		QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
		start = read_QPC();

		srand(time(NULL));
		initialize(graf, rozmiar_problemu, rozmiar_populacji, szansa_krzyzowania, szansa_mutacji);
		sort(population.begin(), population.end());
		Chromosome best = population[0];
		int counter = 0;
		for (int generation = 1; true; generation++)
		{
			cout << "Generacja " << generation << ", najmniejszy koszt: " << best.cost << endl;
			generateMatingPool();
			createElite(0.1);
			cross();
			mutate(typ_mutacji);
			sort(population.begin(), population.end());
			if (best.cost > population[0].cost)
			{
				best = population[0];
				counter = 0;
			}
			counter++;
			if (counter == max_gen)
				break;
		}
		elapsed = read_QPC() - start;
		cout << "Czas wykonania: " << fixed << setprecision(3) << (float)elapsed / frequency << endl;
		return population[0];
	}

	void initialize(int **graf, int rozmiar_problemu, int rozmiar_populacji, double szansa_krzyzowania, double szansa_mutacji)
	{
		graph = graf;
		problem_size = rozmiar_problemu;
		population_size = rozmiar_populacji;
		mut_chance = szansa_mutacji;
		cross_chance = szansa_krzyzowania;

		vector<int> tmp_path;
		for (int i = 1; i < problem_size; i++)
			tmp_path.push_back(i);

		for (int i = 0; i < population_size; i++)
		{
			random_shuffle(tmp_path.begin(), tmp_path.end());
			Chromosome individual(tmp_path, graph);
			population.push_back(individual);
		}
	}

	vector<int> OX(vector<int>& parent1, vector<int>& parent2, int locus1, int locus2)
	{
		vector<int> offspring;
		for (size_t i = 0; i < parent1.size(); i++)
			offspring.push_back(0);
		int index1 = locus1;
		int index2 = locus2;
		if (index2 < index1)
		{
			int tmp = index2;
			index2 = index1;
			index1 = tmp;
		}
		int iterator1 = index1;
		while (iterator1 <= index2)
		{
			offspring[iterator1] = parent1[iterator1];
			iterator1++;
		}
		if (iterator1 == parent1.size())
			iterator1 = 0;

		int iterator2 = iterator1;
		int counter = 0;
		while (counter < parent1.size() - (index2 - index1 + 1))
		{
			if (find(offspring.begin(), offspring.end(), parent2[iterator2]) == offspring.end())
			{
				offspring[iterator1] = parent2[iterator2];
				iterator1++;
				counter++;
				if (iterator1 == offspring.size())
					iterator1 = 0;
			}
			iterator2++;
			if (iterator2 == parent2.size())
				iterator2 = 0;
		}
		return offspring;
	}

	void transposition(Chromosome &parent)
	{
		int locus1 = rand() % parent.genes.size();
		int locus2;
		do
		{
			locus2 = rand() % parent.genes.size();
		} while (locus2 == locus1);

		int tmp = parent.genes[locus1];
		parent.genes[locus1] = parent.genes[locus2];
		parent.genes[locus2] = tmp;

		parent.cost = parent.getCost(graph);
		parent.fitness = parent.getFitness();
	}

	void inversion(Chromosome &parent)
	{
		int locus1 = rand() % parent.genes.size();
		int locus2;
		do
		{
			locus2 = rand() % parent.genes.size();
		} while (locus2 == locus1);
		if (locus2 < locus1)
		{
			int tmp = locus2;
			locus2 = locus1;
			locus1 = tmp;
		}
		reverse(parent.genes.begin() + locus1, parent.genes.begin() + locus2 + 1);
		parent.cost = parent.getCost(graph);
		parent.fitness = parent.getFitness();
	}

	Chromosome roulette()
	{
		double Sum = 0.0;
		for (int i = 0; i < population_size; i++)
			Sum += population[i].fitness;
		double r = ((double)rand() * Sum) / (double)RAND_MAX;
		Sum = 0.0;
		for (int i = 0; i < population_size; i++)
		{
			Sum += population[i].fitness;
			if (Sum >= r)
				return population[i];
		}
	}
	
	Chromosome tournament()
	{
		vector<Chromosome> candidates;
		vector<int> taken;
		int r;

		for (int i = 0; i < 5; i++)
		{
			r = rand() % population_size;
			if (find(taken.begin(), taken.end(), r) == taken.end())
			{
				candidates.push_back(population[r]);
				taken.push_back(r);
			}
		}
		sort(candidates.begin(), candidates.end());
		return candidates[0];
	}

	void generateMatingPool()
	{
		mating_pool.clear();
		for (int i = 0; i < population_size; i++)
		{
			Chromosome selected = roulette();
			mating_pool.push_back(selected);
		}
	}

	void createElite(double param)
	{
		elite.clear();
		int i = 0;
		while (elite.size() < param * population_size)
		{
			elite.push_back(population[i]);
			i++;
		}
	}

	void cross()
	{
		population.clear();
		int counter = 0;
		while (counter < elite.size())
		{
			population.push_back(elite[counter]);
			counter++;
		}
		while (counter < population_size - 1)
		{
			if (getProbability() < cross_chance)
			{
				int index1, index2;
				index1 = rand() % (problem_size - 1);
				do
				{
					index2 = rand() % (problem_size - 1);
				}while (index1 == index2);
				Chromosome offspring1(OX(mating_pool[counter].genes, mating_pool[counter + 1].genes, index1, index2), graph);
				population.push_back(offspring1);
				Chromosome offspring2(OX(mating_pool[counter + 1].genes, mating_pool[counter].genes, index1, index2), graph);
				population.push_back(offspring2);
				counter = counter + 2;
			}
			else
			{
				population.push_back(mating_pool[counter]);
				population.push_back(mating_pool[counter + 1]);
				counter = counter + 2;
			}
		}
	}

	void mutate(int type)
	{
		for (int i = 0; i < population_size; i++)
			if (getProbability() < mut_chance)
			{
				if (type == 1)
					inversion(population[i]);
				else
					transposition(population[i]);
			}
	}

	double getProbability()
	{
		double r = ((double)rand() / (RAND_MAX));
		return r;
	}

	int findValue(vector<int> genes, int value)
	{
		for (int i = 0; i < genes.size(); i++)
			if (genes[i] == value)
				return i;
		return NULL;
	}

	long long int read_QPC()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return((long long int)count.QuadPart);
	}
};