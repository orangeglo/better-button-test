#
# Simple Makefile that compiles all .c and .s files in the same folder
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = ../../../

LCC = $(GBDK_HOME)bin/lcc 

# You can uncomment the line below to turn on debug output
# LCC = $(LCC) -debug

# You can set the name of the .gb ROM file here
PROJECTNAME    = better-button-test

BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard *.c)
ASMSOURCES := $(wildcard *.s)

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link all source files in a single call to LCC
# -Wm-yc : color support
# -Wl-yt0x1B : MBC5 ROM + RAM + Battery (for better compatibility with emulators)
# -Wl-ya1 : 1 RAM bank

$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -Wm-yn"BETTERBUTTON" -Wm-yc -Wl-yt0x1B -Wl-ya1 -D_ram_func=0xD000 -o $@ $(CSOURCES) $(ASMSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm

dev:
	make && (killall SameBoy || true) && open $(PROJECTNAME).gb