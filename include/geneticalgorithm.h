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

#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "chromosome.h"


namespace tsp {
    using Population = std::vector<Chromosome>;
    using Hibrid = std::function<Population(Population &population, Chromosome best, double mutationProbability)>;

    class TSPGeneticAlgorithm {
    private:
        double mutationProbability;
        unsigned int iterations;
        unsigned int pCross;

        Population population;
        Chromosome best;
        std::ofstream output;
        Hibrid hibridization;

        bool isChange;
        bool hasPopulation;
        bool hasCustomAlgorithm;


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

        void setPopulation(Region &region);

        void setHibridization(Hibrid callback);

        void run();
    };

    void TSPGeneticAlgorithm::setPopulation(Region &region) {
        auto size = this->population.size();

        for (auto index = 0; index < size; ++index) {
            this->population[index] = Chromosome(region);
            this->population[index].shuffle();
        }

        this->hasPopulation = true;
        this->setBestCromossome();
    }

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

        auto candidate = this->population[bestIndex];

        if (this->best.size() == 0 ||
            candidate.objectiveFunctionValue() < this->best.objectiveFunctionValue() &&
            candidate.objectiveFunctionValue() != 0) {
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
        if (this->hasCustomAlgorithm) {
            this->population = this->hibridization(this->population, this->best, this->mutationProbability);
            return;
        }

        // Algoritmo genetico default
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
        this->hasCustomAlgorithm = false;
        TSPGeneticAlgorithm();
    }

    TSPGeneticAlgorithm::TSPGeneticAlgorithm() {
        this->best = Chromosome();
        this->output.open("output.txt");
    }

    void TSPGeneticAlgorithm::run() {
        unsigned int executed = 0;

        if (!this->hasPopulation) {
            this->createInitialPopulation();
        }

        while (executed < this->iterations) {
            this->mutation();
            this->setBestCromossome();
            this->printBestSolution();
            executed++;
            std::cout << executed << std::endl;
        }
    }

    void TSPGeneticAlgorithm::setHibridization(Hibrid hibrid) {
        this->hibridization = hibrid;
        this->hasCustomAlgorithm = true;
    }
}

#endif //TSP_GENETICALGORITHM_H
