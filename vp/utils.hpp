#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include <systemc>
#include <fstream>
#include <math.h>


using namespace cv;
using namespace std;
using namespace sc_dt;

// CONVERSION FUNTIONS
vector<vector<sc_uint<8>>> convert_to_vect(Mat& mat_image);

Mat convert_to_mat(vector<vector<sc_uint<16>>> &vec_image);

vector<vector<sc_uint<16>>> convert_to_2d(vector<sc_uint<16>> &vect_1d, int &rowsize, int &colsize);

vector<sc_uint<8>> convert_to_1d(vector<vector<sc_uint<8>>> &vect_2d, int &rowsize, int &colsize);

int row_num(Mat &image);
int col_num(Mat &image);

vector<sc_uint<16>> convert_from_8b_to_16b(vector<sc_uint<8>> &sc_image);

void print_1d (vector<int> &vector_1d);

void print_2d(vector<vector<int>> &vector_2d);
#endif 
