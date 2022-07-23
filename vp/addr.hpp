#ifndef ADDR_HPP
#define ADDR_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>

// *****************ADDRESS SPACE************************
// register in hard

//ddr in soft
#define DDR_L 0x40000000
#define DDR_H 0x400000FF
#define MAX_ROW 2048
#define MAX_COL 2048
#define MAX_CAP MAX_ROW * MAX_COL

//hard
#define HARD_L 0x41000000
#define HARD_H 0x410000FF
// registers in hard
#define HARD_ROWSIZE 0x00
#define HARD_COLSIZE 0x01
#define HARD_CONTROL 0x02
#define HARD_CASH 0x03


// dma
#define DMA_L 0x42000000

#define DMA_H 0x420000FF

// registers in dma

#define DMA_CONTROL 0x00
#define DMA_SOURCE_ADD 0x01
#define DMA_DEST_ADD 0x02
#define DMA_COUNT 0x03
#define DMA_ROWSIZE 0x04
#define DMA_COLSIZE 0x05

#endif