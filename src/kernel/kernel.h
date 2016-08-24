#ifndef KERNEL_HEADER
#define KERNEL_HEADER


#include <stddef.h>





#define  SET_BIT(reg, num_bit)   (reg |=  (1 << num_bit) )
#define  CLR_BIT(reg, num_bit)   (reg &= ~(1 << num_bit) )
#define  INV_BIT(reg, num_bit)   (reg ^=  (1 << num_bit) )



#define  SET_FLAG(reg, flag)   (reg |=  (flag) )
#define  CLR_FLAG(reg, flag)   (reg &= ~(flag) )
#define  INV_FLAG(reg, flag)   (reg ^=  (flag) )





//The count of elements in the array.
#define  COUNT_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))





// For GCC 4.6 or higher, in C++ you can use a standard right static_assert(exp, msg) in *.c and in *.h files.
// For GCC 4.6 is required to add CFLAGS += -std="c++0x"
// Simple C (gcc) have not static_assert.
// A lot of variants, it is the most simple and intuitive
// It can be used in *.c and in *.h files. (macros that use function style can be used in *.c files only)
// This is not an array. It is the type of data !!!!
// Disadvantages: you can not be set msg to display the console when compiling
//
// Example:
//
//  STATIC_ASSERT( sizeof(char) == 1)  //good job
//  STATIC_ASSERT( sizeof(char) != 1)  //You will get a compilation error
//
#define STATIC_ASSERT(EXPR) typedef char static_assert_failed[(EXPR) ? 1 : -1] __attribute__((unused))





#define  FREE_AND_NULL(ptr_var)  ({                      \
                                     if(ptr_var)         \
                                     {                   \
                                         free(ptr_var);  \
                                         ptr_var = NULL; \
                                     }                   \
                                  })





// container_of - cast a member of a structure out to the containing structure
// ptr:    the pointer to the member.
// type:   the type of the container struct this is embedded in.
// member: the name of the member within the struct.
//
#define container_of( ptr, type, member ) \
    ({  (type *)( (const char *)ptr - offsetof(type, member) );  })





#endif //KERNEL_HEADER
