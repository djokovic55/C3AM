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

        tlm_utils::simple_target_socket<Hard> hard_intcon_socket;
        void write(Data& data, int i);
        void read(Data& data, int i);
        
        void hard_cem();
    protected:
        pl_t p1;
        sc_core::sc_time offset;
        void b_transport(pl_t&, sc_core::sc_time&);

        int control = 0;
        int rowsize;
        int colsize;
        bool hard_toggle_row; 
        int write_read_start_addr;
        
        std::vector<unsigned short> cache;
        // std::vector<unsigned short>  buff16_copy;
        unsigned short first_row_element;

        void cache_substitution(Data& data);

};

#endif