# list of tests for build
TESTS  = list_tests



.PHONY: all
all: clean  $(TESTS)



.PHONY: clean
clean:
	-@rm -f *.o
	-@rm -f *.*~
	-@rm -f $(TESTS)



.PHONY: $(TESTS)
$(TESTS):
	$(CROSS_COMPILE)  $@.c -o $@  $(CFLAGS)
