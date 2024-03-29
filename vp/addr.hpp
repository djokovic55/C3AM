#ifndef ADDR_HPP
#define ADDR_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>

// *****************ADDRESS SPACE************************

//hard
#define HARD_L 0x41000000
#define HARD_H 0x410000FF

// registers in hard
#define HARD_ROWSIZE 0x00
#define HARD_COLSIZE 0x01

// dma
#define DMA_L 0x42000000
#define DMA_H 0x420000FF

// registers in dma
#define DMA_CONTROL 0x00
#define DMA_ROWSIZE 0x01
#define DMA_COLSIZE 0x02
#define DMA_SADDR 0x03

#define DELAY 5
#endif