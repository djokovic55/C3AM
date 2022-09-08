#include "soft.hpp"

int input_num;
char** input_parameter;


Soft::Soft(sc_core::sc_module_name name, int argc, char** argv) : sc_module(name), offset(sc_core::SC_ZERO_TIME)
{

    soft_dma_socket.register_b_transport(this, &Soft::b_transport);
    SC_THREAD(seam_carving);

    SC_METHOD(dma_interrupt);
    dont_initialize();
    sensitive<<from_dma;

    dma_control = 1;

    SC_REPORT_INFO("Soft", "Constructed.");

    input_num = argc;
    input_parameter = argv;
    
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destructed.");
}

void Soft::dma_interrupt()
{
    dma_done.notify();
}

void Soft::seam_carving(){

    string filename = input_parameter[1], s_iterations = input_parameter[2];
    int iterations;

    Mat image = imread(filename);
    if (image.empty()) {
        cout << "Unable to load image, please try again." << endl;
        exit(EXIT_FAILURE);
    }

    iterations = stoi(s_iterations);
    int rowsize = image.rows;
    int colsize = image.cols;

    // check that inputted number of iterations doesn't exceed the image size
    if (iterations > colsize) {
        cout << "Input is greater than image's width, please try again." << endl;
    
    }else{
        driver(image, iterations);
    }

}

// **************DDR inside SOFT*********************
void Soft::b_transport(pl_t& p1, sc_core::sc_time& offset)
{
  	tlm_command cmd    = p1.get_command();
	sc_dt::uint64 addr = p1.get_address();
	unsigned char *buf = p1.get_data_ptr();
	unsigned int len   = p1.get_data_length();

    switch(cmd)
    {

        case TLM_WRITE_COMMAND:
            
            write_pixel = toShort(buf);
            ddr16[addr] = write_pixel;
            
            p1.set_response_status(TLM_OK_RESPONSE);
            break;
        case TLM_READ_COMMAND:
            
            toUchar(read_pixel, ddr16[addr]);
            for(int i = 0; i < len; i++)
            {
                buf[i] = read_pixel[i];
            }

            p1.set_response_status(TLM_OK_RESPONSE);
            break;
        default:
            p1.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
   } 
}
//***************************************************************** 

void Soft::driver(Mat& image, int iterations) {

    namedWindow("Original Image", WINDOW_AUTOSIZE); imshow("Original Image", image);
    // perform the specified number of reductions
    for (int i = 0; i < iterations; i++) {

        //SOFT PART
        rowsize = row_num(image);
        colsize = col_num(image);
        //Energy image, type Mat
        Mat energy_image_mat = createEnergyImage(image);
        //Energy image, type 2d 8b vector
        vector<vector<unsigned char>> energy_image_vect_2d = convert_to_vect(energy_image_mat);
        //Energy image, type 1d 8b vector
        vector<unsigned char> energy_image_vect_1d = convert_to_1d(energy_image_vect_2d, rowsize, colsize);

        // SOFT TO DDR
        ddr16.assign(energy_image_vect_1d.begin(), energy_image_vect_1d.end());

        cout<<"---------------------------> Number of pixels: "<<rowsize*colsize<<endl;
        configuration();

        //SOFT PART 
        //CEM, type 2d vector
        vector<vector<unsigned short>> cumulative_energy_map_2d = convert_to_2d(ddr16, rowsize, colsize);
        //CEM, type Mat
        Mat cumulative_energy_map_mat = convert_to_mat(cumulative_energy_map_2d);
        vector<int> path = findOptimalSeam(cumulative_energy_map_mat);
        reduce(image, path);
        cout<<"Seam "<<i+1<<" done."<<endl<<endl;
    
        // cout<<"delay after each seam: "<< offset<<endl;
    }
    namedWindow("Reduced Image", WINDOW_AUTOSIZE); imshow("Reduced Image", image); waitKey(0);
    imwrite("result.jpg", image);
}

void Soft::configuration()
{
    //sending rowsize to hard 
    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(HARD_L + HARD_ROWSIZE);
    p1.set_data_ptr((unsigned char*)&rowsize);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset);

    // sending colsize to hard
    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(HARD_L + HARD_COLSIZE);
    p1.set_data_ptr((unsigned char*)&colsize);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset);

    //sending rowsize to dma 
    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(DMA_L + DMA_ROWSIZE);
    p1.set_data_ptr((unsigned char*)&rowsize);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset);

    // sending colsize to dma
    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(DMA_L + DMA_COLSIZE);
    p1.set_data_ptr((unsigned char*)&colsize);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset);

    //sending source to dma 
    int saddr = 0;

    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(DMA_L + DMA_SADDR);
    p1.set_data_ptr((unsigned char*)&saddr);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset);

    // dma control, ide na kraj jer se ovde poziva dm()
    dma_control++;
    p1.set_command(TLM_WRITE_COMMAND);
    p1.set_address(DMA_L + DMA_CONTROL);
    p1.set_data_ptr((unsigned char*)&dma_control);
    p1.set_data_length(1);
    p1.set_response_status(TLM_INCOMPLETE_RESPONSE);

    offset += sc_core::sc_time(11*DELAY, sc_core::SC_NS);
    soft_intcon_socket->b_transport(p1, offset); 

    wait(dma_done);
}

Mat Soft::createEnergyImage(Mat& image) {

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

vector<int> Soft::findOptimalSeam(Mat& cumulative_energy_map) {

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

void Soft::reduce(Mat& image, vector<int> path) {

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
