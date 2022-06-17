#include "ddr.hpp"

Ddr::Ddr(sc_core::sc_module_name name) : sc_module(name){

    ddr_soft_socket.register_b_transport(this, &Ddr::b_transport);
    mem.reserve(1); // should be changed

    SC_REPORT_INFO("DDR", "Constructed.");

}

Ddr::~Ddr(){

    SC_REPORT_INFO("DDR", "Destructed.");
}

void Ddr::b_transport(pl_t &p1, sc_core::sc_time &offset){

    p1.set_response_status(tlm::TLM_OK_RESPONSE);
}