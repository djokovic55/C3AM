
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

unsigned short toShort(unsigned char *buf)
{
    unsigned short val = 0;
    val += ((unsigned short)buf[0]) << 8;
    val += ((unsigned short)buf[1]);
    return val;
}

void toUchar(unsigned char *buf,unsigned short val)
{
    buf[0] = (unsigned char) (val >> 8);
    buf[1] = (unsigned char) (val);
}

int sc_main(int argc, char** argv) {

    cout<<"Hello world"<<endl;

    vector<unsigned char> vect1;
    vect1.reserve(10);
    cout<<"vec size = "<<vect1.size()<<endl;
    for(int i = 0; i < 10; i++){
        cout<< "usao u petlju"<< endl;
    vect1[i] = 2;
    cout<<"vect1: " << int(vect1[i])<<endl;
    }
    // vect1[0] = 5;
    // cout<< "size "<< vect1.size()<<endl;

    // vect1.clear();
    
    // cout<< "size after clear"<< vect1.size()<<endl;
    
    //     cout<<"vect before"<<vect1[0]<<endl;
    //     vect1[0] = 1;
        
    //     cout<<"vect after"<<vect1[0]<<endl;
        
    //     cout<<"vect size: "<< vect1.size()<<endl;
    // }    




    // vector<unsigned char> vect2 {5, 2 , 6};

    // vect1.assign(vect2.begin(), vect2.end());


    // vector<unsigned short> vect2 {5, 2 , 6};

    // vector<unsigned short> vect_in {500, 2000 , 650, 65535};
    // vector<unsigned short> vect_out;

    // unsigned short out;

    // unsigned char niz[2];

    // for(int i = 0; i < vect_in.size(); i++){
    //     //predajna strana
    //     toUchar(niz, vect_in[i]);

    //     //prijemna strana
    //     unsigned char* buff = (unsigned char*)niz;
    //     out = toShort(buff);
    //     vect_out.push_back(out);
    //     cout<<"Vector out: "<<vect_out[i]<<endl;

    // }
    //unsigned char* buff = (unsigned char*)niz;

    //unsigned char* buff = (unsigned char*)&vect1.front();

    // unsigned short p_in = 32896;
    // unsigned char niz[2];

    // toUchar(niz, p_in);

    // cout<<"niz uchar vrednost: "<<int(niz[1])<< endl;

    // unsigned short p_out = toShort(niz);


    // cout<<"rezultat nakon pakovanja: "<<p_out<<endl;



    //cout<<"first element vect2: "<< int(vect1[0])<<endl;
//    short x = 65537;
//    cout<<x<<endl;
    // vector<sc_uint<8>> vect1(10);
    // unsigned char buff[10];
    // buff[0] = '1';

    //vect1.reserve(10);

    //vect1[0] = (unsigned char)(10);

  //  unsigned char* buff = (unsigned char*)&vect1.front();

//    buff[0] = 12;
//    buff[1] = 5;
//    buff[2] = 6;
//    buff[25] = 78;
    // unsigned char x = 15;
    // unsigned char y = x + 5;
    // unsigned char z = y + 237;
    

    
  

   
//     cout<<int(buff[25])<<endl;




  //cout<<buff[0];
    // int x = int(buff[0]);
    // cout<<x<<endl;

    // for(int i = 0; i < vect1.size(); i++)
    // {
    //     buff[i] = 5;

    // }
    // buff[0] += 5;
    

   // cout<<"un char: "<<int(buff[0])<<endl;
    // for(int i = 0; i < vect1.size(); i++)
    //     cout<<"un char"<<int(buff[i])<<endl;

    return 0;
}