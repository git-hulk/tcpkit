default: all

all:
	cd src && $(MAKE)

test: all
	cd tests && $(MAKE) test

test-race:
	cd tests && $(MAKE) test-race

fixtures:
	cd tests && $(MAKE) fixtures

install:
	cd src && $(MAKE) install

clean:
	cd src && $(MAKE) clean
	cd tests && $(MAKE) clean

.DEFAULT:
	cd src && $(MAKE) $@

.PHONY: default all test test-race fixtures install clean
