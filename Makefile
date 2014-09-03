BUILD_DIR = build

.PHONY: all clean realclean check_configured

all: check_configured
	$(MAKE) -C $(BUILD_DIR)

clean:
	$(MAKE) -C $(BUILD_DIR) clean

realclean:
	@rm -rf $(BUILD_DIR)

check_configured:
	@if [ ! -d $(BUILD_DIR) ] ; then echo "ERROR: Run ./configure before running make" ; false ; else true ; fi

