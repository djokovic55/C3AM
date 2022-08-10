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
                    hard_toggle_row = false;
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;
                
                case HARD_ROWSIZE:
                    rowsize = *((int*)data);
                    p1.set_response_status(TLM_OK_RESPONSE);
                    break;
                case HARD_COLSIZE:
                    colsize = *((int*)data);
                    // cache init
                    cache.clear();
                    for(int i = 0; i < 2*colsize; i++)
                        cache.push_back(0);
                    cout<<"---------------------------> Cache size: "<<cache.size()<<endl;
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


void Hard::write(Data& data, int i)
{
    if(control)
    {
        // flag togluje upis gornji-donji red

        if(hard_toggle_row)
            write_read_start_addr = colsize;
        else 
            write_read_start_addr = 0;

        cache[write_read_start_addr + i] = data.pixel;

        if(!data.first_row && data.last)
        {
            hard_cem();
        }
    }
}

void Hard::read(Data& data, int i)
{
    data.pixel = cache[write_read_start_addr + i];
    if(data.last)
       hard_toggle_row = !hard_toggle_row; 
}

void Hard::hard_cem() {

    // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
    unsigned short a, b, c, min;
    unsigned short target_pixel_addr, abc_addr;
	unsigned short col;

    // generisanje pocetnih adresa
    if(hard_toggle_row)
    {
        col = 0;
        abc_addr = col;
        target_pixel_addr = col + colsize;
    } 
    else
    {
        col = 0;
        abc_addr = col + colsize;
        target_pixel_addr = col;
    }
  
	// Levi granicni slucaj
    b = cache.at(abc_addr);
    min = b;
    
    c = cache.at(abc_addr + 1);
        
    if(c < b)
    {
        min = c;
    }

    cache.at(target_pixel_addr) = cache.at(target_pixel_addr) + min;
		
    col++;
    abc_addr++;
    target_pixel_addr++;

	// Srednji slucaj

    loop:
		a = b;
		b = c;
		min = b;
   
		c = cache.at(abc_addr + 1);
		
		if(a < c)
		{
			if(a < b)
				min = a;
		}
		else
		{
			if(c < b)
				min = c;
		}	

        cache.at(target_pixel_addr) = cache.at(target_pixel_addr) + min;

        col++;
        abc_addr++;
        target_pixel_addr++;

	if(col < colsize - 1)
        goto loop;	
	// Desni granicni slucaj
	
    a = b;
    b = c;
    min = b;
        
    if(a < b)
    {
        min = a;
    }

    cache.at(target_pixel_addr) = cache.at(target_pixel_addr) + min;
}

// replace first and second row in cache
void Hard::cache_substitution(Data& data)
{
    if(data.last)
    {
        for(int j = 0; j < colsize; j++)
        {
            cache[j] = cache[colsize + j];
        }
    }
}