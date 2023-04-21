PROGRAM=cider-ide.rom
INCLUDE=/opt/amiga/m68k-amigaos/ndk-include
AS=vasmm68k_mot
ASFLAGS=-Fhunk -I$(INCLUDE) -quiet -align
LINKER=vlink
LINKFLAGS=-brawbin1 -s -sc -sd -mrel -lamiga -lauto -L/opt/amiga/m68k-amigaos/vbcc/lib
OBJDIR=obj

.PHONY: all clean rom ide_device/liv2ride.device disk

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

ide_device/liv2ride.device:
	$(MAKE) -C ide_device all

$(OBJDIR)/assets.o: assets.S $(OBJDIR)/bootnibbles ide_device/liv2ride.device

$(PROGRAM): $(OBJDIR)/bootnibbles $(OBJDIR)/assets.o
	$(LINKER) $(LINKFLAGS) -Trom.ld -o $@ $(OBJDIR)/assets.o

$(PROGRAM)-atbus: $(OBJDIR)/bootnibbles $(OBJDIR)/assets.o
	$(LINKER) $(LINKFLAGS) -Tatbusrom.ld -o $@ $(OBJDIR)/assets.o

disk:
	$(MAKE) -C disk all

clean:
	$(MAKE) -C disk clean
	$(MAKE) -C ide_device clean
	rm -f $(OBJDIR)/*
	rm -f $(PROGRAM)
	rm -f mungerom
