#include "dma.hpp"

Dma::Dma(sc_core::sc_module_name name) : sc_module(name){

    dma_intcon_socket.register_b_transport(this, &Dma::b_transport);
    SC_METHOD(dm);
    dont_initialize();
    sensitive<<dma_start;

    SC_REPORT_INFO("DMA", "Constructed.");

}

Dma::~Dma(){

    SC_REPORT_INFO("DMA", "Destructed.");
}


void Dma::dm()
{
    data.hard_start = false;
    for(int i = 0; i < rowsize; i++)
    {
        sh_transfer(saddr);
        data.hard_start = true;
        hs_transfer(saddr);

        saddr += colsize;
    }

    control++;
    to_soft->write(control);

}


void Dma::sh_transfer(const int saddr) 
{
    for(int i = 0; i < colsize; i++)
    {

        p1.set_address(saddr + i);
        p1.set_command(TLM_READ_COMMAND);
        p1.set_data_length(2);
        p1.set_data_ptr((unsigned char*)buff_read);
        p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

        dma_soft_socket->b_transport(p1, offset);
        
        pixel_dma = toShort(buff_read);

        data.last = false;
        if (i == colsize - 1)
        {
            data.last = true;
        }
        data.pixel = pixel_dma;
        wr_port -> write(data);
        data.last = false;
    }

}

void Dma::hs_transfer(const int saddr)
{
    for(int i = 0; i < colsize; i++)
    {
        data.last = false;
        if (i == colsize - 1)
        {
            data.last = true;
        }
        rd_port -> read(data);
        data.last = false;

        toUchar(buff_write, data.pixel);
        
        p1.set_address(saddr + i);
        p1.set_command(TLM_WRITE_COMMAND);
        p1.set_data_length(1);
        p1.set_data_ptr((unsigned char*)buff_write);
        p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

        dma_soft_socket->b_transport(p1, offset);
    }
}
void Dma::b_transport(pl_t &p1, sc_core::sc_time &offset){

    tlm_command cmd = p1.get_command();
    sc_dt::uint64 addr = p1.get_address();
    unsigned char *data = p1.get_data_ptr();
    unsigned int length = p1.get_data_length();

    switch(cmd)
    {
        case TLM_WRITE_COMMAND:

            switch (addr)
            {
                case DMA_CONTROL:
                   control = *((int*)data);
                   dma_start.notify();
                   p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_ROWSIZE:
                    rowsize = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_COLSIZE:
                    colsize = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_SADDR:
                    saddr = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                break;

                case DMA_DADDR:
                    daddr = *((int*)data);
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