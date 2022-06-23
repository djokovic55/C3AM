#ifndef ADDR_HPP
#define ADDR_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>

// *****************ADDRESS SPACE************************
// register in hard

//hard
#define HARD_L 0x41000000
#define HARD_H 0x410000FF
// dma
#define DMA_L 0x42000000
#define DMA_H 0x420000FF

// registers in dma
#define DMA_CONTROL 0x00
#define DMA_SOURCE_ADD 0x01
#define DMA_DEST_ADD 0x02
#define DMA_COUNT 0x03

#endif