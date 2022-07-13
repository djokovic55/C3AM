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
    vector<unsigned char> buff8(2*colsize);
    vector<unsigned short int> buff16;

    // vector<sc_uint<8>> v {7, 5, 1, 3, 8};
    // vector<sc_uint<16>> n;

    // vector<unsigned short> n_sh;
    int saddr = 0; 
    unsigned char buff[2];

    pl_t p1;
    cout<<"---------------------------> Number of pixels: "<<rowsize*colsize<<endl;
    for(int j = 0; j < rowsize - 1; j++)
    {
        p1.set_address(saddr);
        p1.set_command(TLM_READ_COMMAND);
        p1.set_data_length(2*colsize);
        p1.set_data_ptr((unsigned char*)buff8.data());
        p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

        dma_soft_socket->b_transport(p1, offset);
        // print_1d_uc(buff8);
        for(int i = 0; i < 2*colsize; i++)
        {
            input.last = false;
            if(i == (2*colsize - 1))
            {
                input.last = true;
            }
            input.byte = buff8[i];
            wr_port -> write(input);
            input.last = false;
        }


        // procitaj iz kanala i upisi u buff16
        for(int i = 0; i < 2*colsize; i++)
        {
            rd_port -> read(input, i);
            // cout<< "input.two_bytes: "<< input.two_bytes<<endl;
            buff16.push_back(input.two_bytes);
            // cout <<endl<<"Niz nakon citanja: "<< i <<" "<<buff16[i] << endl;
        }
        // cout<<"dma: pre slanja podataka u memoriju"<<endl;
        // print_1d_sh(buff16);
        for(int i = 0; i < 2*colsize; i++)
        {
            toUchar(buff, buff16[i]);

            p1.set_address(saddr);
            p1.set_command(TLM_WRITE_COMMAND);
            // ovo ovde se ne odnosi na broj podataka koji se upisuje
            // vec posto upis ide preko push_back() nece biti potreban
            // pojam od koje adrese se nastavlja upis jer se svakako sve nadodaje
            // zbog if-a u write delu saljemo zapravo dimenzije slike kako bismo znali kada da se izvrsi clear()
            p1.set_data_length(rowsize*colsize);
            p1.set_data_ptr((unsigned char*)buff);
            p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

            dma_soft_socket->b_transport(p1, offset);

        }
        saddr += colsize;
    }
    control = 0;

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