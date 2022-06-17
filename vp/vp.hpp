#ifndef VP_HPP
#define VP_HPP

#include "utils.hpp"
#include "soft.hpp"
#include "ddr.hpp"



class Vp : public sc_core::sc_module{
    public:
        Vp(sc_core::sc_module_name name, int argc, char** argv);
        ~Vp();

    protected:
        Soft soft;
        Ddr ddr;
};

#endif