/*******************************************************
*Unit Test Cases
*Test File: memory.c & memory.h
*Test Environment: Linux Ubuntu 16.04 on Virtual Box 5.1
*Date : 5th March 2017
*The following unit tests have been performed
*
*-->Invalid Pointers for my_memmove()
*-->No overlap
*-->Source in destination region overlap
*-->Destination in source region overlap
*-->Invalid Pointers for memset()
*-->Verify memset()
*-->Invalid Pointers for memzero()
*-->Verify memzero()
*-->Invalid Pointers for my_reverse()
*-->Verify my_reverse() for odd number of input elements
*-->Verify my_reverse() for even number of input elements
*-->Verify my_reverse() for character set of 0-255
********************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "memory.h"


/*******************************************************
*Test Case: This test verifies if the function fails
	    when a NULL pointer is passed to the
	    my_memmove() function

*Input Parameters: Pointer to source memory location,
		   Pointer to destination memory
		   location and length of memory to be
		   moved

*Expected Result: The my_memmove() function should
		  return an error code 1 if a null
		  pointer is passed to the function

*Assumptions: Similar result is expected if the 
	      destination pointer is a NULL pointer
	      instead of the source pointer or if
	      both the pointers are NULL pointers since
	      the function checks for NULL pointers
	      using an OR fucntion

*Test Results: The function fails by returning an error
	       code 1 as expected since a NULL pointer
	       was passed to the fucntion
*******************************************************/
void test_null_pointer_memmove(void **state)
{
	uint8_t test_var = 1;
	uint32_t length = 1;
	uint8_t return_value;
	/*Null Pointer which is passed to the fucntion*/
	uint8_t *src = NULL;
	uint8_t *dst = &test_var;
	/*The return value is compared to 1 
	to confirm if the function failed*/
	return_value = my_memmove(src, dst, length);
	assert_int_equal(1,return_value);
}


/*******************************************************
*Test Case: This test verifies if the my_memmove() 
	    function works correctly when there is no 
	    overlap for the source and destination 
	    pointer

*Input Parameters: Pointer to source memory location,
		   Pointer to destination memory
		   location and length of memory to be
		   moved

*Expected Result: The my_memmove() function should
		  return zero indicating that the move 
		  was successful

*Assumptions: N/A

*Test Results: The function returns zero indicating that
	       the move operation was successful
*******************************************************/

void test_no_overlap(void **state)
{
	uint8_t return_value;
	uint32_t length = 4;
	uint32_t test_var_1 = 0xFFFFFFFF;
	uint32_t test_var_2 = 0xAAAAAAAA;
	uint8_t *src = (uint8_t *)&test_var_1;
	uint8_t *dst = (uint8_t *)&test_var_2;
	return_value = my_memmove(src, dst, length);
	assert_int_equal(return_value, 0);
}

/*******************************************************
*Test Case: This test verifies if the my_memmove() 
	    function works correctly when there is an 
	    overlap for a part of the source memory 
	    region with the destination memory region

*Input Parameters: Pointer to source memory location,
		   Pointer to destination memory
		   location and length of memory to be
		   moved

*Expected Result: The my_memmove() function should
		  return zero indicating that the move 
		  was successful

*Assumptions: The comparison to the final data is made
	      assuming little endian structure. The
	      resultant data would be different if the
	      system was big endian. The function would
	      still work correctly but the output would 
	      yield a different result.

*Test Results: The function returns zero indicating that
	       the move operation was successful. The 
	       location of the source and destination
	       does not affect the correct working of 
	       the function because memory is moved from
	       the end of the source memory location 
	       when this condition occurs. The result is
	       compared to correct output to verify if 
	       it is correct
*******************************************************/

void source_destination_overlap(void **state)
{
	uint8_t return_value;
	uint32_t length = 2;
	uint32_t test_var_1 = 0xFFFFAAAA;
	uint8_t *src = (uint8_t *)&test_var_1;
	/*src points to the last A at the end*/
	uint8_t *dst = (src+1);
	/*src points to the second last A at the end*/
	return_value = my_memmove(src, dst, length);
	assert_int_equal(return_value, 0);
	/* The last 2 bytes are zero because when data
	is moved the original location is filled with
	zeroes*/
	assert_int_equal(test_var_1, 0xFFAAAA00);
}


/*******************************************************
*Test Case: This test verifies if the my_memmove() 
	    function works correctly when there is an 
	    overlap for a part of the destination memory 
	    region with the source memory region

*Input Parameters: Pointer to source memory location,
		   Pointer to destination memory
		   location and length of memory to be
		   moved

*Expected Result: The my_memmove() function should
		  return zero indicating that the move 
		  was successful

*Assumptions: The comparison to the final data is made
	      assuming little endian structure. The
	      resultant data would be different if the
	      system was big endian. The function would
	      still work correctly but the output would 
	      yield a different result.

*Test Results: The function returns zero indicating that
	       the move operation was successful. The 
	       location of the source and destination
	       does not affect the correct working of 
	       the function because memory is moved from
	       the beginning of the source memory location 
	       when this condition occurs. The result is
	       compared to correct output to verify if 
	       it is correct
*******************************************************/

void destination_source_overlap(void **state)
{
	uint8_t return_value;
	uint32_t length = 2;
	uint32_t test_var_1 = 0xFFFFAAAA;
	uint8_t *dst = (uint8_t *)&test_var_1;
	/*dst points to the last A at the end*/
	uint8_t *src = (dst+1);
	/*src points to the second last A at the end*/
	return_value = my_memmove(src, dst, length);
	assert_int_equal(return_value, 0);
	assert_int_equal(test_var_1, 0xFF00FFAA);
}

/*******************************************************
*Test Case: This test verifies if the my_memset() 
	    function fails when a NULL pointer
	    is passed as source pointer to the function

*Input Parameters: Pointer to source memory location,
		   length of memory to be set, and the 
		   value which has to be set

*Expected Result: The my_memset() function should
		  return one indicating the function
		  failed

*Assumptions: N/A

*Test Results: The function returns one as expected
	       indicating the function failed since a 
	       null pointer was passed to it
*******************************************************/

void test_null_pointer_memset(void **state)
{
	uint32_t length = 2;
	uint8_t value = 0xFF;
	uint8_t *src = NULL;
	uint8_t return_value = my_memset(src, length, value);
	assert_int_equal(1,return_value);
}

/*******************************************************
*Test Case: This test verifies if the my_memset() 
	    function works correctly for a given data
	    value

*Input Parameters: Pointer to source memory location,
		   length of memory to be set, and the 
		   value which has to be set

*Expected Result: The my_memset() function should
		  return zero indicating that it was 
		  able to set the memory locations
		  successfully

*Assumptions: N/A

*Test Results: The function returns zero indicating the
	       function was able to set memory
	       successfully. The result is compared to
	       the correct output to verify that it
	       works as expected
*******************************************************/

void test_memset(void **state)
{
	uint32_t length = 4;
	uint32_t value = 0x000000FA;
	uint8_t *src = (uint8_t *)&value;
	uint8_t return_value = my_memset(src, length, 0xFA);
	assert_int_equal(0,return_value);
	assert_int_equal(value, 0xFAFAFAFA);
}

/*******************************************************
*Test Case: This test verifies if the my_memzero() 
	    function fails when a NULL pointer
	    is passed as source pointer to the function

*Input Parameters: Pointer to source memory location,
		   length of memory to be set to zero

*Expected Result: The my_memzero() function should
		  return one indicating the function
		  failed

*Assumptions: N/A

*Test Results: The function returns one as expected
	       indicating the function failed since a 
	       null pointer was passed to it.
*******************************************************/


void test_null_pointer_memzero(void **state)
{
	uint32_t length = 2;
	uint8_t *src = NULL;
	uint8_t return_value = my_memzero(src, length);
	assert_int_equal(1,return_value);
}

/*******************************************************
*Test Case: This test verifies if the my_memzero() 
	    function works correctly for a given data
	    value

*Input Parameters: Pointer to source memory location,
		   length of memory to be set to zero

*Expected Result: The my_memzero() function should
		  return zero indicating that it was 
		  able to set the memory locations to
		  zero successfully

*Assumptions: N/A

*Test Results: The function returns zero indicating the
	       function was able to set memory to zero
	       successfully. The result is compared to
	       the correct output to verify that it
	       works as expected
*******************************************************/

void test_memzero(void **state)
{
	uint32_t length = 4;
	uint32_t value = 0xFAFAFAFA;
	uint8_t *src = (uint8_t *)&value;
	uint8_t return_value = my_memzero(src, length);
	assert_int_equal(0,return_value);
	assert_int_equal(value, 0x00000000);
}

/*******************************************************
*Test Case: This test verifies if the my_reverse() 
	    function fails when a NULL pointer
	    is passed as source pointer to the function

*Input Parameters: Pointer to source memory location,
		   length of memory to be reversed

*Expected Result: The my_reverse() function should
		  return one indicating the function
		  failed

*Assumptions: N/A

*Test Results: The function returns one as expected
	       indicating the function failed since a 
	       null pointer was passed to it.
*******************************************************/

void test_null_pointer_reverse(void **state)
{
	uint32_t length = 2;
	uint8_t *src = NULL;
	uint8_t return_value = my_reverse(src, length);
	assert_int_equal(1,return_value);
}

/*******************************************************
*Test Case: This test verifies if the my_reverse() 
	    function works correctly for an odd number of
	    memory elements

*Input Parameters: Pointer to source memory location,
		   length of memory to be reversed

*Expected Result: The my_reverse() function should
		  return zero indicating the reverse 
		  operation was successful

*Assumptions: N/A

*Test Results: The function returns zero indicating that
	       the reverse operation was successful. To
	       verify if the reversal was done correctly
	       the input is reversed twice and then
	       compared to a copy of its original value
	       to check if they are the same
*******************************************************/

void test_odd_reverse(void **state)
{
	uint32_t length = 11;
	uint8_t value[] = "Test-String";
	uint8_t value_to_compare[] = "Test-String";
	uint8_t *src = value;
	uint8_t return_value = my_reverse(src, length);
	assert_int_equal(0, return_value);
	return_value = my_reverse(src, length);
	assert_int_equal(0, return_value);
	assert_string_equal(value, value_to_compare);
}

/*******************************************************
*Test Case: This test verifies if the my_reverse() 
	    function works correctly for an even number of
	    memory elements

*Input Parameters: Pointer to source memory location,
		   length of memory to be reversed

*Expected Result: The my_reverse() function should
		  return zero indicating the reverse 
		  operation was successful

*Assumptions: N/A

*Test Results: The function returns zero indicating that
	       the reverse operation was successful. To
	       verify if the reversal was done correctly
	       the input is reversed twice and then
	       compared to a copy of its original value
	       to check if they are the same
*******************************************************/

void test_even_reverse(void **state)
{
	uint32_t length = 12;
	uint8_t value[] = "Test-Strings";
	uint8_t value_to_compare[] = "Test-Strings";
	uint8_t *src = value;
	uint8_t return_value = my_reverse(src, length);
	assert_int_equal(0, return_value);
	return_value = my_reverse(src, length);
	assert_int_equal(0, return_value);
	assert_string_equal(value, value_to_compare);
}

/*******************************************************
*Test Case: This test verifies if the my_reverse() 
	    function works correctly for a character set
	    ranging from 0-255

*Input Parameters: Pointer to source memory location,
		   length of memory to be reversed

*Expected Result: The my_reverse() function should
		  return zero indicating the reverse 
		  operation was successful

*Assumptions: N/A

*Test Results: The function returns zero indicating that
	       the reverse operation was successful. To
	       verify if the reversal was done correctly
	       the input is reversed twice and then
	       compared to a copy of its original value
	       to check if they are the same
*******************************************************/

void test_character_reverse(void **state)
{
	uint8_t number[256];
	uint8_t number_copy[256];
	for(uint8_t i=0; i<255; i++)
	{
    		*(number+i)=i;
    		*(number_copy+255-i) = i;
    	}
    	uint32_t length = 256;
    	uint8_t *src = number;
    	uint8_t return_value = my_reverse(src, length);
    	assert_int_equal(0, return_value);
    	assert_string_equal(number, number_copy);
}

int main(int argc, char **argv)
{
	const struct CMUnitTest tests[] = 
	{
		cmocka_unit_test(test_null_pointer_memmove),
		cmocka_unit_test(test_no_overlap),
		cmocka_unit_test(source_destination_overlap),
		cmocka_unit_test(destination_source_overlap),
		cmocka_unit_test(test_null_pointer_memset),
		cmocka_unit_test(test_memset),
		cmocka_unit_test(test_null_pointer_memzero),
		cmocka_unit_test(test_memzero),
		cmocka_unit_test(test_null_pointer_reverse),
		cmocka_unit_test(test_odd_reverse),
		cmocka_unit_test(test_even_reverse),
		cmocka_unit_test(test_character_reverse),
	};

  	return cmocka_run_group_tests(tests, NULL, NULL);
}
