#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

// Función de fitness: ejemplo con una función cuadrática simple
double fitness_function(const vector<double>& genome) {
    double fitness = 0.0;
    for (double gene : genome) {
        fitness -= gene * gene;
    }
    return fitness;
}

// Crear una población inicial de individuos aleatorios de números flotantes
vector<vector<double>> create_population(double GENE_MIN, double GENE_MAX, int GENE_LENGTH, int POP_SIZE) {
    vector<vector<double>> population(POP_SIZE, vector<double>(GENE_LENGTH));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(GENE_MIN, GENE_MAX);

    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < GENE_LENGTH; ++j) {
            population[i][j] = dis(gen);
        }
    }
    return population;
}

// Selección por torneo
vector<vector<double>> tournament_selection(const vector<vector<double>>& population, const vector<double>& fitnesses, int POP_SIZE) {
    int tournament_size = 3;
    vector<vector<double>> selected;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, POP_SIZE - 1);

    for (int i = 0; i < POP_SIZE; ++i) {
        vector<int> tournament;
        for (int j = 0; j < tournament_size; ++j) {
            tournament.push_back(dis(gen));
        }

        int winner = tournament[0];
        for (int idx : tournament) {
            if (fitnesses[idx] > fitnesses[winner]) {
                winner = idx;
            }
        }
        selected.push_back(population[winner]);
    }
    return selected;
}

// Cruce de un punto
vector<vector<double>> crossover(const vector<double>& parent1, const vector<double>& parent2, double CROSSOVER_RATE, int GENE_LENGTH) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_int_distribution<> dis_int(1, GENE_LENGTH - 1);

    if (dis(gen) < CROSSOVER_RATE) {
        int crossover_point = dis_int(gen);
        vector<double> child1(parent1.begin(), parent1.begin() + crossover_point);
        child1.insert(child1.end(), parent2.begin() + crossover_point, parent2.end());

        vector<double> child2(parent2.begin(), parent2.begin() + crossover_point);
        child2.insert(child2.end(), parent1.begin() + crossover_point, parent1.end());

        return {child1, child2};
    } else {
        return {parent1, parent2};
    }
}

// Mutación
vector<double> mutate(vector<double>& genome, int GENE_LENGTH, double MUTATION_RATE, double GENE_MIN, double GENE_MAX) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_real_distribution<> dis_gene(GENE_MIN, GENE_MAX);

    for (int i = 0; i < GENE_LENGTH; ++i) {
        if (dis(gen) < MUTATION_RATE) {
            genome[i] = dis_gene(gen);
        }
    }
    return genome;
}

// Algoritmo Genético
void genetic_algorithm() {
    // Parámetros del algoritmo genético
    int NUM_GENERATIONS = 100;  // Número de generaciones
    int POP_SIZE = 50;  // Tamaño de la población
    int GENE_LENGTH = 10;  // Longitud del genoma (número de genes)
    double MUTATION_RATE = 0.01;  // Tasa de mutación
    double CROSSOVER_RATE = 0.7;  // Tasa de cruce
    double GENE_MIN = -5.0;  // Valor mínimo de un gen
    double GENE_MAX = 5.0;  // Valor máximo de un gen

    auto population = create_population(GENE_MIN, GENE_MAX, GENE_LENGTH, POP_SIZE);
    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        // Evaluar la aptitud de cada individuo
        vector<double> fitnesses(POP_SIZE);
        for (int i = 0; i < POP_SIZE; ++i) {
            fitnesses[i] = fitness_function(population[i]);
        }

        // Imprimir el mejor individuo de cada generación
        auto best_fitness = *max_element(fitnesses.begin(), fitnesses.end());
        auto best_individual = population[distance(fitnesses.begin(), max_element(fitnesses.begin(), fitnesses.end()))];
        cout << "Generation " << generation << ": Best Fitness = " << best_fitness << " | Best Individual = ";
        for (double gene : best_individual) {
            cout << gene << " ";
        }
        cout << endl;

        // Selección
        auto selected_population = tournament_selection(population, fitnesses, POP_SIZE);

        // Cruce y creación de la nueva generación
        vector<vector<double>> next_generation;
        for (int i = 0; i < POP_SIZE; i += 2) {
            auto children = crossover(selected_population[i], selected_population[i + 1], CROSSOVER_RATE, GENE_LENGTH);
            next_generation.push_back(children[0]);
            next_generation.push_back(children[1]);
        }

        // Mutación
        for (auto& individual : next_generation) {
            mutate(individual, GENE_LENGTH, MUTATION_RATE, GENE_MIN, GENE_MAX);
        }

        population = next_generation;
    }

    // Resultado final
    vector<double> fitnesses(POP_SIZE);
    for (int i = 0; i < POP_SIZE; ++i) {
        fitnesses[i] = fitness_function(population[i]);
    }
    auto best_fitness = *max_element(fitnesses.begin(), fitnesses.end());
    auto best_individual = population[distance(fitnesses.begin(), max_element(fitnesses.begin(), fitnesses.end()))];
    cout << "\nFinal Best Fitness = " << best_fitness << " | Best Individual = ";
    for (double gene : best_individual) {
        cout << gene << " ";
    }
    cout << endl;
}

int main() {
    genetic_algorithm();
    return 0;
}
