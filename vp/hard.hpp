#ifndef HARD_HPP
#define HARD_HPP


#include "utils.hpp"
#include "soft.hpp"


vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize);


#endif