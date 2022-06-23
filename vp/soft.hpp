#ifndef SOFT_HPP
#define SOFT_HPP
// test
#include "utils.hpp"
#include "hard.hpp"
#include "intcon.hpp"
#include "addr.hpp"
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class Soft : public sc_core::sc_module
{
    public:
       Soft(sc_core::sc_module_name name, int argc, char** argv);
       ~Soft();
       tlm_utils::simple_initiator_socket<Soft> soft_intcon_socket;
       tlm_utils::simple_initiator_socket<Soft> soft_dma_socket;

    protected:
        pl_t pl;
        sc_core::sc_time offset;

        void seam_carving();
        Mat createEnergyImage(Mat& image);
        vector<int> findOptimalSeam(Mat& cumulative_energy_map);
        void reduce(Mat& image, vector<int> path);
        void driver(Mat& image, int iterations);

        //hard part, will be here until the whole structure of vp is constructed
        vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize);

        void write_dma(vector<sc_uint<8>> &vect); //incomplete impl
        void write_hard(); //incomplete impl 

};




#endif