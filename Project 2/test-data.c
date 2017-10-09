/*******************************************************
*Unit Test Cases
*Test File: data.c & data.h
*Test Environment: Linux Ubuntu 16.04 on Virtual Box 5.1
*Date : 5th March 2017
*The following unit tests have been performed
*
*-->Invalid Pointers for big_to_little()
*-->Verify valid conversion for big_to_little()
*-->Invalid Pointers for little_to_big()
*-->Verify valid conversion for little_to_big()
********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "data.h"

/*******************************************************
*Test Case: This test verifies if the function fails
	    when a NULL pointer is passed to the
	    big_to_little() function

*Input Parameters: Pointer to memory location,
		   length of memory to be converted

*Expected Result: The big_to_little() function should
		  return an error code 1 if a null
		  pointer is passed to the function

*Assumptions: N/A

*Test Results: The function fails by returning an error
	       code 1 as expected since a NULL pointer
	       was passed to the fucntion
*******************************************************/

void test_big_to_little_null_pointer(void **state)
{
	uint32_t *data = NULL;
	uint8_t return_value;
	uint32_t length = 1;
	return_value = big_to_little(data, length);
	assert_int_equal(1, return_value);
}

/*******************************************************
*Test Case: This test verifies if the function fails
	    when a NULL pointer is passed to the
	    little_to_big() function

*Input Parameters: Pointer to memory location,
		   length of memory to be converted

*Expected Result: The little_to_big() function should
		  return an error code 1 if a null
		  pointer is passed to the function

*Assumptions: N/A

*Test Results: The function fails by returning an error
	       code 1 as expected since a NULL pointer
	       was passed to the fucntion
*******************************************************/

void test_little_to_big_null_pointer(void **state)
{
	uint32_t *data = NULL;
	uint8_t return_value;
	uint32_t length = 1;
	return_value = little_to_big(data, length);
	assert_int_equal(1, return_value);
}

/*******************************************************
*Test Case: This test verifies if the big_to_little()
	    function coverts correctly

*Input Parameters: Pointer to memory location,
		   length of memory to be converted

*Expected Result: The big_to_little() function should
		  return zero indicating the conversion
		  was successful 

*Assumptions: N/A

*Test Results: The function returns zero indicating the
	       conversion was successful. Then we
	       compare the final output with what the
	       correct output is to confirm if the 
	       conversion was correct. The data is 
	       converted twice and then compared with 
	       the original value
*******************************************************/

void test_big_to_little(void **state)
{
	uint32_t value = 0xABCDEF98;
	uint32_t value_to_compare = 0xABCDEF98;
	uint8_t return_value;
	uint32_t *data = &value;
	uint32_t length = 1;
	return_value = big_to_little(data, length);
	assert_int_equal(0, return_value);
	return_value = big_to_little(data, length);
	assert_int_equal(0, return_value);
	assert_int_equal(value, value_to_compare);
}

/*******************************************************
*Test Case: This test verifies if the little_to_big()
	    function coverts correctly

*Input Parameters: Pointer to memory location,
		   length of memory to be converted

*Expected Result: The little_to_big() function should
		  return zero indicating the conversion
		  was successful 

*Assumptions: N/A

*Test Results: The function returns zero indicating the
	       conversion was successful. Then we
	       compare the final output with what the
	       correct output is to confirm if the 
	       conversion was correct. The data is 
	       converted twice and then compared with 
	       the original value
*******************************************************/

void test_little_to_big(void **state)
{
	uint32_t value = 0xABCDEF98;
	uint32_t value_to_compare = 0xABCDEF98;
	uint8_t return_value;
	uint32_t *data = &value;
	uint32_t length = 1;
	return_value = little_to_big(data, length);
	assert_int_equal(0, return_value);
	return_value = little_to_big(data, length);
	assert_int_equal(0, return_value);
	assert_int_equal(value, value_to_compare);
}

int main(int argc, char **argv)
{
	const struct CMUnitTest tests[] = 
	{
		cmocka_unit_test(test_big_to_little_null_pointer),
		cmocka_unit_test(test_little_to_big_null_pointer),
		cmocka_unit_test(test_big_to_little),
		cmocka_unit_test(test_little_to_big),
	};

  	return cmocka_run_group_tests(tests, NULL, NULL);
}
