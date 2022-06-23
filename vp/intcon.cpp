#include "intcon.hpp"

using namespace std;
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
    sc_dt::uint64 addr = p1.get_address();
    sc_dt::uint64 taddr;

    if(addr >= int(DMA_L) && addr <= int(DMA_H))
    {
        taddr = addr & 0x000000FF;
        p1.set_address(taddr);
        intcon_dma_socket->b_transport(p1, offset);
    }
    else if(addr >= int(HARD_L) && addr <= int(HARD_H))
    {
        taddr = addr & 0x000000FF;
        p1.set_address(addr);
        intcon_hard_socket->b_transport(p1, offset);
    }else
    {
        SC_REPORT_ERROR("Interconnect", "Wrong address.");
		p1.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE );
    }


}