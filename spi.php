<?php
/**
 * MIT License
 *
 * Copyright (c) 2020 TASoft Applications
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * This file describes the extension for php editors and documentation utilities.
 * You must not include this file in your application because the extension defines this functions.
 */

/**
 * Opens the device
 * Usually "/dev/spidev0.0" or "/dev/spidev0.1"
 *
 * @param string $deviceName
 * @param int $mode
 * @param int $speed
 * @param int $bitsPerWord
 * @param int $delay
 * @return bool|resource
 */
function spi_open($deviceName, int $mode, int $speed = 500000, int $bitsPerWord = 8, int $delay = 0) {}

/**
 * @param resource $device
 * @return bool|int
 */
function spi_get_mode($device) {}

/**
 * @param resource $device
 * @return bool|int
 */
function spi_get_speed($device) {}

/**
 * @param resource $device
 * @return bool|int
 */
function spi_get_bits_per_word($device) {}

/**
 * @param resource $device
 * @return bool|int
 */
function spi_get_delay($device) {}


/**
 * Closes the device pointer
 *
 * @param resource $device
 * @return bool
 */
function spi_close($device) {}

/**
 * Reads $length bytes from spi bus
 *
 * @param resource $device
 * @param int $length
 * @return array
 */
function spi_read($device, int $length = 1) {}

/**
 * Writes the passed bytes to the spi bus
 *
 * @param resource $device
 * @param array $bytes
 * @return bool
 */
function spi_write($device, array $bytes) {}

/**
 * Writes the passed bytes to the spi bus and reads the response.
 *
 * @param resource $device
 * @param array $bytes
 * @return array
 */
function spi_transfer($device, array $bytes) {}