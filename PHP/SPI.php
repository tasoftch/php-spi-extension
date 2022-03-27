<?php

namespace TASoft\Bus;

use RuntimeException;

/**
 * Class I2C wraps the functions i2c_* into a class
 * @package TASoft\Bus
 */
class SPI
{
	/** @var resource */
	private $deviceStream;

	const SPI_CPHA =		0x01;
	const SPI_CPOL =		0x02;

	const SPI_MODE_0 =		(0|0);
	const SPI_MODE_1 =		(0 | self::SPI_CPHA);
	const SPI_MODE_2 =		(self::SPI_CPOL | 0);
	const SPI_MODE_3 =		(self::SPI_CPOL | self::SPI_CPHA);

	const SPI_CS_HIGH =		0x04;
	const SPI_LSB_FIRST =	0x08;
	const SPI_3WIRE =		0x10;
	const SPI_LOOP =		0x20;
	const SPI_NO_CS =		0x40;

	/**
	 * SPI constructor.
	 * @param int $bus
	 * @param int $mode
	 * @param int $speed
	 * @param int $bitsPerWord
	 * @param int $delay
	 */
	public function __construct(int $bus, $mode = self::SPI_MODE_0, int $speed = 500000, int $bitsPerWord = 8, int $delay = 0)
	{
		$this->deviceStream = spi_open("/dev/spidev0.$bus", $mode, $speed, $bitsPerWord, $delay);
		if(!$this->deviceStream)
			throw new RuntimeException("Could not open i2c bus");
	}

	/**
	 * Closes the i2c bus.
	 */
	public function close() {
		if($this->deviceStream)
			spi_close($this->deviceStream);
		$this->deviceStream = NULL;
	}

	/**
	 * @inheritDoc
	 */
	public function __destruct()
	{
		$this->close();
	}

	protected function checkDev() {
		if(!$this->deviceStream)
			throw new RuntimeException("Can not read/write to spi bus. Probably closed or not opened correctly");
	}

	/**
	 * Reads a specific length of bytes from bus
	 *
	 * @param $length
	 * @return array
	 */
	public function read($length) {
		$this->checkDev();
		return spi_read($this->deviceStream, $length);
	}

	/**
	 * Reads one single byte from spi bus
	 *
	 * @return int
	 */
	public function readByte() {
		$this->checkDev();
		return spi_read($this->deviceStream, 1) [0] ?? 0;
	}

	/**
	 * Reads 2 bytes or 16 bit from spi bus
	 *
	 * @return int
	 */
	public function read2Bytes() {
		$this->checkDev();
		@ list($b1, $b2) = spi_read($this->deviceStream, 2);
		return ($b1 << 8) | $b2;
	}

	/**
	 * Reads 4 bytes or 32 bits from spi bus
	 *
	 * @return int
	 */
	public function read4Bytes() {
		$this->checkDev();
		@ list($b1, $b2, $b3, $b4) = spi_read($this->deviceStream, 4);
		return ($b1 << 24) | ($b2 << 16) | ($b3 << 8) | $b4;
	}

	/**
	 * Writes all bytes from an array into the specified register on the connected chip.
	 *
	 * @param array $bytes
	 * @return bool
	 */
	public function write(array $bytes) {
		$this->checkDev();
		return spi_write($this->deviceStream, $bytes);
	}

	/**
	 * Writes 2 bytes to the specified register on the connected chip
	 *
	 * @param int $bit16
	 * @return bool
	 */
	public function write16(int $bit16) {
		return spi_write($this->deviceStream, [($bit16>>8) & 0xFF, $bit16 & 0xFF]);
	}

	/**
	 * Writes 4 bytes to the specified register on the connected chip.
	 *
	 * @param int $bit32
	 * @return bool
	 */
	public function write32(int $bit32) {
		return spi_write($this->deviceStream, [($bit32>>24) & 0xFF, ($bit32>>16) & 0xFF, ($bit32>>8) & 0xFF, $bit32 & 0xFF]);
	}

	/**
	 * Transfers data synchronously between the master and the chip.
	 *
	 * @param array $data
	 * @return array
	 */
	public function transfer(array $data): array {
		return spi_transfer($this->deviceStream, $data);
	}

	/**
	 * Interprets a given integer as signed integer using a specific bit depth
	 *
	 * @param int $integer
	 * @param int $bits
	 * @return int
	 */
	public static function convertToSignedInteger(int $integer, int $bits = 16): int {
		$div = 1<<($bits-1);
		$mask = $div-1;
		return ($integer > $mask) ? -$div+($integer&$mask) : $integer;
	}
}