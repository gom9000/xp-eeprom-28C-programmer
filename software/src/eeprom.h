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
 * Module.....: header file of library module eeprom.c
 * Notes......:
 */


#ifndef __EEPROM_H__
#define __EEPROM_H__


#include <stdint.h>
typedef uint32_t address_t;
typedef uint32_t length_t;
typedef uint8_t data_t;


#define BUS_SIZE(bus) (sizeof(bus)/sizeof(bus[0]))


extern uint8_t D[8]; // D0-D7
extern uint8_t A[15]; // A0-A14
extern uint8_t CE;
extern uint8_t OE;
extern uint8_t WE;


void setup(void);
void setAddress(address_t);
void setData(data_t);
data_t getData(void);
void setDataBusDirection(unsigned char);
data_t readROM(address_t);
void writeROM(address_t, data_t);
void waitForWriteCycle(address_t, data_t);
void eraseROM(address_t, address_t, data_t);
length_t testROM(address_t, length_t, data_t);
void dumpROM(address_t, length_t, FILE*);
void setSDPMode(unsigned char);


#endif /* !defined(__EEPROM_H__) */
