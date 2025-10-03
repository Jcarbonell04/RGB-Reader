// SPI Header
#include <stdint.h>

// prescale values for the SPI (user must select in INIT)
typedef enum
{
  _SPI_Prescale_fPCLK_2   = 0b000,
  _SPI_Prescale_fPCLK_4   = 0b001,
  _SPI_Prescale_fPCLK_8   = 0b010,
  _SPI_Prescale_fPCLK_16  = 0b011,
  _SPI_Prescale_fPCLK_32  = 0b100,
  _SPI_Prescale_fPCLK_64  = 0b101,
  _SPI_Prescale_fPCLK_128 = 0b110,
  _SPI_Prescale_fPCLK_256 = 0b111

} _SPI_Prescale;

// initialize SPI1 for use, assumes full-duplex, and user controls target *CS
void _SPI1_Init (_SPI_Prescale prescale);

// deselect the target device by asserting a high *CS signal (customize to your configuration)
void _SPI1_DeselectTarget (void);

// select the target device by asserting a low *CS signal (customize to your configuration)
void _SPI1_SelectTarget (void);

// perform a bidirectional data transfer
int _SPI1_Transaction (uint8_t* pTxData, uint8_t* pRxData, int ByteCount);