include config


INCLUDE_DIR   = $(CURDIR)/include
INCLUDE_DIR  += $(CURDIR)/arch/$(ARCH)

CFLAGS       += $(addprefix -I, $(INCLUDE_DIR))


#core
MODULES   = $(CURDIR)/include
MODULES  += $(CURDIR)/arch/$(ARCH)



OBJ_LIST  = $(CURDIR)/objects.txt



# function for add objects to OBJ_LIST
# $$1 - Dir  $$2 - list of objects
to_obj_list = \
    for obj in $$2; do \
        echo  $$1/$$$$obj >> $(OBJ_LIST); \
    done


export


 


OBJ = $(shell cat $(OBJ_LIST))

.PHONY: all
all: clean get_list_obj $(OBJ) link



.PHONY: clean
clean:
	@rm -f $(OBJ)



.PHONY: get_list_obj
get_list_obj:
	@echo "" > $(OBJ_LIST)
	@for m in $(MODULES); do \
          $(MAKE) -C $$m; \
    done



.PHONY: link
link:
	@rm -f kernel
	$(CROSS_COMPILE) $(OBJ) -o kernel





%.o: %.c
	$(CROSS_COMPILE) $(CFLAGS) -c $< -o $@



%.o: %.S
	$(CROSS_COMPILE) $(CFLAGS) -c $< -o $@

