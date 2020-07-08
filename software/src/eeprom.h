/*
 * eeprom.h
 *
 *  ___ ___ ___ ___  ___  __  __                                                     
 * | __| __| _ \ _ \/ _ \|  \/  |___ _ __ _ _ ___  __ _ _ _ __ _ _ __  _ __  ___ _ _ 
 * | _|| _||  _/   / (_) | |\/| |___| '_ \ '_/ _ \/ _` | '_/ _` | '  \| '  \/ -_) '_|
 * |___|___|_| |_|_\\___/|_|  |_|   | .__/_| \___/\__, |_| \__,_|_|_|_|_|_|_\___|_|  
 *                                  |_|           |___/                              
 *
 * Author.....: Alessandro Fraschetti (mail: gos95@gommagomma.net)
 * Target.....: RaspberryPI
 * Version....: 1.1 2020/06/06
 * Description: EEPROM 28C-family programmer utility
 * URL........: https://github.com/gom9000/xp-eeprom-28C-programmer
 * License....: this program is under the terms of MIT License
 * Module.....: header file
 * Notes......: ADDR BUS : A0-A14 = 8,9,7  0,2,3  12,13,14  30,21,22,23,24,25 (wiringPi pin numbers)
 *              DATA BUS : D0-D7  = 15,16,1  4,5  6,10,11
 *              CTRL BUS : CE=27, OE=28, WE=29
 *
 */


#ifndef __EEPROM_H__
#define __EEPROM_H__


#define BUS_SIZE(bus) (sizeof(bus)/sizeof(bus[0]))


void setup(void);
void setAddress(uint32_t);
void setData(uint8_t);
uint8_t getData(void);
void setDataBusDirection(uint8_t);
uint8_t readROM(uint32_t);
void writeROM(uint32_t, uint8_t);
void waitForWriteCycle(uint32_t, uint8_t);
void eraseROM(uint32_t, uint32_t, uint8_t);
int testROM(uint32_t, uint32_t, uint8_t);
void dumpROM(uint32_t, uint32_t, FILE*);
void setSDPMode(uint8_t);


#endif /* !defined(__EEPROM_H__) */
