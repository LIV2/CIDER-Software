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
	mkdir -p obj
	$(AS) $(ASFLAGS) $< -o $@

obj/%.o: %.c
	mkdir -p obj
	${CC} -o $@ $(CFLAGS) $<

../ide_device/source/ide.device:
	$(MAKE) -C ../ide_device/source all

obj/mounter.hunk: obj/romtag.o obj/mounter.o obj/endrom.o ../ide_device/source/ide.device
	$(LINKER) $(LINKFLAGS) -o $@ $^

clean:
	$(MAKE) -C ../ide_device/source clean
	-rm -rf obj/*
