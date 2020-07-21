/*
 * eeprom.c
 *
 *  ___ ___ ___ ___  ___  __  __                                                     
 * | __| __| _ \ _ \/ _ \|  \/  |___ _ __ _ _ ___  __ _ _ _ __ _ _ __  _ __  ___ _ _ 
 * | _|| _||  _/   / (_) | |\/| |___| '_ \ '_/ _ \/ _` | '_/ _` | '  \| '  \/ -_) '_|
 * |___|___|_| |_|_\\___/|_|  |_|   | .__/_| \___/\__, |_| \__,_|_|_|_|_|_|_\___|_|  
 *                                  |_|           |___/                              
 *
 * Author.....: Alessandro Fraschetti (mail: gos95@gommagomma.net)
 * Target.....: RaspberryPI
 * Version....: 1.0 2020/07/06
 * Description: EEPROM 28C-family programmer utility
 * URL........: https://github.com/gom9000/xp-eeprom-28C-programmer
 * License....: this program is under the terms of MIT License
 * Module.....: eeprom operations library
 * Notes......: ADDR BUS : A0-A14 = 8,9,7  0,2,3  12,13,14  30,21,22,23,24,25 (wiringPi pin numbers)
 *              DATA BUS : D0-D7  = 15,16,1  4,5  6,10,11
 *              CTRL BUS : CE=27, OE=28, WE=29
 *
 */


#include <stdio.h>
#include <wiringPi.h>
#include "eeprom.h"


uint8_t D[] = {15,16,1,4,5,6,10,11}; // D0-D7
uint8_t A[] = {8,9,7,0,2,3,12,13,14,30,21,22,23,24,25}; //A0-A14
uint8_t CE = 27;
uint8_t OE = 28;
uint8_t WE = 29;


void setup()
{
    for (unsigned char ii=0; ii<BUS_SIZE(D); ii++)
    {
        pinMode(A[ii], OUTPUT);
        digitalWrite(A[ii], LOW);
    }

    setDataBusDirection(INPUT);

    pinMode(CE, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(WE, OUTPUT);

    digitalWrite(CE, HIGH);
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);
}

void setAddress(address_t address)
{
    for (unsigned char ii=0; ii<BUS_SIZE(A); ii++)
    {
        digitalWrite(A[ii], address & 1);
        address >>= 1;
    }
}

void setData(data_t data)
{
    for (unsigned char ii=0; ii<BUS_SIZE(D); ii++)
    {
        digitalWrite(D[ii], data & 1);
        data >>= 1;
    }
}

data_t getData()
{
    data_t data = 0;

    for (unsigned char ii=BUS_SIZE(D); ii>0; ii--)
        data = (data << 1) | digitalRead(D[ii-1]);

    return data;
}

void setDataBusDirection(unsigned char dir)
{
    for (unsigned char ii=0; ii<BUS_SIZE(D); ii++)
        pinMode(D[ii], dir);
}

data_t readROM(address_t address)
{
    data_t data = 0;
    setDataBusDirection(INPUT);
    setAddress(address);
    digitalWrite(CE, LOW);
    digitalWrite(OE, LOW);
    delayMicroseconds(1);
    data = getData();
    digitalWrite(OE, HIGH);
    digitalWrite(CE, HIGH);

    return data;
}

void writeROM(address_t address, data_t data)
{
    setAddress(address);
    setDataBusDirection(OUTPUT);
    setData(data);
    digitalWrite(CE, LOW);
    digitalWrite(WE, LOW);
    delayMicroseconds(1);
    digitalWrite(WE, HIGH);
    digitalWrite(CE, HIGH);
}

void waitForWriteCycle(address_t address, data_t data)
{
//    while (readROM(address)&0x80 != data&0x80) printf("waiting... %d  %d\n", address, data);
    delay(10);
}

void eraseROM(address_t start, length_t length, data_t blank)
{
    for (address_t address=start; address<start+length; address++)
    {
       writeROM(address, blank);
       waitForWriteCycle(address, blank);
    }
}

length_t testROM(address_t start, length_t length, data_t data)
{
    length_t count = 0;
    for (address_t address=start; address<start+length; address++)
       if (readROM(address) != data) count++;;

    return count;
}

void dumpROM(address_t start, length_t length, FILE *outstream)
{
    if (outstream == stdout && start%16)
    {
        printf("%.5X:  ", start-start%16);
        for (unsigned char ii=0; ii<start%16; ii++)
            printf("   ");
    }

    for (address_t address=start; address<start+length; address++)
    {
        if (outstream == stdout && !(address%16)) printf("%.5X:  ", address);
        fprintf(outstream, "%.2X", readROM(address));
        if (outstream == stdout) printf(" ");
        if (address%16 == 15) fprintf(outstream, "\r\n");
    }
    if (outstream == stdout && (start+length)%16) printf("\n");
}

void setSDPMode(unsigned char mode)
{
    if (mode)
    {
        writeROM(0x5555, 0xAA);
        writeROM(0x2AAA, 0x55);
        writeROM(0x5555, 0xA0);
    } else {
        writeROM(0x5555, 0xAA);
        writeROM(0x2AAA, 0x55);
        writeROM(0x5555, 0x80);
        writeROM(0x5555, 0xAA);
        writeROM(0x2AAA, 0x55);
        writeROM(0x5555, 0x20);
    } 
}
