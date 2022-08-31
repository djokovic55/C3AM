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
        SC_HAS_PROCESS(Hard);
        Hard(sc_core::sc_module_name name);
        ~Hard();

        tlm_utils::simple_target_socket<Hard> hard_intcon_socket;
        void write(Data& data);
        void read(Data& data);

        // sc_out<int> to_soft_h;
        
    protected:
        pl_t p1;
        sc_core::sc_time offset;
        void b_transport(pl_t&, sc_core::sc_time&);
        void hard_cem();

        // sc_event hard_start; 


        int control;
        int rowsize;
        int colsize;
        bool hard_toggle_row; 

        int cache_raddr;
        int cache_waddr;

        bool hard_done;
        
        std::vector<unsigned short> cache;

        // void cache_substitution(Data& data);

};

#endif