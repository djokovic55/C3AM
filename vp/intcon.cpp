#include "intcon.hpp"


Intcon::Intcon(sc_core::sc_module_name name) 
    : sc_module(name), offset(sc_core::SC_ZERO_TIME)
{
    intcon_soft_socket.register_b_transport(this, &Intcon::b_transport);
    SC_REPORT_INFO("Interconnect", "Consructed");
}
Intcon::~Intcon()
{
    SC_REPORT_INFO("Interconnect", "Destructed");

}

void Intcon::b_transport(pl_t &p1, sc_core::sc_time &offset)
{

    p1.set_response_status(tlm::TLM_OK_RESPONSE);
    intcon_hard_socket->b_transport(p1, offset);


}