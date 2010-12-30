/*
 * SoftDS1307RTC.cpp - library for DS1307 RTC using Software I2c
  
  Software I2c based DS1307 RTC library by Steve Kamerman 2010
  Based almost entirely on Michael Margolis' 12/30/2009 library "DS1307RTC"

  Copyright (c) Steve Kamerman 2010
  This library is intended to be uses with Arduino Time.h library functions

  The library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  30 Dec 2010 - Modified by Steve Kamerman to support SoftI2cMaster
 */
#include "../SoftI2cMaster/TwoWireBase.h"
#include "../SoftI2cMaster/SoftI2cMaster.h"
#include "SoftDS1307RTC.h"

#define DS1307_CTRL_ID 0xD0

#ifndef SCL_PIN
#define SCL_PIN 11
#endif

#ifndef SDA_PIN
#define SDA_PIN 10
#endif

SoftI2cMaster SoftDS1307RTC::_i2c;

SoftDS1307RTC::SoftDS1307RTC()
{
  _i2c.init(SCL_PIN, SDA_PIN);
}
  
// PUBLIC FUNCTIONS
time_t SoftDS1307RTC::get()   // Aquire data from buffer and convert to time_t
{
  tmElements_t tm;
  read(tm);
  return(makeTime(tm));
}

void  SoftDS1307RTC::set(time_t t)
{
  tmElements_t tm;
  breakTime(t, tm);
  tm.Second |= 0x80;  // stop the clock
  write(tm); 
  tm.Second &= 0x7f;  // start the clock
  write(tm); 
}

// Aquire data from the RTC chip in BCD format
void SoftDS1307RTC::read( tmElements_t &tm)
{
  _i2c.start(DS1307_CTRL_ID | I2C_WRITE);
  _i2c.write(0x00);
  _i2c.restart(DS1307_CTRL_ID | I2C_READ);
  
  tm.Second = bcd2dec(_i2c.read(false));
  tm.Minute = bcd2dec(_i2c.read(false));
  tm.Hour =   bcd2dec(_i2c.read(false));
  tm.Wday = bcd2dec(_i2c.read(false));
  tm.Day = bcd2dec(_i2c.read(false));
  tm.Month = bcd2dec(_i2c.read(false));
  tm.Year = y2kYearToTm((bcd2dec(_i2c.read(true))));

  _i2c.stop();
}

void SoftDS1307RTC::write(tmElements_t &tm)
{
  _i2c.start(DS1307_CTRL_ID | I2C_WRITE);
  _i2c.write(0x00);

  _i2c.write(dec2bcd(tm.Second));
  _i2c.write(dec2bcd(tm.Minute));
  _i2c.write(dec2bcd(tm.Hour));
  _i2c.write(dec2bcd(tm.Wday));
  _i2c.write(dec2bcd(tm.Day));
  _i2c.write(dec2bcd(tm.Month));
  _i2c.write(dec2bcd(tmYearToY2k(tm.Year)));

  _i2c.stop();
}
// PRIVATE FUNCTIONS

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t SoftDS1307RTC::dec2bcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t SoftDS1307RTC::bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}
SoftDS1307RTC RTC = SoftDS1307RTC(); // create an instance for the user

