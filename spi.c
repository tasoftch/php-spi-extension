// include the PHP API itself
#include <php.h>
// then include the header of your extension
#include "spi.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// register our function to the PHP API
// so that PHP knows, which functions are in this module
zend_function_entry spi_php_functions[] = {
    PHP_FE(spi_open, NULL)
    PHP_FE(spi_close, NULL)
    PHP_FE(spi_read, NULL)
    PHP_FE(spi_write, NULL)
    PHP_FE(spi_transfer, NULL)

	PHP_FE(spi_get_mode, NULL)
	PHP_FE(spi_get_speed, NULL)
	PHP_FE(spi_get_delay, NULL)
	PHP_FE(spi_get_bits_per_word, NULL)

    {NULL, NULL, NULL}
};

// some pieces of information about our module
zend_module_entry spi_php_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_SPI_EXTNAME,
    spi_php_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_SPI_VERSION,
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(spi_php)

struct spi_bus {
	int fd;
	int state;
    uint32_t mode;
    uint32_t speed;
    uint8_t bits;
    uint16_t delay;
};

// The resource management
static void _spi_resource_destructor(zend_resource *rsrc)
{
	struct spi_bus fd = *((struct spi_bus *)rsrc->ptr);
    if(fd.state > 0)
    	close(fd.fd);

    free(rsrc->ptr);
}

PHP_FUNCTION(spi_open) {
    char *devPath = NULL;
    uint32_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 500000;
    uint16_t delay = 0;
    
    size_t length = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|lll", &devPath, &length, &mode, &speed, &bits, &delay) != SUCCESS) {
        RETURN_BOOL(0);
      }

	int fd = open(devPath, O_RDWR);
	if (fd < 0) {
		RETURN_BOOL(0);
	}

	struct spi_bus *_fd = calloc(sizeof(struct spi_bus), 1);
	_fd->fd = fd;
	_fd->state = 1;
    
    
    int ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set mode");
        close(fd);
        RETURN_BOOL(0);
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set mode");
        close(fd);
        RETURN_BOOL(0);
    }
    _fd->mode = mode;
    
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set speed");
        close(fd);
        RETURN_BOOL(0);
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set speed");
        close(fd);
        RETURN_BOOL(0);
    }
    _fd->speed = speed;
    
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set bits per word");
        close(fd);
        RETURN_BOOL(0);
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) {
        zend_error(E_WARNING, "Can not set bits per word");
        close(fd);
        RETURN_BOOL(0);
    }
    _fd->bits = bits;
    _fd->delay = delay;

	int res_num = zend_register_list_destructors_ex(_spi_resource_destructor, NULL, "spi_dev", 0x01);
	zend_resource *my_res  = zend_register_resource((void *)_fd, res_num);
	RETURN_RES(my_res);
}


PHP_FUNCTION(spi_get_mode) {
    zval *rval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &rval) != SUCCESS) {
       RETURN_BOOL(0);
    }
    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    
    RETURN_LONG(fd.mode);
}

PHP_FUNCTION(spi_get_speed) {
    zval *rval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &rval) != SUCCESS) {
       RETURN_BOOL(0);
    }
    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    
    RETURN_LONG(fd.speed);
}

PHP_FUNCTION(spi_get_bits_per_word) {
    zval *rval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &rval) != SUCCESS) {
       RETURN_BOOL(0);
    }
    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    
    RETURN_LONG(fd.bits);
}

PHP_FUNCTION(spi_get_delay) {
    zval *rval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &rval) != SUCCESS) {
       RETURN_BOOL(0);
    }
    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    
    RETURN_LONG(fd.delay);
}

// Finally, we implement our "Hello World" function
// this function will be made available to PHP
// and prints to PHP stdout using printf
PHP_FUNCTION(spi_close) {
    zval *rval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &rval) != SUCCESS) {
       RETURN_BOOL(0);
    }
	zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    if(fd.state > 0)
    	close(fd.fd);
    fd.state = 0;
    RETURN_BOOL(1);
}


int transfer(struct spi_bus bus, char *tx, char *rx, int len) {
    int ret;
    int out_fd;
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .delay_usecs = bus.delay,
        .speed_hz = bus.speed,
        .bits_per_word = bus.speed,
    };
    
    ret = ioctl(bus.fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        zend_error(E_WARNING, "Device was already closed");
        return 0;
    }
    return 1;
}


PHP_FUNCTION(spi_read) {
    zval *rval;
    long length = 1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|l", &rval, &length) != SUCCESS) {
       RETURN_BOOL(0);
    }


    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);
    if(fd.state < 1) {
        zend_error(E_WARNING, "Device was already closed");
        RETURN_BOOL(0);
    }
    
    unsigned char tx[length];
    unsigned char rx[length];
    
    if(!transfer(fd, tx, rx, length)) {
        zend_error(E_WARNING, "Could not transfer data");
        RETURN_BOOL(0);
    }

    zend_array *ret = zend_new_array( length );
    
    for(int e=0;e<length;e++) {
        add_next_index_long(ret, rx[e]);
    }

    RETVAL_ARR(ret);
}


PHP_FUNCTION(spi_write) {
    zval *rval, *bytes = NULL;
	HashTable *arr_hash;
	int array_count;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ra", &rval, &bytes) != SUCCESS) {
	   RETURN_BOOL(0);
	}
	zend_resource *resource = Z_REF_P(rval);
	struct spi_bus fd = *((struct spi_bus *)resource->ptr);

    if(fd.state < 1) {
        zend_error(E_WARNING, "Device was already closed");
        RETURN_BOOL(0);
    }

	if(bytes) {
		arr_hash = Z_ARRVAL_P(bytes);
		array_count = zend_hash_num_elements(arr_hash);
	} else
		array_count = 0;

	uint8_t buffer[array_count];
    int bufCount = 0;

	if(bytes) {
		for(int idx = 0;idx < array_count;idx++) {
			zval *data = data = zend_hash_index_find(arr_hash, idx);
			if (Z_TYPE_P(data) == IS_LONG) {
				long value = Z_LVAL_P(data);
				if(value >= 0 && value <=255) {
					buffer[bufCount++] = value;
				} else {
					zend_error(E_WARNING, "spi_write expects an array containing integer values between 0 and 255");
					RETURN_BOOL(0);
				}
			} else {
				zend_error(E_WARNING, "spi_write expects an array containing integer values between 0 and 255");
				RETURN_BOOL(0);
			}
		}
	}

    uint8_t rx[array_count];
    
    if(!transfer(fd, buffer, rx, array_count)) {
        zend_error(E_WARNING, "Could not transfer data");
        RETURN_BOOL(0);
    }

    RETURN_TRUE;
}

PHP_FUNCTION(spi_transfer) {
    zval *rval, *bytes = NULL;
    HashTable *arr_hash;
    int array_count;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|a", &rval, &bytes) != SUCCESS) {
       RETURN_BOOL(0);
    }
    zend_resource *resource = Z_REF_P(rval);
    struct spi_bus fd = *((struct spi_bus *)resource->ptr);

    if(fd.state < 1) {
        zend_error(E_WARNING, "Device was already closed");
        RETURN_BOOL(0);
    }

    if(bytes) {
        arr_hash = Z_ARRVAL_P(bytes);
        array_count = zend_hash_num_elements(arr_hash);
    } else
        array_count = 0;

    uint8_t buffer[array_count];
    int bufCount = 0;

    if(bytes) {
        for(int idx = 0;idx < array_count;idx++) {
            zval *data = data = zend_hash_index_find(arr_hash, idx);
            if (Z_TYPE_P(data) == IS_LONG) {
                long value = Z_LVAL_P(data);
                if(value >= 0 && value <=255) {
                    buffer[bufCount++] = value;
                } else {
                    zend_error(E_WARNING, "spi_write expects an array containing integer values between 0 and 255");
                    RETURN_BOOL(0);
                }
            } else {
                zend_error(E_WARNING, "spi_write expects an array containing integer values between 0 and 255");
                RETURN_BOOL(0);
            }
        }
    }

    uint8_t rx[array_count];
    
    if(!transfer(fd, buffer, rx, array_count)) {
        zend_error(E_WARNING, "Could not transfer data");
        RETURN_BOOL(0);
    }
    
    zend_array *ret = zend_new_array( array_count );

        for(int e=0;e<array_count;e++) {
            add_next_index_long(ret, rx[e]);
        }

        RETVAL_ARR(ret);
}
