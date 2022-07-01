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
       tlm_utils::simple_initiator_socket<Soft> soft_intcon_socket; // 90% impl
       tlm_utils::simple_target_socket<Soft> soft_dma_socket; // 50%, receiving data left, sending done

    protected:
        pl_t pl;
        sc_core::sc_time offset;

        // ********* DDR IN SOFT ****** 
        void b_transport(pl_t &p1, sc_core::sc_time &offset);

        vector<unsigned char> ddr8;
        vector<unsigned short int> ddr16;

        vector<sc_uint<16>> sc_ddr16;

        int ite = 0;

        unsigned short out;
        // **************************************
        int rowsize;
        int colsize;
        void seam_carving();
        Mat createEnergyImage(Mat& image);
        vector<int> findOptimalSeam(Mat& cumulative_energy_map);
        void reduce(Mat& image, vector<int> path);
        void driver(Mat& image, int iterations);

        //hard part, will be here until the whole structure of vp is constructed
        vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize);

};




#endif