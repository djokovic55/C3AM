
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include <systemc>
#include <fstream>
#include <math.h>
#include <tlm>


using namespace std;
using namespace sc_dt;

int sc_main(int argc, char** argv) {

    cout<<"Hello world"<<endl;
        
    vector<sc_uint<8>> vect;
    vect.reserve(10);
    vect.push_back(50);
    cout<<vect[0]<<endl<<vect.capacity()<<endl;  


    // *((sc_uint<16>*)&vect[0]) = 500;
    // int a = 500;
    // sc_uint<8> b;
    // b = a;

    // cout<<b<<endl;

    return 0;
}