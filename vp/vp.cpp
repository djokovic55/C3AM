#include "vp.hpp"


Vp::Vp (sc_core::sc_module_name name, int argc, char** argv):
    sc_module (name),
    soft ("Soft", argc, argv),
    ddr("Ddr")
{
    soft.soft_ddr_socket.bind(ddr.ddr_soft_socket);
    SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp(){

    SC_REPORT_INFO("Virtual Platform", "Constructed.");
}