#ifndef HARD_HPP
#define HARD_HPP


#include "utils.hpp"

class Hard : public sc_core::sc_module
{
    public:
        Hard(sc_core::sc_module_name name);
        ~Hard();

        tlm_utils::simple_target_socket<Hard> hard_intcon_socket;
        
        // vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize);
    protected:
        pl_t p1;
        sc_core::sc_time offset;
        void b_transport(pl_t&, sc_core::sc_time&);

};





#endif