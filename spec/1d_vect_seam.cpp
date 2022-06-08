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
    Mat createEnergyImage(Mat& image) {
    
        Mat image_blur, image_gray;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Mat grad, energy_image;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        // apply a gaussian blur to reduce noise
        GaussianBlur(image, image_blur, Size(3, 3), 0, 0, BORDER_DEFAULT);
    
        // convert to grayscale
        cvtColor(image_blur, image_gray, COLOR_BGR2GRAY);

        // use Scharr to calculate the gradient of the image in the x and y direction
        Scharr(image_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
        Scharr(image_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);

        // convert gradients to abosulte versions of themselves
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);

        // total gradient (approx)
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        // convert the default values to int precision
        grad.convertTo(energy_image, CV_32S, 1, 0);

        return energy_image;
    }

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
/*
    Mat create_empty_cem(int &rowsize, int &colsize, Mat& energy_image){

        // initialize the map with zeros
        Mat empty_cem = Mat(rowsize, colsize, CV_32S, int(0));

        // copy the first row, seam is only vertical
        energy_image.row(0).copyTo(empty_cem.row(0));

        return empty_cem;
    }
*/
    vector<sc_uint<16>> createCumulativeEnergyMap(vector<sc_uint<8>> &energy_image, int &rowsize, int &colsize) {

        sc_uint<16> a, b, c;
        int index_1d;
        // take the minimum of the three neighbors and add to total, this creates a running sum which is used to determine the lowest energy path
        
        vector<sc_uint<16>> energy_image_16b = convert_from_8b_to_16b(energy_image);


            for (int row = 1; row < rowsize; row++) {
                for (int col = 0; col < colsize; col++) {
                    index_1d = (row * colsize) + col;

                    b = energy_image_16b.at(index_1d - colsize);

                    if(col == 0){
                        a = b;
                        
                    }else{
                        a = energy_image_16b.at(index_1d - (colsize + 1));

                    }
                    if(col == (colsize - 1)){
                        c=b;
                    }else {
                        c = energy_image_16b.at(index_1d - (colsize - 1));
                    }
                    energy_image_16b.at(index_1d) = energy_image_16b.at(index_1d) + std::min(a, min(b, c));
                }
            }
        return energy_image_16b;
    }
    
    vector<int> findOptimalSeam(Mat& cumulative_energy_map) {

        int a, b, c;
        int offset = 0;
        vector<int> path;
        double min_val, max_val;
        Point min_pt, max_pt;

        // get the number of rows and columns in the cumulative energy mapF
        int rowsize = cumulative_energy_map.rows;
        int colsize = cumulative_energy_map.cols;

            // copy the data from the last row of the cumulative energy map
            Mat row = cumulative_energy_map.row(rowsize - 1);

            // get min and max values and locations
            minMaxLoc(row, &min_val, &max_val, &min_pt, &max_pt);

            // initialize the path vector
            path.resize(rowsize);
            int min_index = min_pt.x;
            path[rowsize - 1] = min_index;

            // starting from the bottom, look at the three adjacent pixels above current pixel, choose the minimum of those and add to the path
            for (int i = rowsize - 2; i >= 0; i--) {
                a = cumulative_energy_map.at<int>(i, max(min_index - 1, 0));
                b = cumulative_energy_map.at<int>(i, min_index);
                c = cumulative_energy_map.at<int>(i, min(min_index + 1, colsize - 1));

                if (min(a, b) > c) {
                    offset = 1;
                }
                else if (min(a, c) > b) {
                    offset = 0;
                }
                else if (min(b, c) > a) {
                    offset = -1;
                }

                min_index += offset;
                min_index = min(max(min_index, 0), colsize - 1); // take care of edge cases
                path[i] = min_index;
            }
    
        return path;
    }

    void reduce(Mat& image, vector<int> path) {

        // get the number of rows and columns in the image
        int rowsize = image.rows;
        int colsize = image.cols;

        // create a 1x1x3 dummy matrix to add onto the tail of a new row to maintain image dimensions and mark for deletion
        Mat dummy(1, 1, CV_8UC3, Vec3b(0, 0, 0));

        // reduce the width
            for (int i = 0; i < rowsize; i++) {
                // take all pixels to the left and right of marked pixel and store them in appropriate subrow variables
                Mat new_row;
                Mat lower = image.rowRange(i, i + 1).colRange(0, path[i]);
                Mat upper = image.rowRange(i, i + 1).colRange(path[i] + 1, colsize);

                // merge the two subrows and dummy matrix/pixel into a full row
                if (!lower.empty() && !upper.empty()) {
                    hconcat(lower, upper, new_row);
                    hconcat(new_row, dummy, new_row);
                }
                else {
                    if (lower.empty()) {
                        hconcat(upper, dummy, new_row);
                    }
                    else if (upper.empty()) {
                        hconcat(lower, dummy, new_row);
                    }
                }
                // take the newly formed row and place it into the original image
                new_row.copyTo(image.row(i));
            }
            // clip the right-most side of the image
            image = image.colRange(0, colsize - 1);
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

    void driver(Mat& image, int iterations) {
    
        namedWindow("Original Image", WINDOW_AUTOSIZE); imshow("Original Image", image);
        // perform the specified number of reductions
        for (int i = 0; i < iterations; i++) {
            //SOFT PART
            int rowsize = row_num(image);
            int colsize = col_num(image);
            //Energy image, type Mat
            Mat energy_image_mat = createEnergyImage(image);
            //Energy image, type 2d 8b vector
            vector<vector<sc_uint<8>>> energy_image_vect_2d = convert_to_vect(energy_image_mat);
            //Energy image, type 1d 8b vector
            vector<sc_uint<8>> energy_image_vect_1d = convert_to_1d(energy_image_vect_2d, rowsize, colsize);

            //HARD PART
            //CEM, type 1d vector
            vector<sc_uint<16>> cumulative_energy_map_16b = createCumulativeEnergyMap(energy_image_vect_1d, rowsize, colsize);

            //SOFT PART 
            //CEM, type 2d vector
            vector<vector<sc_uint<16>>> cumulative_energy_map_2d = convert_to_2d(cumulative_energy_map_16b, rowsize, colsize);
            //CEM, type Mat
            Mat cumulative_energy_map_mat = convert_to_mat(cumulative_energy_map_2d);
            
            vector<int> path = findOptimalSeam(cumulative_energy_map_mat);
            reduce(image, path);
            cout<<"Seam "<<i+1<<" done."<<endl;
        
        }
        namedWindow("Reduced Image", WINDOW_AUTOSIZE); imshow("Reduced Image", image); waitKey(0);
        imwrite("result.jpg", image);
    }

    int sc_main(int argc, char* argv[]) {

        string filename = argv[1], s_iterations = argv[2];
        int iterations;

        //cout << "Please enter a filename: ";
        //cin >> filename;

        Mat image = imread(filename);
        if (image.empty()) {
            cout << "Unable to load image, please try again." << endl;
            exit(EXIT_FAILURE);
        }
        //cout << "Reduce width how many times? ";
        //cin >> s_iterations;

        iterations = stoi(s_iterations);
        int rowsize = image.rows;
        int colsize = image.cols;

        // check that inputted number of iterations doesn't exceed the image size
    
        if (iterations > colsize) {
            cout << "Input is greater than image's width, please try again." << endl;
            return 0;
        }

        driver(image, iterations);

/*       

        Mat energy_image = createEnergyImage(image);
        //cout<<image;
        vector<vector<int>> energy_image_2d = convert_to_vect(energy_image);
        print_2d(energy_image_2d);
        vector<int> energy_image_1d = convert_to_1d(energy_image_2d, rowsize, colsize);

        vector<int> cem_1d = createCumulativeEnergyMap(energy_image_1d, rowsize, colsize);
        //print_1d(cem_1d);
         cout << "\nMax Element = "<< *max_element(cem_1d.begin(), cem_1d.end());
         cout<<endl;
        vector<vector<int>> cem_2d = convert_to_2d(cem_1d, rowsize, colsize);
        //print_2d(cem_2d);

        Mat cem_mat = convert_to_mat(cem_2d);
        //cout<<"1d version"<<cem_mat<<endl;

*/

        


/*
// *************************   DEBUG *************************************
        int array[] = {3, 2, 10, 8, 1, 0, 4, 12, 13, 0, 6, 3, 1, 2, 8, 7, 3, 9, 14, 12, 18, 3, 0, 2, 6};
        Mat small_energy_image = Mat(5, 5, CV_32S, array);
        cout<<"Small image, Mat: "<<endl<<small_energy_image<<endl;

        int rowsize = small_energy_image.rows;
        int colsize = small_energy_image.cols;
        //cout<<rowsize<<endl;

        vector<vector<int>> small_energy_image_2d = convert_to_vect(small_energy_image);
        print_2d(small_energy_image_2d);

        vector<int> small_energy_image_1d = convert_to_1d(small_energy_image_2d, rowsize, colsize);
        print_1d(small_energy_image_1d);        

        vector<int> cumulative_energy_map_1d = createCumulativeEnergyMap(small_energy_image_1d, rowsize, colsize);
        print_1d(cumulative_energy_map_1d);

        
        vector<vector<int>> cumulative_energy_map_2d = convert_to_2d(cumulative_energy_map_1d, rowsize, colsize);
        print_2d(cumulative_energy_map_2d);

        Mat cumulative_energy_map_mat = convert_to_mat(cumulative_energy_map_2d);
        cout<<"Mat CEM, final: "<<endl<<cumulative_energy_map_mat<<endl;
// *********************************************************************************
*/
        return 0;
    }
