/*
 * Copyright (C) 2018  Lucas S. Vieira
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <region.h>
#include <chromosome.h>
#include <geneticalgorithm.h>

using namespace std;
using namespace tsp;

int main() {
    string file = "input.txt";
    auto map = tsp::Region();

    // Hibridizacao - as mutacoes passam a se basear no melhor Cromossomo / funcao-objetivo da iteracao anterior
    auto callback = [](Population &population, unsigned int best, double mutationProbability) -> Population {
        auto size = population.size();
        auto bestChromosome = population[best];

        /**
         * Busca por individuos proximos ao melhor atual
         * Variacoes de 5% no valor da FO sao consideradas boas o suficiente
         */
        double nVariation = bestChromosome.objectiveFunctionValue() - bestChromosome.objectiveFunctionValue() * 0.05;
        double pVariation = bestChromosome.objectiveFunctionValue() + bestChromosome.objectiveFunctionValue() * 0.05;

        for (unsigned int i = (best - 5); i <= (best + 5); i++) {
            if (population[i].objectiveFunctionValue() <= pVariation &&
                population[i].objectiveFunctionValue() >= nVariation) {
                population.push_back(population[i].clone());
            }
        }

        for (auto &chromosome: population) {
            chromosome.mutation(mutationProbability);
        }

        return population;
    };

    // Algoritmo genetico puro
    if (map.fromFile(file)) {
        auto solution = tsp::TSPGeneticAlgorithm(500, 500, 10, 0.7);
        solution.setPopulation(map);
        solution.run();
    }

    std::cout << "Híbrido" << std::endl;

    // Híbrido
    if (map.fromFile(file)) {
        auto solution = tsp::TSPGeneticAlgorithm(500, 500, 10, 0.7);
        solution.setPopulation(map);
        solution.setHibridization(callback);
        solution.run();
    }

    std::exit(0);
}