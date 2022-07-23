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

    Data input;
    vector<unsigned short> buff16(colsize);

    //izmena
    int saddr = colsize; 
    unsigned char buff_write[2];
    unsigned char buff_read[2];

    unsigned short out_dma;

    pl_t p1;
    cout<<"---------------------------> Number of pixels: "<<rowsize*colsize<<endl;
    for(int j = 0; j < rowsize - 1; j++)
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
            
            // cout<<"prosao b_transport"<<endl;
            // cout<<"first byte: "<<int(buff_read[0])<<endl;

            // cout<<"second byte: "<<int(buff_read[1])<<endl;

            out_dma = toShort(buff_read);
            
            // cout<<"Collected element: "<<out_dma<<"  "<< "address: "<<i<<endl;
            
            buff16[i] = out_dma;
            // cout<<"buff16 "<<buff16[i]<<endl;

            input.last = false;
            if(i == (colsize - 1))
            {
                input.last = true;
            }
            input.two_bytes = buff16[i];
            wr_port -> write(input, i);
            input.last = false;
        }
        // cout<<"prosao drugi for"<<endl;
        
        // mess("Dma", "before sending to hard");
        // cout<<buff16.size()<<endl;
        // print_1d_sh(buff16);



        // exit(0);
        // if(j == 0)
        // {
        //     cout<<"dma: "<<endl;
        //     print_1d_sh(buff16);
        // }
        // exit(0);
        // input.first_row = false;
        // if(saddr == colsize)
        // {
        //     input.first_row = true;
        // }
        // for(int i = 0; i < colsize; i++)
        // {
        //     input.last = false;
        //     if(i == (colsize - 1))
        //     {
        //         input.last = true;
        //     }
        //     input.two_bytes = buff16[i];
        //     wr_port -> write(input, i);
        //     input.last = false;
        // }


        // procitaj iz kanala i upisi u buff16
        for(int i = 0; i < colsize; i++)
        {

            input.last = false;
            if(i == (colsize - 1))
            {
                input.last = true;
            }
            rd_port -> read(input, i);
            // cout<<i+1<< " input.two_bytes: "<< input.two_bytes<<endl;
            buff16[i] = input.two_bytes;
            input.last = false;
            // cout <<endl<<"Niz nakon citanja: "<< i <<" "<<buff16[i] << endl;

            toUchar(buff_write, buff16[i]);
            // cout<<"buff za upis, dma "<<endl<<"bajt 1: "<<int(buff_write[0])<<endl<<"bajt 2: "<<int(buff_write[1])<<endl;
            p1.set_address(saddr + i);
            p1.set_command(TLM_WRITE_COMMAND);
            // ovo ovde se ne odnosi na broj podataka koji se upisuje
            // vec posto upis ide preko push_back() nece biti potreban
            // pojam od koje adrese se nastavlja upis jer se svakako sve nadodaje
            // zbog if-a u write delu saljemo zapravo dimenzije slike kako bismo znali kada da se izvrsi clear()
            p1.set_data_length(1);
            p1.set_data_ptr((unsigned char*)buff_write);
            p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

            dma_soft_socket->b_transport(p1, offset);

        }

        // if(j == 0)
        // {
        //     cout<<"dma after hard: "<<endl;
        //     print_1d_sh(buff16);
        // }
        // mess("Dma", "After receiving from hard");
        // print_1d_sh(buff16);

        // exit(EXIT_FAILURE);
        // cout<<"----------"<<endl;
        // for(int i = 0; i < colsize; i++)
        // {
        //     toUchar(buff_write, buff16[i]);
        //     // cout<<"buff za upis, dma "<<endl<<"bajt 1: "<<int(buff_write[0])<<endl<<"bajt 2: "<<int(buff_write[1])<<endl;
        //     p1.set_address(saddr + i);
        //     p1.set_command(TLM_WRITE_COMMAND);
        //     // ovo ovde se ne odnosi na broj podataka koji se upisuje
        //     // vec posto upis ide preko push_back() nece biti potreban
        //     // pojam od koje adrese se nastavlja upis jer se svakako sve nadodaje
        //     // zbog if-a u write delu saljemo zapravo dimenzije slike kako bismo znali kada da se izvrsi clear()
        //     p1.set_data_length(1);
        //     p1.set_data_ptr((unsigned char*)buff_write);
        //     p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

        //     dma_soft_socket->b_transport(p1, offset);

        // }

        // exit(EXIT_FAILURE);
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