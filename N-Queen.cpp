#include <iostream>
#include <algorithm> 
#include <sstream> 
#include <map>
#include <queue> 

using namespace std;

typedef struct {
	string path;
	int cost;
} individual;

vector<individual*> population;
int boardSize;
int maxSolutions;
int initialPopulationCount = 10;

int fitnessValue(string path) {
	int fitness = (boardSize * (boardSize - 1)) / 2;
	for (int i = 0; i < boardSize; i++)
		for (int j = i + 1; j < boardSize; j++)
			if ((path[i] == path[j]) || (i - path[i] == j - path[j]) || (i + path[i] == j + path[j]))
				fitness--;
	return fitness;
}

individual* reproduce(individual* a, individual* b) {
	individual* item = new individual;
	int c = rand() % boardSize;
	item->path =
		(a->path).substr(0, c) +
		(b->path).substr(c, boardSize - c + 1)
		;
	item->cost = fitnessValue(item->path);
	return item;
}

bool compare(individual* a, individual* b) {
	return (a->cost > b->cost);
}

individual* geneticAlgorithm() {
	int rand1, rand2;
	individual* ind1, * ind2, * item;
	bool found = 0;
	while (!found) {
		vector<individual*> newPopulation;
		for (unsigned int i = 0; i < population.size(); i++) {
			sort(population.begin(), population.end(), compare);

			rand1 = rand() % population.size() % 2;
			rand2 = rand() % population.size() % 2;

			ind1 = population[rand1];
			ind2 = population[rand2];

			item = reproduce(ind1, ind2);

			if (rand() % 2 == 0)
				item->path[rand() % (boardSize) + 1] = (rand() % (boardSize) + 1) + 48;

			if (fitnessValue(item->path) == ((boardSize * (boardSize - 1)) / 2)) {
				found = 1;
				return item;
			}
			newPopulation.push_back(item);
		}
		//
		population = newPopulation;
	}
	return item;
}

int main() {
	srand(time(0));
	boardSize = 8;
	maxSolutions = 92;
	map<string, int> solutions;
	int numFound = 0;
	while (numFound != maxSolutions) {
		string tempPath = "";
		for (int i = 1; i <= boardSize; i++) {
			ostringstream ostr;
			ostr << i;
			tempPath += ostr.str();
		}

		individual* temp;
		for (int i = 0; i < initialPopulationCount; i++) {
			random_shuffle(tempPath.begin(), tempPath.end());
			temp = new individual;
			temp->path = tempPath;
			temp->cost = fitnessValue(tempPath);
			population.push_back(temp);
		}

		individual* solution = geneticAlgorithm();
		if (!solutions[solution->path]) {
			solutions[solution->path] = 1;
			cout << "Possible Solution #" << (++numFound) << ":\t" << solution->path << endl;
		}
	}
	return 0;
}
