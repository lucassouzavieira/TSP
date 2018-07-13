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

#ifndef TSP_CHROMOSOME_H
#define TSP_CHROMOSOME_H

#include <vector>
#include <cstdio>
#include <functional>
#include <random>
#include <chrono>
#include <algorithm>

namespace tsp {

    class Chromosome {
    public:
        Chromosome();

        Chromosome(tsp::Region &region);

        double objectiveFunctionValue();

        void print();

        void mutation(double probability);

        void shuffle();

        static Chromosome randomChromossome(unsigned int size);

        Chromosome crossover(const Chromosome &chromosome, unsigned int pcross);

        unsigned long size();

    private:
        Region region;
        std::vector<int> values;

        void mutation(std::function<void(double)> mutation);
    };

    Chromosome::Chromosome() {
        this->values = std::vector<int>();
    }

    Chromosome::Chromosome(tsp::Region &region) {
        this->values = std::vector<int>();
        uint size = region.countCities();

        for (uint v = 1; v <= size; v++) {
            this->values.push_back(v);
        }

        this->region = region;
    }

    double Chromosome::objectiveFunctionValue() {
        double result = 0.0;
        auto size = this->values.size();

        for (auto v = 0; v < size - 1; ++v) {
            result += region.getDistance(static_cast<uint32_t>(this->values.at(static_cast<unsigned long>(v))),
                                         static_cast<uint32_t>(this->values.at(static_cast<unsigned long>(v + 1))));
        }

        return result;
    }

    void Chromosome::print() {
        for (auto v : this->values) {
            std::cout << v << "*";
        }

        std::cout << "- Obj. Function: " << this->objectiveFunctionValue() << std::endl;
    }

    void Chromosome::mutation(double probability) {
        auto cpy = this;
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        generator.seed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));

        auto size = this->values.size();

        for (auto v = 0; v < size - 1; ++v) {
            auto value = distribution(generator);

            // Uma unica mutacao aleatoria
            if (value <= probability) {
                auto indexChange = static_cast<unsigned int>(distribution(generator) * 100) % size;
                auto aux = this->values.at(indexChange);
                cpy->values[v] = aux;

                this->values = cpy->values;

                break;
            }
        }
    }

    Chromosome Chromosome::crossover(const Chromosome &chromosome, unsigned int pcross) {
        Chromosome child = Chromosome();

        auto size = this->values.size();
        child.values.resize(size);

        std::fill(child.values.begin(), child.values.end(), 0);

        int indexC = 0;
        int indexP = 0;

        for (indexP = 0; indexP < pcross; ++indexP) {
            child.values[indexC] = this->values[indexP];
            ++indexC;
        }

        while (indexC < size) {
            auto value = chromosome.values[indexP % size];
            auto it = std::find(child.values.begin(), child.values.end(), value);

            if (it == child.values.end()) {
                child.values[indexC] = value;
                ++indexC;
            }

            ++indexP;
        }

        return child;
    }

    void Chromosome::shuffle() {
        std::random_shuffle(this->values.begin(), this->values.end());
    }

    Chromosome Chromosome::randomChromossome(unsigned int size) {
        std::vector<int> values = std::vector<int>();
        values.resize(size);

        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));

        Chromosome generated = Chromosome();
        generated.values = values;
        return generated;
    }

    unsigned long Chromosome::size() {
        return this->values.size();
    }
}

#endif //TSP_CHROMOSOME_H
