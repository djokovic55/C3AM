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
    vector<unsigned short int> buff16(cnt);

    vector<sc_uint<8>> sc_buff8;
    vector<sc_uint<16>> sc_buff16;
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

            sc_buff8.assign(buff8.begin(), buff8.end());

            // print_1d_sc8(sc_buff8);
            //cout<<"ddr in dma: "<< sc_buff8[50]<<endl;

            // posalji buff8 preko kanala i postavi control na 0

            // for(int i = 0; i < sc_buff8.size(); i++)
            // {
            //     input.last = false;
            //     if(i == (sc_buff8.size() - 1))
            //     {
            //         input.last = true;
            //     }
            //     input.byte = sc_buff8[i];
            //     wr_port -> write(input);
            // }

            
            cout<<"---------------------------> Number of pixels: "<<cnt<<endl;
            for(int i = 0; i < cnt; i++)
            {
                input.last = false;
                if(i == (cnt - 1))
                {
                    input.last = true;
                }
                input.byte = sc_buff8[i];
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
                sc_buff16.push_back(input.two_bytes);
                //cout <<"Niz nakon citanja: "<<sc_buff16[i] << endl;
            }

            buff16.assign(sc_buff16.begin(), sc_buff16.end());
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