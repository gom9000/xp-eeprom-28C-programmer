/*
 * eeprog28.c
 *
 *  ___ ___ ___ ___  ___  __  __                                                     
 * | __| __| _ \ _ \/ _ \|  \/  |___ _ __ _ _ ___  __ _ _ _ __ _ _ __  _ __  ___ _ _ 
 * | _|| _||  _/   / (_) | |\/| |___| '_ \ '_/ _ \/ _` | '_/ _` | '  \| '  \/ -_) '_|
 * |___|___|_| |_|_\\___/|_|  |_|   | .__/_| \___/\__, |_| \__,_|_|_|_|_|_|_\___|_|  
 *                                  |_|           |___/                              
 *
 * Author.....: Alessandro Fraschetti (mail: gos95@gommagomma.net)
 * Target.....: RaspberryPI
 * Version....: 1.0 2020/06/28
 * Description: EEPROM 28C-family programmer utility
 * URL........: https://github.com/gom9000/xp-eeprom-28C-programmer
 * License....: this program is under the terms of MIT License
 * Notes......: ADDR BUS : A0-A14 = 8,9,7  0,2,3  12,13,14  30,21,22,23,24,25 (wiringPi pin numbers)
 *              DATA BUS : D0-D7  = 15,16,1  4,5  6,10,11
 *              CTRL BUS : CE=27, OE=28, WE=29
 *
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "eeprom.h"


int main(int argc, char **argv)
{
    int c;
    unsigned char t_flag = 0;
    unsigned char d_flag = 0;
    unsigned char e_flag = 0;
    unsigned char w_flag = 0;
    unsigned char v_flag = 0;
    unsigned char l_flag = 0;
    unsigned char a_flag = 0;
    unsigned char p_flag = 0;
    unsigned int start = 0;
    unsigned int length = 0;
    unsigned char mode = 0;
    unsigned int zerobyte = 0xFF;
    char *filename = NULL;
    FILE *fp = stdout;


    wiringPiSetup();
    setup();

    while ((c = getopt(argc, argv, "s:f:z:t:d:e:w::v::l:aph")) != -1)
    {
        switch (c)
        {
            case 's':
                sscanf(optarg, "%5X", &start);
                break;
            case 'f':
                filename = optarg;
                break;
            case 'z':
                sscanf(optarg, "%2X", &zerobyte);
                break;
            case 't':
                t_flag = 1;
                sscanf(optarg, "%5X", &length);
                break;
            case 'd':
                d_flag = 1;
                sscanf(optarg, "%5X", &length);
                break;
            case 'e':
                e_flag = 1;
                sscanf(optarg, "%5X", &length);
                break;
            case 'w':
                w_flag = 1;
                if (optarg) sscanf(optarg, "%5X", &length);
                break;
            case 'v':
                v_flag = 1;
                if (optarg) sscanf(optarg, "%5X", &length);
                break;
            case 'l':
                l_flag = 1;
                if (optarg) sscanf(optarg, "%c", &mode);
                break;
            case 'a':
                a_flag = 1;
                break;
            case 'p':
                p_flag = 1;
                break;
            case 'h':
                printf("usage: %s option [params...]\n", argv[0]);
                printf("EEPROM 28C programmer utility\n");
                printf("  options:\n");
                printf("    -t: <LENGTH> [-s <START>] [-z <BLANK>]\n");
                printf("    \ttest if rom is filled with BLANK data values\n");
                printf("    -d: <LENGTH> [-s <START>] [-z <BLANK>] [-f <DATAFILE>]\n");
                printf("    \tdump LENGTH bytes of rom, starting from START address, into DATAFILE\n");
                printf("    -e: <LENGTH> [-s <START>]\n");
                printf("    \terase LENGTH bytes (filled with BLANK values) of rom, starting from START address\n");
                printf("    -v: [<LENGTH>] [-s <START>] [-z <BLANK>] [-f <DATAFILE>]\n");
                printf("    \tverify LENGTH bytes of rom, starting from START address, with the contents of DATAFILE\n");
                printf("    \tif LENGTH is not specified or LENGTH is greater than DATAFILE length, the latter is used.\n");
                printf("    -w: [<LENGTH>] [-s <START>] [-z <BLANK>] [-f <DATAFILE>]\n");
                printf("    \twrite LENGTH bytes of rom, starting from START address, with the contents of DATAFILE\n");
                printf("    \tif LENGTH is not specified, DATAFILE length is used. If LENGTH is greter than DATAFILE length, diff is filled with BLANK values\n");
                printf("    -l: <MODE>\n");
                printf("    \tenable (MODE=1) or disable (MODE=0) rom Software Data Protection\n");
                printf("    -h: show this help and exit\n");
                printf("  params:\n");
                printf("    -s: <START> set the start hex address, default value is 0x0\n");
                printf("    -f: <DATAFILE> set the datafile where to dump/read hex values, default value for dump option is stdout\n");
                printf("    -z: <BLANK> set the blank hex data value to fill/test the rom, default value is 0xFF\n");
                printf("    -a: set the mode of the datafile as ascii (default mode is binary)\n");
                printf("    -p: show the value of input params and bus pin numbers\n");
                return 0;
            case '?':
                if (optopt == 'w' || optopt == 'v')
                    fprintf(stderr, "Option -%c requires valid filename as param.\n", optopt);
                else if (optopt == 't' || optopt == 'd' || optopt == 'e')
                    fprintf(stderr, "Option -%c requires ROM size as param.\n", optopt);
                else if (isprint (optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }


    if (p_flag)
    {
        printf("start-address=%.5X, data-length=%.5X, zero-byte=%.2X, data-file=%s, data-file-mode=%s, SDP=%d\n", start, length, zerobyte, filename, (a_flag? "ascii":"binary"), mode);
        printf("address bus pins: ");
        for (unsigned char ii=0; ii<BUS_SIZE(A); ii++) printf("%d ", A[ii]);
        printf("\ndata bus pins: ");
        for (unsigned char ii=0; ii<BUS_SIZE(D); ii++) printf("%d ", D[ii]);
        printf("\nWE pin: %d, CE pin: %d, OE pin: %d\n", WE, CE, OE);
    }
    if (t_flag)
    {
        length_t count;
        if ((count = testROM(start, length, zerobyte)))
            printf("ROM is not empty, found %d data bytes\n", count);
        else
            printf("ROM is empty\n");
        return 0; 
    }
    if (d_flag)
    {
        if (filename) fp = fopen(filename, "w");
        printf("Dumping of 0x%X locations of ROM from start 0x%X...\n", length, start);
        dumpROM(start, length, fp);
        if (filename) fclose(fp);
        printf("ROM dumped.\n");
        return 0;
    }
    if (e_flag)
    {
        printf("Erasing (0x%X) of 0x%X locations of ROM from start 0x%X...\n", zerobyte, length, start);
        eraseROM(start, length, zerobyte);
        printf("ROM erased.\n");
        return 0;
    }
    if (w_flag && !a_flag)
    {
        data_t data = 0;
        address_t address = 0;
        if (filename) fp = fopen(filename, "rb");
        printf("Writing ROM with contents of binary file: %s from start 0x%X...\n", filename, start);
        while (!feof(fp) && address<length)
        {
            fread(&data, sizeof(data_t), 1, fp); 
            writeROM(start+address, data);
            waitForWriteCycle(start+address, data);
            address++;
        }
        if (filename) fclose(fp);
        if (address<length)
            eraseROM(start+address, length-address, zerobyte);
        printf("0x%X locations of ROM wrote.\n", address);
        return 0;
    }
    if (w_flag && a_flag)
    {
        unsigned int data = 0;
        address_t address = 0;
        if (filename) fp = fopen(filename, "r");
        printf("Writing ROM with contents of ascii file: %s from start 0x%X...\n", filename, start);
        while (fscanf(fp, "%2X", &data) == 1 && address<length)
        {
            writeROM(start+address, data);
            waitForWriteCycle(start+address, data);
            address++;
        }
        if (filename) fclose(fp);
        if (address<length)
            eraseROM(start+address, length-address, zerobyte);
        printf("0x%X locations of ROM wrote.\n", address);
        return 0;
    }
    if (v_flag && !a_flag)
    {
        address_t address = 0;
        data_t data = 0;
        data_t actual = 0;
        char result = 1;
        if (filename) fp = fopen(filename, "rb");
        if (!length) length = -1;
        printf("Verifying ROM with contents of binary file: %s from start 0x%X...\n", filename, start);
        while (!feof(fp) && address-length)
        {
            result = 0;
            fread(&data, sizeof(data_t), 1, fp);
            actual = readROM(start+address++);
            if (data != actual)
            {
                result = 1;
                break;
            }
        } 
        if (filename) fclose(fp);
        if (result)
            printf("verify error, found 0x%X at location 0x%X.\n", actual, --address);
        else
            printf("verify done.\n");
        return 0;
    }
    if (v_flag && a_flag)
    {
        address_t address = 0;
        unsigned int data = 0;
        unsigned int actual = 0;
        char result = 1;
        if (filename) fp = fopen(filename, "r");
        if (!length) length = -1;
        printf("Verifying ROM with contents of ascii file: %s from start 0x%X...\n", filename, start);
        while (fscanf(fp, "%2X", &data) == 1 && address-length)
        {
            result = 0;
            actual = readROM(start+address++);
            if (data != actual)
            {
                result = 1;
                break;
            }
        } 
        if (filename) fclose(fp);
        if (result)
            printf("verify error, found 0x%X at location 0x%X.\n", actual, --address);
        else
            printf("verify done.\n");
        return 0;
    }
    if (l_flag)
    {
        printf("ROM Software Data Protection %s.\n", (mode? "Enabled" : "Disabled"));
        setSDPMode(mode);
        return 0;
    }
    return 0;
}
