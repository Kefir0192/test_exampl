CFLAGS  += -I$(CURDIR)





# list of tests for build
TESTS  = $(shell ls -d */)



.PHONY: all
all: $(TESTS)



.PHONY: clean
clean:
	@for test in $(TESTS); do \
        $(MAKE) -C $$test clean; \
    done



.PHONY: $(TESTS)
$(TESTS):
	@for test in $(TESTS); do \
        $(MAKE) -C $$test; \
	    echo "  ---- Compiled $@ ----"; \
	    echo ""; \
    done

