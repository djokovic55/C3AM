#ifndef HARD_IF_HPP
#define HARD_IF_HPP

#include <systemc>
#include <vector>

using namespace sc_dt;
using namespace std;
using namespace sc_core;

typedef struct
{
	unsigned short pixel;
	bool last;
	bool hard_start;
}Data;

class hard_write_if : virtual public sc_core::sc_interface
{
	public:
		virtual void write(Data& data) = 0;
};

class hard_read_if: virtual public sc_core::sc_interface
{
	public:
		virtual void read(Data& data) = 0;
};

#endif