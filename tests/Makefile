INCLUDE_DIR  = ../include

CFLAGS      += -I. -I$(INCLUDE_DIR)
CFLAGS      += -O2  -s  -Wall
GCC          =  gcc





# list of tests for build
TESTS  = list_tests



SOURCES  = unit_tests.c           \
           list_tests.c



.PHONY: all
all: clean  $(TESTS)



.PHONY: clean
clean:
	-@rm -f *.o
	-@rm -f *.*~
	-@rm -f $(TESTS)





.PHONY: $(TESTS)
$(TESTS):
	$(GCC)  $(SOURCES) -o $@  $(CFLAGS)
	@echo "  ---- Compiled $@ ----"
	@echo ""



.PHONY: debug
debug: CFLAGS  += -DDEBUG
debug: all



.PHONY: help
help:
	@echo "make [command]"
	@echo "command is:"
	@echo "   clean   -  remove all binary files"
	@echo "   all     -  clean and build all tests"
	@echo "   debug   -  build in debug mode (#define DEBUG 1)"
	@echo "   help    -  This help"

