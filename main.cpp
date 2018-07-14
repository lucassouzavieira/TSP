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

int main() {
    string file = "input.txt";
    auto map = tsp::Region();

    if (map.fromFile(file)) {
        auto solution = tsp::TSPGeneticAlgorithm(500, 1000, 10, 0.7);
        std::cout << map.countCities() << std::endl;
        solution.setPopulation(map);
        solution.run();
    }

    std::cout << "Invalid input file" << std::endl;
    std::exit(1);
}