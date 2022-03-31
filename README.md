# php spi extension
I've created this extension to get access to the spi bus on my raspberry pi.

### Installation
```bin
$ cd ~
$ git clone https://github.com/tasoftch/php-spi-extension.git
$ cd php-spi-extension
$ phpize
$ ./configure --enable-php-spi
$ make
$ sudo make install
```
This compiles the source on your machine.  
Next find the php.ini file
```bin
$ php --ini
```
Will list scanned ini files.  
Add the following line to that php.ini file:
```extension=php_spi```
```php
<?php
var_dump( extension_loaded('php_spi') ); // Should be true
```

### Usage
The extension adds the following functions to the global scope:
1. ```spi_open```  
    This opens the device bus.
1. ```spi_read```  
    Reads data from the spi bus.
1. ```spi_write```  
    Writes data to the spi bus  
1. ```spi_transfer```  
       Reads and writes data to and from the spi bus in duplex mode
1. ```spi_close```  
    Closes the bus.
1.  ```spi_get_mode```  
    Reads the mode
1.  ```spi_get_speed```  
    Reads the maximum speed
1.  ```spi_get_bits_per_word```  
    Reads bits per word
1.  ```spi_get_delay```  
    Reads the delay
    
### Example
```php
<?php
use TASoft\Bus\SPI;

$dev = new SPI(0); // /dev/spidev0.0
$backData = $dev->transfer([0x0, 0xFF]);
print_r($backData);
?>
```