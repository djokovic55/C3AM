#ifndef HARD_IF_HPP
#define HARD_IF_HPP

#include <systemc>
#include <vector>

using namespace sc_dt;
using namespace std;
using namespace sc_core;

typedef struct
{
	sc_uint<8> byte;
	sc_uint<16> two_bytes;
	bool last;
}Data;

class hard_write_if : virtual public sc_core::sc_interface
{
	public:
		virtual void write(const Data& data) = 0;
};

class hard_read_if: virtual public sc_core::sc_interface
{
	public:
		virtual void read(Data& data, int i) = 0;
};

#endif