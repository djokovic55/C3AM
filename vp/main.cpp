#include "hard.hpp"
#include "soft.hpp"
#include "utils.hpp"
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