// we define Module constants
#define PHP_SPI_EXTNAME "php_spi"
#define PHP_SPI_VERSION "0.8.0"

// then we declare the function to be exported
PHP_FUNCTION(spi_open);
PHP_FUNCTION(spi_close);
PHP_FUNCTION(spi_read);
PHP_FUNCTION(spi_write);
PHP_FUNCTION(spi_transfer);

PHP_FUNCTION(spi_get_mode);
PHP_FUNCTION(spi_get_speed);
PHP_FUNCTION(spi_get_delay);
PHP_FUNCTION(spi_get_bits_per_word);
