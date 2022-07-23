#include "hard.hpp"

using namespace sc_core;
using namespace std;

Hard::Hard(sc_core::sc_module_name name): sc_channel(name)
{
    hard_intcon_socket.register_b_transport(this, &Hard::b_transport);
    // SC_REPORT_INFO("Hard", "Constructed.");
}

Hard::~Hard()
{
    SC_REPORT_INFO("Hard", "Destructed.");
}

void Hard::b_transport(pl_t &p1, sc_core::sc_time &offset)
{
    
    tlm_command cmd = p1.get_command();
    sc_dt::uint64 addr = p1.get_address();
    unsigned char *data = p1.get_data_ptr();
    unsigned int length = p1.get_data_length();

    switch(cmd)
    {
        case TLM_WRITE_COMMAND:
            switch(addr)
            {
                case HARD_CONTROL:
                    control = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;
                
                case HARD_ROWSIZE:
                    rowsize = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;
                case HARD_COLSIZE:
                    colsize = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;

                case HARD_CASH:

                    first_row_element = toShort(data);
                    buff16.push_back(first_row_element);
                    
                    // cout<<"Element: "<<first_row_element<<endl;

                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;
                default:
                    p1.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                    break;
            }
        case TLM_READ_COMMAND:
            switch(addr)
            {
                case HARD_CONTROL:
                    memcpy(data, &control, sizeof(control));
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;

                default:
                    p1.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                    break;
            }
        default:
            p1.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            break;
    }
}


void Hard::write(const Data& data, int i)
{
    if(control)
    {

        if(data.first_row)
        {
            buff16.push_back(data.two_bytes);
            if(data.last)
            {
                buff16_copy.assign(buff16.begin(), buff16.end());
                buff16.clear();
            }
        }
        else{

            buff16_copy[colsize + i] = data.two_bytes;
        }

        
        if(data.last)
        {
            // mess("Hard", "received data");
            // print_1d_sh(buff16);
            // exit(EXIT_FAILURE);
            buff16_copy = hard_cem(buff16_copy, rowsize, colsize);
            // print_1d_sh(buff16);
            // buff16.clear();
            // print_1d_sc16(sc_buff16);
            // BUG !!!!!!
                    // control = 0;
        }
    }

}

void Hard::read(Data& data, int i)
{
    data.two_bytes = buff16_copy[colsize + i];
    // cout<<"two_butes, hard: "<<data.two_bytes<<endl;
    if(data.last)
    {
        for(int j = 0; j < colsize; j++)
        {
            buff16_copy[j] = buff16_copy[colsize + j];
        }
    }
}



vector<unsigned short> Hard::hard_cem(vector<unsigned short> &energy_image_16b, int &rowsize, int &colsize) {

    unsigned short a, b, c;
    int index_1d;
    // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
    
    // vector<unsigned short> energy_image_16b (energy_image.begin(), energy_image.end());

        for (int row = 1; row < 2; row++) {
            for (int col = 0; col < colsize; col++) {
                index_1d = (row * colsize) + col;

                b = energy_image_16b.at(index_1d - colsize);

                if(col == 0){
                    a = b;
                    
                }else{
                    a = energy_image_16b.at(index_1d - (colsize + 1));

                }
                if(col == (colsize - 1)){
                    c=b;
                }else {
                    c = energy_image_16b.at(index_1d - (colsize - 1));
                }
                energy_image_16b.at(index_1d) = energy_image_16b.at(index_1d) + std::min(a, min(b, c));
            }
        }
    return energy_image_16b;
}
