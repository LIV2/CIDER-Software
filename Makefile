SHELL:=/bin/bash
CURDIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
TMPDIR=$(CURDIR)/tmp/
ARTIFACTDIR:=$(CURDIR)/artifacts

.PHONY: all software clean

software:
	$(MAKE) -C bootrom all
	$(MAKE) -C addram all
	$(MAKE) -C cflash all
#	$(MAKE) -C iderom all

clean:
	$(MAKE) -C bootrom clean
	$(MAKE) -C addram clean
	$(MAKE) -C cflash clean
#	$(MAKE) -C iderom clean
	rm -rf $(ARTIFACTDIR)
	rm -rf $(TMPDIR)

package: addram/addram bootrom/bootrom cflash/cflash
	rm -rf $(TMPDIR)
	mkdir -p $(TMPDIR)
	mkdir -p $(ARTIFACTDIR)
	cp $^ $(TMPDIR)
	cd $(TMPDIR) && xdftool $(ARTIFACTDIR)/CIDER-Software.adf format CIDER $(addprefix + write ,$(notdir $^))
	cd $(TMPDIR) && lha -c $(ARTIFACTDIR)/CIDER-Software.lha $(notdir $^)
