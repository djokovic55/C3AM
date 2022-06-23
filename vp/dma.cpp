#include "dma.hpp"

Dma::Dma(sc_core::sc_module_name name) : sc_module(name){

    dma_soft_socket.register_b_transport(this, &Dma::b_transport);
    dma_intcon_socket.register_b_transport(this, &Dma::b_transport);
    mem.reserve(1); // should be changed

    SC_REPORT_INFO("DMA", "Constructed.");

}

Dma::~Dma(){

    SC_REPORT_INFO("DMA", "Destructed.");
}

void Dma::b_transport(pl_t &p1, sc_core::sc_time &offset){

    tlm_command cmd = p1.get_command();
    int addr = p1.get_address();
    unsigned char *data = p1.get_data_ptr();
    unsigned int length = p1.get_data_length();


    // int x;
    // int y;

    // int *a;
    

    // a = &x;

    // *a = y;

    // int *(&x)


    switch(cmd)
    {
        case TLM_WRITE_COMMAND:

            switch(int(addr))
            {
                case int(DMA_CONTROL):
                   control = *((int*)data);
                   p1.set_response_status(TLM_OK_RESPONSE);
                   cout<<control;
                break;

                case int(DMA_SOURCE_ADD):
                break;

                case int(DMA_DEST_ADD):
                break;

                case int(DMA_COUNT):
                break;

                default:
                break;

            }
        break;

        case TLM_READ_COMMAND:
        break;

        default:
        break;

    }

}