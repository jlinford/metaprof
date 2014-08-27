
TARGETS = foo start_stop


.PHONY: all clean $(TARGETS)

all: $(TARGETS)

foo:
	$(MAKE) -C foo

start_stop:
	$(MAKE) -C start_stop

clean:
	$(MAKE) -C foo clean
	$(MAKE) -C start_stop clean

realclean:
	$(MAKE) -C foo realclean
	$(MAKE) -C start_stop realclean

