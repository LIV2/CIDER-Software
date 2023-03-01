all: CIDER-IDE-Update.adf

CIDER-IDE-Update.adf: startup-sequence ../../cflash/cflash ../cider-ide.rom
	xdftool $@ format CIDER-IDE + boot install boot1x + makedir s + write startup-sequence s/startup-sequence + write ../../cflash/cflash cflash + write ../cider-ide.rom cider-ide.rom

clean:
	rm -rf *.adf
