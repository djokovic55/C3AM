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
using namespace tlm;
// *******************TYPES**********************
typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;

//********************FUNTIONS**************
vector<vector<unsigned char>> convert_to_vect(Mat& mat_image);
Mat convert_to_mat(vector<vector<unsigned short>> &vec_image);
vector<vector<unsigned short>> convert_to_2d(vector<unsigned short> &vect_1d, int &rowsize, int &colsize);
vector<unsigned char> convert_to_1d(vector<vector<unsigned char>> &vect_2d, int &rowsize, int &colsize);

int row_num(Mat &image);
int col_num(Mat &image);

vector<unsigned short> convert_from_8b_to_16b(vector<unsigned char> &image_8b);
void print_1d_sh (vector<unsigned short> &vector_1d);

void mess(const char*, const char*);
unsigned short toShort(unsigned char *buf);
void toUchar(unsigned char *buf,unsigned short val);

#endif 
