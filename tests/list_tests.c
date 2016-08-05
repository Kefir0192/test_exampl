#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#include "unit_tests.h"
#include "list.h"





struct tmp_data
{
   struct list_head list;
   int              data;
};





int test_list_empty(struct test_info_t  *test_info)
{

    TEST_INIT;

    DECLARE_LIST_HEAD(tmp_list);
    struct tmp_data d1;


    if(!list_empty(&tmp_list))             //list must be empty
        return TEST_BROKEN;


    list_push_front(&d1.list, &tmp_list);   //now d1 is first

    if(list_empty(&tmp_list))               //now list is NOT empty
        return TEST_BROKEN;


    return TEST_PASSED;
}



ptest_func tests[] =
{

    test_list_empty,

};



int main(void)
{

    RUN_TESTS(tests);

    return 0;
}

