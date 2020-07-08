#  ___ ___ ___ ___  ___  __  __
# | __| __| _ \ _ \/ _ \|  \/  |___ _ __ _ _ ___  __ _ _ _ __ _ _ __  _ __  ___ _ _
# | _|| _||  _/   / (_) | |\/| |___| '_ \ '_/ _ \/ _` | '_/ _` | '  \| '  \/ -_) '_|
# |___|___|_| |_|_\\___/|_|  |_|   | .__/_| \___/\__, |_| \__,_|_|_|_|_|_|_\___|_|
#                                  |_|           |___/
#
# Author.....: Alessandro Fraschetti (mail: gos95@gommagomma.net)
# Target.....: RaspberryPI
# Version....: 1.0 2020/06/06
# Description: EEPROM 28C-family programmer utility
# URL........: https://github.com/gom9000/xp-eeprom-28C-programmer
# License....: this program is under the terms of MIT License
# Module.....: makefile for GCC compiler
# Notes......:


SRCPATH   = src
OBJPATH   = obj
BINPATH   = dist
 
CC        = gcc
RM        = rm -f

CFLAGS    = -Wall -g

BIN       = eeprog28
OBJS      = $(OBJPATH)/main.o $(OBJPATH)/eeprom.o
LIBS      = -lwiringPi


$(BINPATH)/$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(BINPATH)/$(BIN)


clean:
	$(RM) $(OBJS) $(BINPATH)/$(BIN)

$(OBJPATH)/main.o: $(SRCPATH)/main.c $(SRCPATH)/eeprom.h
	$(CC) -c $(SRCPATH)/main.c -o $(OBJPATH)/main.o
$(OBJPATH)/eeprom.o: $(SRCPATH)/eeprom.c $(SRCPATH)/eeprom.h
	$(CC) -c $(SRCPATH)/eeprom.c -o $(OBJPATH)/eeprom.o