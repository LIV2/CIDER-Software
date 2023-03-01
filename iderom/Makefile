PROGRAM=cider-ide.rom
INCLUDE=/opt/amiga/m68k-amigaos/ndk-include
AS=vasmm68k_mot
ASFLAGS=-Fhunk -I$(INCLUDE) -quiet -align
LINKER=vlink
LINKFLAGS=-brawbin1 -s -sc -sd -mrel -lamiga -lauto -L/opt/amiga/m68k-amigaos/vbcc/lib
OBJDIR=obj

.PHONY: all clean rom mounter/obj/mounter.hunk ide_device/source/ide.device disk

SRCS = bootldr.S \
       endrom.S
OBJS = $(SRCS:%.S=$(OBJDIR)/%.o)

all:	$(PROGRAM) disk

$(OBJDIR)/mungerom: mungerom.c
	@mkdir -p $(OBJDIR)
	$(CC) $< -Wall -o $@

$(OBJDIR)/%.o: %.S
	@mkdir -p $(OBJDIR)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJDIR)/bootldr:	$(OBJDIR)/bootldr.o
	$(LINKER) $(LINKFLAGS) -o $@ $^

$(OBJDIR)/bootnibbles: $(OBJDIR)/bootldr $(OBJDIR)/mungerom
	@mkdir -p $(OBJDIR)
	./obj/mungerom

mounter/obj/mounter.hunk: ide_device/source/ide.device
	$(MAKE) -C mounter all

$(OBJDIR)/assets.o: assets.S $(OBJDIR)/bootnibbles mounter/obj/mounter.hunk

$(PROGRAM): $(OBJDIR)/bootnibbles $(OBJDIR)/assets.o
	$(LINKER) $(LINKFLAGS) -Trom.ld -o $@ $(OBJDIR)/assets.o

disk:
	$(MAKE) -C disk all

clean:
	$(MAKE) -C disk clean
	$(MAKE) -C mounter clean
	rm -f $(OBJDIR)/*
	rm -f $(PROGRAM)
	rm -f mungerom
