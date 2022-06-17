#include "hard.hpp"

vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize) {

    sc_uint<16> a, b, c;
    int index_1d;
    // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
    
    // vector<sc_uint<16>> energy_image_16b = convert_from_8b_to_16b(energy_image);

    vector<sc_uint<16>> energy_image_16b (energy_image.begin(), energy_image.end());

        for (int row = 1; row < rowsize; row++) {
            for (int col = 0; col < colsize; col++) {
                index_1d = (row * colsize) + col;

                b = energy_image_16b.at(index_1d - colsize);

                if(col == 0){
                    a = b;
                    
                }else{
                    a = energy_image_16b.at(index_1d - (colsize + 1));

                }
                if(col == (colsize - 1)){
                    c=b;
                }else {
                    c = energy_image_16b.at(index_1d - (colsize - 1));
                }
                energy_image_16b.at(index_1d) = energy_image_16b.at(index_1d) + std::min(a, min(b, c));
            }
        }
    return energy_image_16b;
}
