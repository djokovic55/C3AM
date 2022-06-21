#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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


using namespace cv;
using namespace std;
using namespace sc_dt;
using namespace chrono;
// *******************TYPES**********************
typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;

//********************FUNTIONS**************
vector<vector<sc_uint<8>>> convert_to_vect(Mat& mat_image);
Mat convert_to_mat(vector<vector<sc_uint<16>>> &vec_image);
vector<vector<sc_uint<16>>> convert_to_2d(vector<sc_uint<16>> &vect_1d, int &rowsize, int &colsize);
vector<sc_uint<8>> convert_to_1d(vector<vector<sc_uint<8>>> &vect_2d, int &rowsize, int &colsize);

int row_num(Mat &image);
int col_num(Mat &image);

// vector<sc_uint<16>> convert_from_8b_to_16b(vector<sc_uint<8>> &sc_image);
void print_1d (vector<int> &vector_1d);
void print_2d(vector<vector<int>> &vector_2d);


// *****************ADDRESS SPACE************************
// register in hard
#define BEGIN_ADDR 0x0

// memory and hard
#define HARD_L_VP 0x41000000
#define HARD_H_VP 0x410000FF
#endif 
