
TARGETS = src tests


.PHONY: all clean $(TARGETS)

all: $(TARGETS)

src:
	$(MAKE) -C src

tests:
	$(MAKE) -C tests

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean

