// MB85RS2MTA library header

// read and write functions use heap space for transfers
//  if large block transfers are required, the heap allocation in the 
//  project settings will need to be increased, or transfers will 
//  need to be reduced in size
//  using DMA transfers would mitigate this issue

#include <stdint.h>

// set write enable latch
int _MB85RS2MTA_WREN (void);

// reset write enable latch
int _MB85RS2MTA_WRDI (void);

// read status register
int _MB85RS2MTA_RDSR (void);

// write status register (requires register to be unlocked)
int _MB85RS2MTA_WRSR (int WPEN, int BP1, int BP0, int WEL);

// read count bytes of data starting at the 24-bit addr
int _MB85RS2MTA_READ (unsigned int addr, uint8_t* buff, int count);

// write count bytes of data starting at the 24-bit addr
int _MB85RS2MTA_WRITE (unsigned int addr, uint8_t* buff, int count);

// check that the device is responding with the correct ID (determined from the datasheet)
int _MB85RS2MTA_CheckDeviceID (void);