#include "vp.hpp"


Vp::Vp (sc_core::sc_module_name name, int argc, char** argv):
    sc_module (name),
    soft ("Soft", argc, argv),
    dma("Dma"),
    hard("Hard"),
    intcon("Intcon")
{
    soft.soft_dma_socket.bind(dma.dma_soft_socket);
    soft.soft_intcon_socket.bind(intcon.intcon_soft_socket);
    intcon.intcon_hard_socket.bind(hard.hard_intcon_socket);
    intcon.intcon_dma_socket.bind(dma.dma_intcon_socket);

    dma.wr_port(hard);
    dma.rd_port(hard);

    SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp(){

    SC_REPORT_INFO("Virtual Platform", "Destructed.");
}