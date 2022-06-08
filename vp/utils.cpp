#include "utils.hpp"


    vector<vector<sc_uint<8>>> convert_to_vect(Mat& mat_image){


        int rowsize = mat_image.rows;
        int colsize = mat_image.cols;

        vector<vector<sc_uint<8>>> vec_image(rowsize, vector<sc_uint<8>> (colsize, 0));

        for (int i = 0; i < rowsize; i++)
        {
            for (int j = 0; j < colsize; j++)
            {
                vec_image[i][j] = mat_image.at<int>(i, j);
            }   
        }
        
        return vec_image;
    }

    Mat convert_to_mat(vector<vector<sc_uint<16>>> &vec_image){

        int rowsize = vec_image.size();
        int colsize = vec_image[0].size();

        Mat mat_image(rowsize, colsize, CV_32S, int(0));

        for (int i = 0; i < rowsize; i++)
        {
            for (int j = 0; j < colsize; j++)
            {
                mat_image.at<int>(i, j) = vec_image[i][j];
            }   
        }
        return mat_image;

    }

    vector<sc_uint<8>> convert_to_1d(vector<vector<sc_uint<8>>> &vect_2d, int &rowsize, int &colsize){

        int i, j, k;
        vector<sc_uint<8>> vect_1d(rowsize*colsize, 0);
            for(i = 0; i < rowsize; i++){ 
                for(j = 0; j < colsize; j++){

                    k = (i * colsize) + j;
                    vect_1d[k] = vect_2d[i][j];
                
            }
        }
        return vect_1d;
    }

    vector<vector<sc_uint<16>>> convert_to_2d(vector<sc_uint<16>> &vect_1d, int &rowsize, int &colsize){

        //assert(rowsize* colsize == vect_1d.size());
        vector<vector<sc_uint<16>>> vect_2d(rowsize, vector<sc_uint<16>>(colsize, 0));
        for(int i = 0; i < vect_1d.size(); i++){

            int row = i / colsize;
            int col = i % colsize; 

            vect_2d[row][col] = vect_1d[i];
        }
        return vect_2d;     
    }

    int row_num(Mat &image) {int rowsize = image.rows; return rowsize;}
    int col_num(Mat &image) {int colsize = image.cols; return colsize;}

    vector<sc_uint<16>> convert_from_8b_to_16b(vector<sc_uint<8>> &sc_image){

        int length = sc_image.size();
          

        vector<sc_uint<16>> image(length, 0);

        for (int i = 0; i < length; i++)
        {

                image[i] = sc_image[i];
               
        }

    return image;
    }

    void print_1d (vector<int> &vector_1d){
        
        cout<<"1d vector: "<<endl;
        for(int i = 0; i < vector_1d.size(); i++){
            cout<<vector_1d[i]<<' ';
        }
        cout<<endl;
    }

    void print_2d(vector<vector<int>> &vector_2d){

        cout<<"2d vector: "<<endl;
        for (int i = 0; i < vector_2d.size(); i++) {
            for (int j = 0; j < vector_2d[i].size(); j++) {
                cout<<vector_2d[i][j]<<' ';
            }
        }
        cout<<endl;
    }
