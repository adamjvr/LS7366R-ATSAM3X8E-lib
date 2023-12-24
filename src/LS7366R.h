#ifndef LS7366R_H
#define LS7366R_H

#include <asf.h>

// Define pins and SPI configuration for LS7366R
#define LS7366R_CS_PIN          AVR32_PIN_PB21  // Chip Select (CS2) Pin
#define LS7366R_SPI             &AVR32_SPI0      // SPI port with MISO on PA25, SCLK on PA27, MOSI on PA26
#define LS7366R_SPI_CS          2                // Chip Select line on CS2

// LS7366R Commands
#define LS7366R_READ_COUNTER    0x60
#define LS7366R_CLEAR_COUNTER   0x20

// Function prototypes
void ls7366r_init(void);
void ls7366r_send_command(uint8_t command);
uint32_t ls7366r_read_counter(void);
void ls7366r_clear_counter(void);

#endif /* LS7366R_H */
