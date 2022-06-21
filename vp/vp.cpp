#include "vp.hpp"


Vp::Vp (sc_core::sc_module_name name, int argc, char** argv):
    sc_module (name),
    hard("Hard"),
    soft ("Soft", argc, argv),
    ddr("Ddr"),
    intcon("Intcon")
{
    soft.soft_ddr_socket.bind(ddr.ddr_soft_socket);
    soft.soft_intcon_socket.bind(intcon.intcon_soft_socket);
    intcon.intcon_hard_socket.bind(hard.hard_intcon_socket);
    SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp(){

    SC_REPORT_INFO("Virtual Platform", "Destructed.");
}