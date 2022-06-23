#ifndef VP_HPP
#define VP_HPP

#include "utils.hpp"
#include "soft.hpp"
#include "dma.hpp"
#include "intcon.hpp"
#include "hard.hpp"




class Vp : public sc_core::sc_module{
    public:
        Vp(sc_core::sc_module_name name, int argc, char** argv);
        ~Vp();

    protected:
        Hard hard;
        Soft soft;
        Dma dma;
        Intcon intcon;
        
        
};

#endif