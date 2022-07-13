#include "hard.hpp"

using namespace sc_core;
using namespace std;

Hard::Hard(sc_core::sc_module_name name): sc_channel(name)
{
    hard_intcon_socket.register_b_transport(this, &Hard::b_transport);
    SC_REPORT_INFO("Hard", "Constructed.");
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


void Hard::write(const Data& data)
{
    if(control)
    {
        buff8.push_back(data.byte);
        
        if(data.last)
        {
            //cout<<"ddr in hard: "<< sc_buff8[5]<<endl;
            // print_1d_uc(buff8);
            buff16 = hard_cem(buff8, rowsize, colsize);
            // print_1d_sh(buff16);
            buff8.clear();
            // print_1d_sc16(sc_buff16);
            control = 0;
        }
    }

}

void Hard::read(Data& data, int i)
{
    data.two_bytes = buff16[i];

}



vector<unsigned short> Hard::hard_cem(vector<unsigned char> &energy_image, int &rowsize, int &colsize) {

    unsigned short a, b, c;
    int index_1d;
    // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
    
    vector<unsigned short> energy_image_16b (energy_image.begin(), energy_image.end());

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
