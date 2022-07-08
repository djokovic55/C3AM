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

    Data input;
    vector<unsigned char> buff8(cnt);
    vector<unsigned short int> buff16;

    // vector<sc_uint<8>> v {7, 5, 1, 3, 8};
    // vector<sc_uint<16>> n;

    // vector<unsigned short> n_sh;
    
    unsigned char buff[2];

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
            // print_1d_uc(buff8);
            cout<<"---------------------------> Number of pixels: "<<cnt<<endl;
            for(int i = 0; i < cnt; i++)
            {
                input.last = false;
                if(i == (cnt - 1))
                {
                    input.last = true;
                }
                input.byte = buff8[i];
                wr_port -> write(input);
                input.last = false;
            }

            control = 0;
            break;

        case DDR_L:
            // procitaj iz kanala i upisi u buff16
            for(int i = 0; i < cnt; i++)
            {
                rd_port -> read(input, i);
                // cout<< "input.two_bytes: "<< input.two_bytes<<endl;
                buff16.push_back(input.two_bytes);
                // cout <<endl<<"Niz nakon citanja: "<< i <<" "<<buff16[i] << endl;
            }
            // cout<<"dma: pre slanja podataka u memoriju"<<endl;
            // print_1d_sh(buff16);
            for(int i = 0; i < cnt; i++)
            {
                toUchar(buff, buff16[i]);

                p1.set_address(saddr);
                p1.set_command(TLM_WRITE_COMMAND);
                p1.set_data_length(cnt);
                p1.set_data_ptr((unsigned char*)buff);
                p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

                dma_soft_socket->b_transport(p1, offset);

            }
            control = 0;  
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