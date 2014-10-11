# MPLAB IDE generated this makefile for use with GNU make.
# Project: Project-V1.mcp
# Date: Fri Oct 10 22:15:54 2014

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

Project-V1.cof : main.o
	$(LD) /p18F4550 "main.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"Project-V1.cof" /M"Project-V1.map" /W

main.o : main.c ../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f4550.h
	$(CC) -p=18F4550 "main.c" -fo="main.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "Project-V1.cof" "Project-V1.hex"

