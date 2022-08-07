#ifndef DMA_HPP
#define DMA_HPP

#include "utils.hpp"
#include "soft.hpp"
#include "addr.hpp"

#include "hard_if.hpp"
using namespace sc_core;

class Dma: public sc_core::sc_module
{
    public:
        Dma (sc_core::sc_module_name name);
        ~Dma ();

        tlm_utils::simple_initiator_socket<Dma> dma_soft_socket; 
        tlm_utils::simple_target_socket<Dma> dma_intcon_socket; 

        sc_port<hard_write_if> wr_port;
        sc_port<hard_read_if> rd_port;       

    protected:
        sc_core::sc_time offset;
        pl_t p1;
        
        void b_transport(pl_t&, sc_core::sc_time&);
        void dm();
        void sh_transfer(const int);
        void hs_transfer(const int);
        
        int control;
        int rowsize;
        int colsize;
        
        unsigned char buff_write[2];
        unsigned char buff_read[2];
        unsigned short pixel_dma;
        Data data;
};

#endif