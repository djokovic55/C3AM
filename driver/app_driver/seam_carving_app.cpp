#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;
#define START 1
#define RESET 1
#define SET 0
#define STOP 0

void write_seam_ip(const int rowsize, const int colsize, const int start);
void read_seam_ip();
void write_seam_dma(const int value, const int reset);
void read_seam_dma(vector<int> &cache);
void print_cache_content(vector<int> &cache);

int rowsize = 5;
int colsize = 7;
int start;
int ready;


int main()
{
    vector<int> cache;

    write_seam_ip(rowsize, colsize, STOP);

    // Form cache size
    for(int i = 0; i < 2*colsize; i++)
    {
        cache.push_back(0);
    }

    read_seam_ip();
    cout<<"Rowsize: "<<rowsize<<endl;
    cout<<"Colsize: "<<colsize<<endl;
    cout<<"Start: "<<start<<endl;
    cout<<"Ready: "<<ready<<endl;

    //Initialize cache    
    write_seam_dma(1, RESET);
    for (int i = 0; i < 2*colsize; i++)
    {
        write_seam_dma(i, SET);
    }

    cout<<"Before seam:"<<endl;    
    read_seam_dma(cache);
    print_cache_content(cache); 

    // Start seam process
    write_seam_ip(rowsize, colsize, START);
    write_seam_ip(rowsize, colsize, START);

    cout<<"After seam:"<<endl;    
    read_seam_dma(cache);
    print_cache_content(cache); 

    return 0;
}

void print_cache_content(vector<int> &cache)
{
    cout<<"Cache content: ";
    for(int i = 0; i < 2*colsize; i++)
    {
        cout<<cache[i]<<" "; 
    }
    cout<<endl;
}

void write_seam_ip(const int rowsize, const int colsize, const int start)
{
    FILE *seam_ip;

    seam_ip = fopen("/dev/xlnx,seam", "w");
    fprintf(seam_ip, "%d, %d, %d ", rowsize, colsize, start);
    fclose(seam_ip);
}
void read_seam_ip()
{
    FILE *seam_ip;

    seam_ip = fopen("/dev/xlnx,seam", "r");
    fscanf(seam_ip, "[READ] colsize = %d, rowsize = %d, start = %d, ready = %d\n", &colsize, &rowsize, &start, &ready);
    fclose(seam_ip);

}

void write_seam_dma(const int value, const int reset)
{

    FILE *seam_dma;

    seam_dma = fopen("/dev/xlnx,dma_seam", "w");
    fprintf(seam_dma, "%d, %d ", value, reset);
    fclose(seam_dma);
}

void read_seam_dma(vector<int> &cache)
{

    FILE *seam_dma;
    int read_value;
    // int cache_waddr = 0;
    seam_dma = fopen("/dev/xlnx,dma_seam", "r");

    for(int i = 0; i < 2*colsize; i++)
    {
        fscanf(seam_dma, "%d ", &read_value);
        cache[i] = read_value;
    }

    fclose(seam_dma);
}