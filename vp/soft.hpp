#ifndef SOFT_HPP
#define SOFT_HPP
// test
#include "utils.hpp"
#include "hard.hpp"
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class Soft : public sc_core::sc_module
{
    public:
       Soft(sc_core::sc_module_name name, int argc, char** argv);
       ~Soft();
       tlm_utils::simple_initiator_socket<Soft> interconnect_socket;
       tlm_utils::simple_initiator_socket<Soft> mem_socket;



    protected:
        pl_t pl;
        sc_core::sc_time offset;

        void seam_carving();
        Mat createEnergyImage(Mat& image);
        vector<int> findOptimalSeam(Mat& cumulative_energy_map);
        void reduce(Mat& image, vector<int> path);
        void driver(Mat& image, int iterations);


};




#endif