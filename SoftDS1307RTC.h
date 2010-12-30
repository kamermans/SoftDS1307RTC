/*
 * SoftDS1307RTC.h - library for DS1307 RTC using Software I2c
 * This library is intended to be uses with Arduino Time.h library functions
 */

#ifndef DS1307RTC_h
#define DS1307RTC_h

#include "../SoftI2cMaster/TwoWireBase.h"
#include "../SoftI2cMaster/SoftI2cMaster.h"
#include <Time.h>

// library interface description
class SoftDS1307RTC {
  // user-accessible "public" interface
  public:
    SoftDS1307RTC();
    static time_t get();
	static void set(time_t t);
	static void read(tmElements_t &tm);
	static void write(tmElements_t &tm);
  private:
	static SoftI2cMaster _i2c;
	static uint8_t dec2bcd(uint8_t num);
    static uint8_t bcd2dec(uint8_t num);
};

extern SoftDS1307RTC RTC;

#endif
 

