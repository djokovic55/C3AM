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
        sc_buff8.push_back(data.byte);
        
        if(data.last)
        {
            //cout<<"ddr in hard: "<< sc_buff8[5]<<endl;
            //print_1d_sc8(sc_buff8);
            sc_buff16 = hard_cem(sc_buff8, rowsize, colsize);
            sc_buff8.clear();
            // print_1d_sc16(sc_buff16);
            control = 0;
        }
    }

}

void Hard::read(Data& data, int i)
{
    data.two_bytes = sc_buff16[i];

}

// void Hard::calculate()
// {
//     for(int i = 0; i < sc_buff8.size(); i++)
//     {
//        sc_buff16.push_back(sc_buff8[i] + 255);
//        cout<<"after calculate: "<< sc_buff16[i]<<endl;
//     }
// }


vector<sc_uint<16>> Hard::hard_cem(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize) {

    sc_uint<16> a, b, c;
    int index_1d;
    // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
    
    // vector<sc_uint<16>> energy_image_16b = convert_from_8b_to_16b(energy_image);

    vector<sc_uint<16>> energy_image_16b (energy_image.begin(), energy_image.end());

        for (int row = 1; row < rowsize; row++) {
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
