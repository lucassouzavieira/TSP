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

#ifndef REGION_H
#define REGION_H

#include "city.h"
#include <map>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <vector>

namespace tsp {
    class Region {
    public:
        Region();

        void clear();

        void addCity(const City &city);

        double getDistance(uint32_t firstCity, uint32_t sndCity);

        uint32_t countCities();

        void print();

        bool fromFile(std::string &filename);

    private:
        std::map<int, tsp::City> mapOfCities = std::map<int, tsp::City>();
    };

    Region::Region() {}

    void Region::clear() {
        this->mapOfCities.clear();
    }

    void Region::addCity(const City &city) {
        uint32_t id = static_cast<uint32_t>(this->mapOfCities.size() + 1);
        this->mapOfCities[id] = city;
    }

    double Region::getDistance(uint32_t firstCity, uint32_t sndCity) {
        auto itA = this->mapOfCities.find(firstCity);
        auto itB = this->mapOfCities.find(sndCity);

        if (itA == this->mapOfCities.end() || itB == this->mapOfCities.end()) {
            throw std::invalid_argument("Invalid city id");
        }

        City a = itA->second;
        City b = itB->second;

        return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
    }

    uint32_t Region::countCities() {
        return static_cast<uint32_t>(this->mapOfCities.size());
    }

    void Region::print() {
        for (auto entry: this->mapOfCities) {
            std::cout << entry.first << ": (" << entry.second.x << ", " << entry.second.y << ")" << std::endl;
        }
    }

    bool Region::fromFile(std::string &filename) {
        try {
            if (this->mapOfCities.size() > 0) {
                this->clear();
            }

            std::ifstream input(filename, std::ifstream::in);

            int id;
            double x, y;

            while (input >> id >> x >> y) {
                City city;

                city.x = x;
                city.y = y;

                this->addCity(city);
            }

            return true;
        } catch (std::exception &exception) {
            std::cout << exception.what() << std::endl;
        }

        return false;
    }
}

#endif //REGION_H
