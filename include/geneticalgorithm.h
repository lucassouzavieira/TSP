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

#ifndef TSP_GENETICALGORITHM_H
#define TSP_GENETICALGORITHM_H

#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "chromosome.h"


namespace tsp {
    using Population = std::vector<Chromosome>;

    class TSPGeneticAlgorithm {
    private:
        double mutationProbability;
        unsigned int iterations;
        unsigned int pCross;

        Population population;
        Chromosome best;
        std::ofstream output;
        bool isChange;

        void setPopulation(const Region &region);

        void setBestCromossome();

        void createInitialPopulation();

        void mutation();

        void crossover();

        void printPopulation();

        void printBestSolution();

    public:
        TSPGeneticAlgorithm();

        TSPGeneticAlgorithm(unsigned int population, unsigned int iterations = 1000, int pCross = 10,
                            double mutationProbability = 0.5);

        void run();
    };

    void TSPGeneticAlgorithm::setBestCromossome() {
        unsigned int bestIndex = 0;
        this->isChange = false;

        auto size = this->population.size();

        for (auto index = 1; index < size; ++index) {
            if (this->population[index].objectiveFunctionValue() <
                this->population[bestIndex].objectiveFunctionValue()) {
                bestIndex = static_cast<unsigned int>(index);
            }
        }

        if (this->best.size() == 0 ||
            this->population[bestIndex].objectiveFunctionValue() < this->best.objectiveFunctionValue()) {
            this->best = this->population[bestIndex];
            this->isChange = true;
        }
    }

    void TSPGeneticAlgorithm::createInitialPopulation() {
        auto size = this->population.size();

        for (auto index = 0; index < size; ++index) {
            this->population[index] = Chromosome::randomChromossome(static_cast<unsigned int>(size));
        }

        this->setBestCromossome();
    }

    void TSPGeneticAlgorithm::mutation() {
        for (auto &chromosome: this->population) {
            chromosome.mutation(this->mutationProbability);
        }
    }

    void TSPGeneticAlgorithm::crossover() {
        std::map<int, Chromosome> childs = std::map<int, Chromosome>();

        auto size = this->population.size();

        for (auto index = 0; index < size - 1; ++index) {
            auto firstChild = std::pair<int, Chromosome>();
            auto sndChild = std::pair<int, Chromosome>();

            firstChild.first = index;
            firstChild.second = this->population[index].crossover(this->population[index + 1], this->pCross);

            childs.insert(firstChild);
            sndChild.first = index;
            sndChild.second = this->population[index + 1].crossover(this->population[index], this->pCross);

            childs.insert(sndChild);
        }

        for (auto child: childs) {
            if (child.second.objectiveFunctionValue() < this->population[child.first].objectiveFunctionValue()) {
                this->population[child.first] = child.second;
            }
        }
    }

    void TSPGeneticAlgorithm::printPopulation() {
        std::cout << "---- Population --- " << std::endl;

        for (auto chromosome: this->population) {
            chromosome.print();
        }

        std::cout << "---- End of Population --- " << std::endl;
    }

    void TSPGeneticAlgorithm::printBestSolution() {
        this->best.print();
    }

    TSPGeneticAlgorithm::TSPGeneticAlgorithm(unsigned int population,
                                             unsigned int iterations,
                                             int pCross,
                                             double mutationProbability) {
        this->population.resize(population);
        this->iterations = iterations;
        this->pCross = static_cast<unsigned int>(pCross);
        this->mutationProbability = mutationProbability;
        TSPGeneticAlgorithm();
    }

    TSPGeneticAlgorithm::TSPGeneticAlgorithm() {
        this->best = Chromosome();
        this->output.open("output.txt");
    }

    void TSPGeneticAlgorithm::run() {
        unsigned int executed = 0;
        this->createInitialPopulation();

        while (executed < this->iterations) {
            this->crossover();
            this->mutation();
            this->setBestCromossome();
            this->printBestSolution();
            executed++;
        }
    }
}

#endif //TSP_GENETICALGORITHM_H
