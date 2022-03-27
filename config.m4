PHP_ARG_ENABLE(php_spi, Whether to enable the SPI extension, [ --enable-spi-php Enable SPI])

if test "$PHP_SPI" != "no"; then
    PHP_NEW_EXTENSION(php_spi, spi.c, $ext_shared)
fi