#include "utils.hpp"


    vector<vector<unsigned char>> convert_to_vect(Mat& mat_image){


        int rowsize = mat_image.rows;
        int colsize = mat_image.cols;

        vector<vector<unsigned char>> vec_image(rowsize, vector<unsigned char> (colsize, 0));

        for (int i = 0; i < rowsize; i++)
        {
            for (int j = 0; j < colsize; j++)
            {
                vec_image[i][j] = mat_image.at<int>(i, j);
            }   
        }
        
        return vec_image;
    }

    Mat convert_to_mat(vector<vector<unsigned short>> &vec_image){

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

    vector<unsigned char> convert_to_1d(vector<vector<unsigned char>> &vect_2d, int &rowsize, int &colsize){

        int i, j, k;
        vector<unsigned char> vect_1d(rowsize*colsize, 0);
            for(i = 0; i < rowsize; i++){ 
                for(j = 0; j < colsize; j++){

                    k = (i * colsize) + j;
                    vect_1d[k] = vect_2d[i][j];
                
            }
        }
        return vect_1d;
    }

    vector<vector<unsigned short>> convert_to_2d(vector<unsigned short> &vect_1d, int &rowsize, int &colsize){

        vector<vector<unsigned short>> vect_2d(rowsize, vector<unsigned short>(colsize, 0));

        for(int i = 0; i < vect_1d.size(); i++){

            int row = i / colsize;
            int col = i % colsize; 

            vect_2d[row][col] = vect_1d[i];
        }
        return vect_2d;     
    }

    int row_num(Mat &image) {int rowsize = image.rows; return rowsize;}
    int col_num(Mat &image) {int colsize = image.cols; return colsize;}

    vector<unsigned short> convert_from_8b_to_16b(vector<unsigned char> &image_8b){

        int length = image_8b.size();
        vector<unsigned short> image_16b(length, 0);

        for (int i = 0; i < length; i++)
        {

                image_16b[i] = image_8b[i];
               
        }
    return image_16b;
    }

    // void print_1d_uc (vector<unsigned char> &vector_1d){
        
    //     cout<<"1d vector: "<<endl;
    //     for(int i = 0; i < vector_1d.size(); i++){
    //         cout<<int(vector_1d[i])<<' ';
    //     }
    //     cout<<endl;
    // }


    void print_1d_sh(vector<unsigned short> &vector_1d){
        
        for(int i = 0; i < vector_1d.size(); i++)
        {
            cout<<vector_1d[i]<<' ';
    
        }
        cout<<endl;
        cout<<endl;
    }

    // void print_2d(vector<vector<int>> &vector_2d){

    //     cout<<"2d vector: "<<endl;
    //     for (int i = 0; i < vector_2d.size(); i++) {
    //         for (int j = 0; j < vector_2d[i].size(); j++) {
    //             cout<<vector_2d[i][j]<<' ';
    //         }
    //     }
    //     cout<<endl;
    // }


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

void mess(const char* part, const char* message)
{
    cout<<"Module: "<<part<<", "<<message<<endl;
}