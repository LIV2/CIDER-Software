PROJECT=mounter
#CC=m68k-amigaos-gcc
#CFLAGS=-lamiga -nostartfiles -mcrt=clib2 -mcpu=68000 -c
INCLUDE=/opt/amiga/m68k-amigaos/ndk-include
CC=vc
CFLAGS=-cpu=68000 -c99 -lamiga -c -Os -dontwarn=153 -dontwarn=368 -I/opt/amiga/m68k-amigaos/vbcc/targets/m68k-amigaos/include
AS=vasmm68k_mot
ASFLAGS=-Fhunk -I$(INCLUDE) -quiet -align
LINKER=vlink
LINKFLAGS=-bamigahunk -s -sc -sd -mrel -lamiga -lauto -L/opt/amiga/m68k-amigaos/vbcc/lib

.PHONY:	clean all
all:	obj/mounter.hunk

OBJ = mounter.o

ASMOBJ = romtag.o \
         endrom.o

ASMSRCS = $(ASMOBJ:%.o=%.S)

SRCS = $(OBJ:%.o=%.c)

obj/%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

obj/%.o: %.c
	${CC} -o $@ $(CFLAGS) $<

obj/mounter.hunk: obj/romtag.o obj/mounter.o obj/endrom.o liv2ride.device
	$(LINKER) $(LINKFLAGS) -o $@ $^
clean:
	-rm -rf obj/*
