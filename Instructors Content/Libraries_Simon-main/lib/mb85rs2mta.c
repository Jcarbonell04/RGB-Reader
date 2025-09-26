// MB85RS2MTA implementation file

#include "spi.h"
#include "mb85rs2mta.h"
#include <stdlib.h>
#include <string.h>

// set write enable latch
int _MB85RS2MTA_WREN (void)
{
  uint8_t txbuff[1] = { 0x06 }; // command
  uint8_t rxbuff[1] = { 0 };    // actual values received

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 1);

  if (transcount != 1)
    return -1;

  return 0;
}

// reset write enable latch
int _MB85RS2MTA_WRDI (void)
{
  uint8_t txbuff[1] = { 0x04 }; // command
  uint8_t rxbuff[1] = { 0 };    // actual values received

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 1);

  if (transcount != 1)
    return -1;

  return 0;
}

// read status register
int _MB85RS2MTA_RDSR (void)
{
  uint8_t txbuff[2] = { 0x05, 0x00 }; // command with bogus continuation value
  uint8_t rxbuff[2] = {0};           // actual values received

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 2);

  if (transcount != 2)
    return -1;

  return rxbuff[1];  
}

// write status register (requires register to be unlocked)
int _MB85RS2MTA_WRSR (int WPEN, int BP1, int BP0, int WEL)
{
  uint8_t regval = 0;
  if (WPEN)
    regval |= 0b10000000;
  if (BP1)
    regval |= 0b00001000;
  if (BP0)
    regval |= 0b00000100;
  if (WEL)
    regval |= 0b00000010;

  uint8_t txbuff[2] = { 0x01, regval }; // command with value
  uint8_t rxbuff[2] = {0};              // actual values received

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 2);

  if (transcount != 2)
    return -1;

  return 0;  
}

// read count bytes of data starting at the 24-bit addr
int _MB85RS2MTA_READ (unsigned int addr, uint8_t* buff, int count)
{
  // command 3, 24-bits of right-aligned data, then count bytes of data
  uint8_t addrh = (uint8_t)(addr >> 16);
  uint8_t addrm = (uint8_t)(addr >> 8);
  uint8_t addrl = (uint8_t)(addr);
  
  uint8_t* txbuff = malloc (4 + count);
  uint8_t* rxbuff = malloc (4 + count);
  txbuff[0] = 0x03;
  txbuff[1] = addrh;
  txbuff[2] = addrm;
  txbuff[3] = addrl;

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 4 + count);

  // copy down and free memory
  memcpy(buff, rxbuff + 4, count);

  free (txbuff);
  free (rxbuff);

  return transcount;  
}

// write count bytes of data starting at the 24-bit addr
int _MB85RS2MTA_WRITE (unsigned int addr, uint8_t* buff, int count)
{
  // command 2, 24-bits of right-aligned data, then count bytes of data
  uint8_t addrh = (uint8_t)(addr >> 16);
  uint8_t addrm = (uint8_t)(addr >> 8);
  uint8_t addrl = (uint8_t)(addr);
  
  uint8_t* txbuff = malloc (4 + count);
  uint8_t* rxbuff = malloc (4 + count);
  txbuff[0] = 0x02;
  txbuff[1] = addrh;
  txbuff[2] = addrm;
  txbuff[3] = addrl;

  // copy target data down
  memcpy (txbuff + 4, buff, count);

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 4 + count);

  // free memory
  free (txbuff);
  free (rxbuff);

  return transcount - 4;  
}

int _MB85RS2MTA_CheckDeviceID (void)
{
  uint8_t dsbuff[5] = { 0x00, 0x04, 0x7f, 0x48, 0x03 }; // expected values
  uint8_t txbuff[5] = { 0x9F, 0x00, 0x01, 0x02, 0x03 }; // command with bogus continuation values
  uint8_t rxbuff[50] = {0};                             // actual values received

  // perform the transaction
  int transcount = _SPI1_Transaction(txbuff, rxbuff, 5);

  // 5 bytes should have been processed
  if (transcount != 5)
    return -1;

  // do the rxed bytes match the ds expected values?
  for (int i = 0; i < 5; ++i)
  {
    if (rxbuff[i] != dsbuff[i])
      return -2;
  }

  // it's an old code, but it checks out
  return 0;
}
