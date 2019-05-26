#! /usr/bin/make -f
#  Makefile for the gint control add-in
#---

#
#  Configuration
#

# Compiler flags
cf        := -DFX9860G -lm -lfx -Wl,-Map=build.fx/map
cf-fx     := $(cf) `fxsdk --libs --cflags`

# Linker flags
#None needed :)

dflags     = -MMD -MT $@ -MF $(@:.o=.d) -MP
cpflags   := -R .bss -R .gint_bss

g1af      := -i icon.bmp -n rendex --internal=@RENDEX

#
#  File listings
#

elf        = $(dir $<)rendex.elf
bin        = $(dir $<)rendex.bin
target-fx := rendex.g1a

# Source and object files
src       := $(wildcard *.c)
res       := $(wildcard resources/*)
obj-fx    := $(src:%.c=build.fx/%.o) $(res:resources/%=build.fx/%.o)

# Additional dependencies
deps-fx   := icon.bmp

#
#  Build rules
#

all: all-fx

all-fx: $(target-fx)

$(target-fx): $(obj-fx) $(deps-fx)

	sh3eb-elf-gcc -o $(elf) $(obj-fx) $(cf-fx)
	sh3eb-elf-objcopy -O binary $(cpflags) $(elf) $(bin)
	g1a-wrapper $(bin) -o $@ -i icon.bmp
	#fxg1a $(bin) -o $@ $(g1af)

# C sources
build.fx/%.o: %.c | build.fx/
	sh3eb-elf-gcc -c $< -o $@ $(cf-fx) $(dflags)

# Images
build.fx/%.png.o: resources/%.png | build.fx/
	fxconv -i $< -o $@ -n $*

# Fonts
build.fx/assets_font_island.bmp.o: resources/assets_font_island.bmp | build.fx/
	fxconv -font $< -o $@ -n assets_font_island

# Bitmaps
build.fx/%.bmp.o: resources/%.bmp | build.fx/
	fxconv -image $< -o $@ -n $*

%/:
	@ mkdir -p $@

#
#  Cleaning and utilities
#

# Dependency information
-include $(shell find build* -name *.d 2> /dev/null)
build.fx/%.d: ;
.PRECIOUS: build.fx build.fx/%.d %/

clean:
	@ rm -rf build*
distclean: clean
	@ rm -f $(target-fx)

install-fx: $(target-fx)
	p7 send -f $<

.PHONY: all all-fx clean distclean install-fx
 
