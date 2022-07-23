#ifndef HARD_HPP
#define HARD_HPP

#include "utils.hpp"
#include "hard_if.hpp"
#include "addr.hpp"

using namespace sc_core;
using namespace std;

class Hard : 
    //public sc_core::sc_module 
    public sc_core::sc_channel,
    public hard_write_if,
    public hard_read_if
{
    public:
        Hard(sc_core::sc_module_name name);
        ~Hard();

        tlm_utils::simple_target_socket<Hard> hard_intcon_socket; // 90% complete
        void write(const Data& data);
        void read(Data& data, int i);
        //implementation of hierarchical channel 0%
        
        vector<unsigned short> hard_cem(vector<unsigned short> &energy_image_16b, int &rowsize, int &colsize);
    protected:
        pl_t p1;
        sc_core::sc_time offset;
        int control = 0;
        void b_transport(pl_t&, sc_core::sc_time&);
        int rowsize;
        int colsize;
        std::vector<unsigned char> buff8;
        std::vector<unsigned short> buff16;

        std::vector<unsigned short> buff16_copy;

       

        // void calculate();
};

#endif