#ifndef DMA_HPP
#define DMA_HPP

#include "utils.hpp"
#include "soft.hpp"
#include "addr.hpp"

class Dma: public sc_core::sc_module
{
    public:
        Dma (sc_core::sc_module_name name);
        ~Dma ();

        tlm_utils::simple_target_socket<Dma> dma_soft_socket;
        tlm_utils::simple_target_socket<Dma> dma_intcon_socket; // not impl

    protected:

        void b_transport(pl_t&, sc_core::sc_time&);
        vector<sc_uint<8>> mem;
        int control;
        int saddr;
        int cnt;
        int daddr;

        
};

#endif