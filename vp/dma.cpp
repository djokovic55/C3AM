#include "dma.hpp"

Dma::Dma(sc_core::sc_module_name name) : sc_module(name){

    dma_intcon_socket.register_b_transport(this, &Dma::b_transport);

    // SC_REPORT_INFO("DMA", "Constructed.");

}

Dma::~Dma(){

    SC_REPORT_INFO("DMA", "Destructed.");
}


void Dma::dm()
{
    vector<unsigned short> buff16(colsize);
    int saddr = colsize; 

    cout<<"---------------------------> Number of pixels: "<<rowsize*colsize<<endl;
    for(int j = 0; j < rowsize - 1; j++)
    {

        sh_transfer(buff16, saddr);
        hs_transfer(buff16, saddr);

        saddr += colsize;
    }
    control = 0;

}


void Dma::sh_transfer(vector<unsigned short>& buff16, int& saddr) 
{

    input.first_row = false;
    if(saddr == colsize)
    {
        input.first_row = true;
    }
    
    for(int i = 0; i < colsize; i++){

        p1.set_address(saddr + i);
        p1.set_command(TLM_READ_COMMAND);
        p1.set_data_length(2);
        p1.set_data_ptr((unsigned char*)buff_read);
        p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

        dma_soft_socket->b_transport(p1, offset);
        
        out_dma = toShort(buff_read);
        buff16[i] = out_dma;

        input.last = false;
        if(i == (colsize - 1))
        {
            input.last = true;
        }
        input.two_bytes = buff16[i];
        wr_port -> write(input, i);
        input.last = false;
    }

}

void Dma::hs_transfer(vector<unsigned short>& buff16, int& saddr)
{

    for(int i = 0; i < colsize; i++)
    {

        input.last = false;
        if(i == (colsize - 1))
        {
            input.last = true;
        }
        rd_port -> read(input, i);
        buff16[i] = input.two_bytes;
        input.last = false;
        toUchar(buff_write, buff16[i]);
        
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
                   dm();
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