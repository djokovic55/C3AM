#ifndef INTCON_HPP_
#define INTCON_HPP_

#include "utils.hpp"
#include "addr.hpp"
class Intcon : public sc_core::sc_module{

    public:
        Intcon(sc_core::sc_module_name name);
        ~Intcon();

        tlm_utils::simple_target_socket<Intcon> intcon_soft_socket;
        
        tlm_utils::simple_initiator_socket<Intcon> intcon_dma_socket;
        tlm_utils::simple_initiator_socket<Intcon> intcon_hard_socket;

    protected:

        sc_core::sc_time offset;
        void b_transport(pl_t &p1, sc_core::sc_time &offset);        

};

#endif