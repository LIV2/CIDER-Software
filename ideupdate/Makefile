all: CIDER-IDE-Update.adf

lide.device/lide.rom:
	make -C lide.device

CIDER-IDE-Update.adf: startup-sequence ../cflash/cflash lide.device/lide.rom
	xdftool $@ format CIDER-IDE + boot install boot1x + makedir s + write startup-sequence s/startup-sequence + write ../cflash/cflash cflash + write lide.device/lide.rom lide.rom

clean:
	rm -rf *.adf
	make -C lide.device clean
