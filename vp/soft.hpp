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
        SC_HAS_PROCESS(Soft);
        Soft(sc_core::sc_module_name name, int argc, char** argv);
        ~Soft();
        tlm_utils::simple_initiator_socket<Soft> soft_intcon_socket; // 90% impl
        tlm_utils::simple_target_socket<Soft> soft_dma_socket; // 50%, receiving data left, sending done
        
        sc_in<int> from_dma;
        sc_in<int> from_hard;
    protected:
        pl_t p1;
        sc_core::sc_time offset;

        // ********* DDR IN SOFT ****** 
        void b_transport(pl_t &p1, sc_core::sc_time &offset);
        
        void dma_interrupt();
        sc_core::sc_event dma_done;
        int dma_control;

        void hard_interrupt();
        sc_core::sc_event hard_done;
        int hard_control;

        void configuration();

        vector<unsigned short> ddr16;
        unsigned short write_pixel;
        unsigned char read_pixel[2];
        // **************************************
        int rowsize;
        int colsize;
        void seam_carving();
        Mat createEnergyImage(Mat& image);
        vector<int> findOptimalSeam(Mat& cumulative_energy_map);
        void reduce(Mat& image, vector<int> path);
        void driver(Mat& image, int iterations);

};

#endif