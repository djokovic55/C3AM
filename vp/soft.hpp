#ifndef SOFT_HPP
#define SOFT_HPP

#include "utils.hpp"
#include "hard.hpp"


Mat createEnergyImage(Mat& image);

vector<int> findOptimalSeam(Mat& cumulative_energy_map);

void reduce(Mat& image, vector<int> path);

void driver(Mat& image, int iterations);

#endif