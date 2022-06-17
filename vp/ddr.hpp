#ifndef DDR_HPP
#define DDR_HPP

#include "utils.hpp"
#include "soft.hpp"
#include "ddr.hpp"
class Ddr : public sc_core::sc_module
{
    public:
        Ddr (sc_core::sc_module_name name);
        ~Ddr ();

        tlm_utils::simple_target_socket<Ddr> ddr_soft_socket;

    protected:

        void b_transport(pl_t&, sc_core::sc_time&);
        vector<sc_uint<8>> mem;


};

#endif