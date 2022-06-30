#include "dma.hpp"

Dma::Dma(sc_core::sc_module_name name) : sc_module(name){

    dma_intcon_socket.register_b_transport(this, &Dma::b_transport);

    SC_REPORT_INFO("DMA", "Constructed.");

}

Dma::~Dma(){

    SC_REPORT_INFO("DMA", "Destructed.");
}

void Dma::dm()
{

    vector<unsigned char> buff8(cnt);
    vector<unsigned short int> buff16(cnt);

    vector<sc_uint<8>> sc_buff8;
    vector<sc_uint<16>> sc_buff16;
    pl_t p1;

    switch(daddr)
    {
        case HARD_L:
            p1.set_address(saddr);
            p1.set_command(TLM_READ_COMMAND);
            p1.set_data_length(cnt);
            p1.set_data_ptr((unsigned char*)buff8.data());
            p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

            dma_soft_socket->b_transport(p1, offset);

            sc_buff8.assign(buff8.begin(), buff8.end());



            // posalji buff8 preko fifo i postavi control na 0

            // print_1d(buff8);

            control = 0;


        break;

        case DDR_L:
            // procitaj iz fifo i upisi u buff16

            p1.set_address(saddr);
            p1.set_command(TLM_WRITE_COMMAND);
            p1.set_data_length(cnt);
            p1.set_data_ptr((unsigned char*)buff16.data());
            p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

            dma_soft_socket->b_transport(p1, offset);

            //postavi control na 0


        break;

        default:
        break;
    }
}

void Dma::b_transport(pl_t &p1, sc_core::sc_time &offset){

    tlm_command cmd = p1.get_command();
    sc_dt::uint64 addr = p1.get_address();
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

            switch (addr)
            {
                case DMA_CONTROL:
                   control = *((int*)data);
                   dm();
                   p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_SOURCE_ADD:
                    saddr = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_DEST_ADD:
                    daddr = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_COUNT:
                    cnt = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                default:
                    p1.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
                break;

            }
        break;

        case TLM_READ_COMMAND:
            switch (addr)
            {
                case DMA_CONTROL:
                   memcpy(data, &control, sizeof(control));
                   p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_SOURCE_ADD:
                   memcpy(data, &saddr, sizeof(saddr));
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_DEST_ADD:
                   memcpy(data, &daddr, sizeof(daddr));
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_COUNT:
                   memcpy(data, &cnt, sizeof(cnt));
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                default:
                    p1.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
                break;

            }
        break;

        default:
            p1.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
        break;

    }

}