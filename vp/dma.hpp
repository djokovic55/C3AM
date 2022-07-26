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

        tlm_utils::simple_initiator_socket<Dma> dma_soft_socket; //50%, transfer HARD TO DDR LEFT

        tlm_utils::simple_target_socket<Dma> dma_intcon_socket; // 90% impl


        sc_port<hard_write_if> wr_port;
        sc_port<hard_read_if> rd_port;       
    protected:
        sc_core::sc_time offset;
        pl_t p1;
        
        void b_transport(pl_t&, sc_core::sc_time&);
        void dm();
        void sh_transfer(vector<unsigned short>& buff16, int& saddr);
        void hs_transfer(vector<unsigned short>& buff16, int& saddr);
        
        int control;
        int rowsize;
        int colsize;

        unsigned char buff_write[2];
        unsigned char buff_read[2];
        unsigned short out_dma;
        Data input;
};

#endif