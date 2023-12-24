#include "ls7366r.h"

// Function to configure SPI for LS7366R communication
static void configure_spi(void) {
    // Configure Chip Select (CS2) pin
    gpio_configure_pin(LS7366R_CS_PIN, GPIO_DIR_OUT | GPIO_INIT_HIGH);

    // Initialize and configure SPI with MISO on PA25, SCLK on PA27, MOSI on PA26
    spi_master_init(LS7366R_SPI);

    // Setup SPI device on CS2 with master mode, 1 Mbps baudrate, 8 bits per transfer
    spi_master_setup_device(LS7366R_SPI, NULL, AVR32_SPI_MR_MSTR_Msk, 1000000, LS7366R_SPI_CS);

    // Enable SPI
    spi_enable(LS7366R_SPI);
}

// Function to transfer a byte over SPI
uint8_t spi_transfer(uint8_t data) {
    // Write data to SPI and wait until the transfer is complete
    spi_write_single(LS7366R_SPI, data);
    while (!(spi_read_status(LS7366R_SPI) & AVR32_SPI_SR_RDRF_MASK));
    
    // Read and return the received byte
    return spi_read_single(LS7366R_SPI);
}

// Function to send a command to LS7366R
void ls7366r_send_command(uint8_t command) {
    // Select LS7366R by lowering Chip Select (CS2) pin
    gpio_clr_gpio_pin(LS7366R_CS_PIN);

    // Send the specified command over SPI
    spi_transfer(command);

    // Deselect LS7366R by raising Chip Select (CS2) pin
    gpio_set_gpio_pin(LS7366R_CS_PIN);
}

// Function to read the counter value from LS7366R
uint32_t ls7366r_read_counter(void) {
    uint32_t count = 0;

    // Select LS7366R by lowering Chip Select (CS2) pin
    gpio_clr_gpio_pin(LS7366R_CS_PIN);

    // Send the Read Counter command
    spi_transfer(LS7366R_READ_COUNTER);

    // Read the counter value (32 bits) over SPI
    count |= (uint32_t)spi_transfer(0) << 24;
    count |= (uint32_t)spi_transfer(0) << 16;
    count |= (uint32_t)spi_transfer(0) << 8;
    count |= (uint32_t)spi_transfer(0);

    // Deselect LS7366R by raising Chip Select (CS2) pin
    gpio_set_gpio_pin(LS7366R_CS_PIN);

    // Return the counter value
    return count;
}

// Function to clear the counter in LS7366R
void ls7366r_clear_counter(void) {
    // Send the Clear Counter command
    ls7366r_send_command(LS7366R_CLEAR_COUNTER);
}

// Function to initialize LS7366R communication
void ls7366r_init(void) {
    // Initialize the board and system clock
    board_init();
    sysclk_init();

    // Configure SPI for LS7366R communication
    configure_spi();
}
