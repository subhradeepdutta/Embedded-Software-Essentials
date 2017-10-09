/*******************************************************
*Unit Test Cases
*Test File: circbuf.c & circbuf.h
*Test Environment: Linux Ubuntu 16.04 on Virtual Box 5.1
*Date : 5th March 2017
*The following unit tests have been performed
*
*-->allocate free
*-->invalid pointer
*-->non-initialized buffer
*-->add-remove check
*-->buffer full test
*-->buffer empty test
*-->wrap over and add
*-->wrap over and remove
*-->over fill
*-->over empty
********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "circbuf.h"


/*******************************************************
*Test Case: test_allocate_free tests if memory 
	    allocation was successful and then tests if 
	    the memory was successfuly released back for 
	    the circular buffer

*Input Parameters: Pointer to circular buffer structure
		   and size of buffer

*Expected Result: If the buffer structure is allocated
		  successfully then malloc will return
		  a non NULL value and then if the
		  buffer is successfully intialized then
		  it returns NO_ERROR

*Assumptions: N/A

*Test Results: The function returns NO_ERROR indicating
	       that memory allocation for the buffer was
	       successful and a buffer of size 10 has 
	       been created in the heap
*******************************************************/
void test_allocate_free(void **state)
{
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 10);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_destroy(test_buffer);
	assert_int_equal(status, NO_ERROR);
}

/*******************************************************
*Test Case: test_valid_buffer tests if a valid circular
	    buffer structure has been created in 
	    memory. This is determined by checking if
	    the return value for malloc is NULL or not

*Input Parameters: N/A

*Expected Result: The function should return a non NULL
		  value indicating that the buffer
		  structure was successfully allocated

*Assumptions: N/A

*Test Results: malloc returns a non value on allocating
	       the circular buffer structure
*******************************************************/

void test_valid_buffer(void **state)
{
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	circbuf_destroy(test_buffer);
}


/*******************************************************
*Test Case: test_init_buffer tests if the circular
	    buffer was initialized correctly

*Input Parameters: Pointer to the buffer structure and
		   size of buffer

*Expected Result: The function returns NO_ERROR if the
		  buffer intializes successfully. If the
		  buffer length is zero or the pointer is
		  NULL then the test fails

*Assumptions: N/A

*Test Results: malloc returns a non null value on 
	       allocating the circular buffer structure
	       and then the function returns NO_ERROR
	       indicating that the buffer was intialized
*******************************************************/
void test_init_buffer(void **state)
{
	CircBuf_t *test_buffer;
	uint8_t buffer_length = 10;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, buffer_length);
	assert_int_not_equal(buffer_length, 0);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_destroy(test_buffer);
}

/*******************************************************
*Test Case: test_buffer_full tests if the circbuf_full
	    returns BUFFER_FULL once the buffer is full

*Input Parameters: Pointer to the buffer structure

*Expected Result: The function would return BUFFER_FULL if
		  the buffer size is equal to the number
		  of items in the buffer

*Assumptions: N/A

*Test Results: The function returns BUFFER_FULL to indicate
	       that the buffer size is equal to the 
	       number of items in the buffer and any
	       further elements cannot be added to the
	       buffer
*******************************************************/

void test_buffer_full(void **state)
{
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	/*Check if malloc was successful*/
	CB_Enum_t status = circbuf_init(test_buffer, 1);
	/*Check if buffer intialization was successful*/
	assert_int_equal(status, NO_ERROR);
	/*Adding an item to the buffer*/
	status = circbuf_add(test_buffer, 25);
	assert_int_equal(status, NO_ERROR);
	/*Check if buffer is full*/
	status = circbuf_full(test_buffer);
	assert_int_equal(status, BUFFER_FULL);
	status = circbuf_destroy(test_buffer);
}


/*******************************************************
*Test Case: test_buffer_empty tests if the 
	    circbuf_empty returns BUFFER_FULL once the 
	    buffer is full

*Input Parameters: Pointer to the buffer structure

*Expected Result: The function would return BUFFER_EMPTY if
		  the number of elements in the buffer
		  is zero

*Assumptions: N/A

*Test Results: The function returns BUFFER_EMPTY to indicate
	       that the number of items in the buffer
	       is zero
*******************************************************/

void test_buffer_empty(void **state)
{
	uint8_t deleted_item;
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 1);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_add(test_buffer, 25);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_full(test_buffer);
	assert_int_equal(status, BUFFER_FULL);
	/*Verify if the buffer is full*/
	status = circbuf_remove(test_buffer, &deleted_item);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_empty(test_buffer);
	/*Check if the function returns BUFFER_EMPTY once the last element is removed*/
	assert_int_equal(status, BUFFER_EMPTY);
	status = circbuf_destroy(test_buffer);
}

/*******************************************************
*Test Case: test_buffer_overfill verifies that any
	    attempt to add elements to a buffer that is 
	    completely filled results in the function 
	    returning BUFFER_FULL

*Input Parameters: Pointer to the buffer structure and 
		   element to be added

*Expected Result: The function would return BUFFER_FULL to
		  indicate that any more items cannot be
		  added to the buffer

*Assumptions: N/A

*Test Results: The function returns BUFFER_FULL indicating
	       that any more elements cannot be added 
	       to the buffer
*******************************************************/
void test_buffer_overfill(void **state)
{
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 1);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_add(test_buffer, 25);
	assert_int_equal(status, NO_ERROR);
	/*Try to add an element to a filled up buffer*/
	status = circbuf_add(test_buffer, 45);
	assert_int_equal(status, BUFFER_FULL);
	status = circbuf_destroy(test_buffer);
}

/*******************************************************
*Test Case: test_buffer_overempty verifies that any
	    attempt to remove elements from a buffer 
	    that is completely empty results in the 
	    function returning BUFFER_EMPTY

*Input Parameters: Pointer to the buffer structure and 
		   pointer to store the deleted item

*Expected Result: The function would return BUFFER_EMPTY to
		  indicate that any more items cannot be
		  removed from the buffer

*Assumptions: N/A

*Test Results: The function returns BUFFER_EMPTY indicating
	       that any more elements cannot be removed
	       from the buffer
*******************************************************/
void test_buffer_overempty(void **state)
{
	uint8_t deleted_item;
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 1);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_add(test_buffer, 25);
	assert_int_equal(status, NO_ERROR);
	status = circbuf_remove(test_buffer, &deleted_item);
	assert_int_equal(status, NO_ERROR);
	/*Try to remove elements from an empty buffer*/
	status = circbuf_remove(test_buffer, &deleted_item);
	assert_int_equal(status, BUFFER_EMPTY);	
	status = circbuf_destroy(test_buffer);
}


/*******************************************************
*Test Case: test_buffer_add_remove verifies if the item
	    being added to the buffer is same as the one
	    which is removed from the same location for
	    the entire length of the buffer

*Input Parameters: Pointer to the buffer structure and 
		   data to be added
		   Pointer to the buffer structure and
		   pointer to the data being deleted

*Expected Result: Elements from an array are added to
		  the buffer and then removed one by
		  one and then compared with the
		  original array to verify if they are
		  indeed the same values

*Assumptions: N/A

*Test Results: A comparison of the original array values
	       with the elements deleted from the buffer
	       shows that both of them are the same
*******************************************************/

void test_buffer_add_remove(void **state)
{
	uint8_t data_values[]={1,2,3,4};
	uint8_t deleted_item;
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 4);
	assert_int_equal(status, NO_ERROR);
	/*Add array elements into the buffer*/
	for(uint8_t i=0; i<4; i++)
	{
		status = circbuf_add(test_buffer, *(data_values+i));
		assert_int_equal(status, NO_ERROR);
	}
	/*Remove each element and compare with the array */
	for(uint8_t i=0; i<4; i++)
	{
		status = circbuf_remove(test_buffer, &deleted_item);
		assert_int_equal(status, NO_ERROR);
		assert_int_equal(deleted_item, *(data_values+i));
	}
	status = circbuf_destroy(test_buffer);

}

/*******************************************************
*Test Case: wrap_add verifies if the buffer can add 
	    elements by rolling over the end and adding 
	    elements in the beginning of the buffer

*Input Parameters: Pointer to the buffer structure and 
		   data to be added

*Expected Result: Once the elements are filled up till
		  the end of the buffer, the head should
		  now move to the beginning of the buffer
		  and add elements

*Assumptions: N/A

*Test Results: Once the head reaches the end of the
	       buffer, it rolls over to the beginning of
	       the allocated memory and adds elements to
	       the beginning
*******************************************************/

void wrap_add(void **state)
{
	uint8_t data_values[]={1,2,3,4};
	uint8_t deleted_item;
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 4);
	assert_int_equal(status, NO_ERROR);
	/*Load the buffer with the contents of the array*/
	for(uint8_t i=0; i<4; i++)
	{
		status = circbuf_add(test_buffer, *(data_values+i));
		assert_int_equal(status, NO_ERROR);
	}
	/*Remove two elements so that tail moves ahead*/
	for(uint8_t i=0; i<2; i++)
	{
		status = circbuf_remove(test_buffer, &deleted_item);
		assert_int_equal(status, NO_ERROR);
	}
	/*Add new elements so that head rolls over and adds to the buffer*/
	for(uint8_t i=0; i<2; i++)
	{
		status = circbuf_add(test_buffer, *(data_values+i+2));
		assert_int_equal(status, NO_ERROR);
	}
	/*Now we peek into the buffer to fetch the values at the
	specified index and compare it with the array elements*/
	for(uint8_t i=0; i<2; i++)
	{
		status = circbuf_peek(test_buffer, i+2, &deleted_item);
		assert_int_equal(status, NO_ERROR);
		assert_int_equal(deleted_item, *(data_values+i+2));
	}
	status = circbuf_destroy(test_buffer);
}


/*******************************************************
*Test Case: wrap_remove verifies if the buffer can 
	    continue removing elements by rolling over 
	    the end

*Input Parameters: Pointer to the buffer structure and 
		   pointer to store the deleted element

*Expected Result: Once the tail reaches the last memory
		  location in the buffer, it should roll
		  over to the beginning of the allocated
		  memory and continue removing elements

*Assumptions: N/A

*Test Results: After the tail reaches the end of the
	       allocated memory area, it rolls over
	       to the beginning and starts removing
	       elements from the beginning
*******************************************************/

void wrap_remove(void **state)
{
	uint8_t data_values[]={1,2,3,4};
	uint8_t deleted_item;
	CircBuf_t *test_buffer;
	test_buffer = (CircBuf_t *)malloc(sizeof(CircBuf_t));
	assert_non_null(test_buffer);
	CB_Enum_t status = circbuf_init(test_buffer, 4);
	assert_int_equal(status, NO_ERROR);
	/*Add array elements to the buffer*/
	for(uint8_t i=0; i<4; i++)
	{
		status = circbuf_add(test_buffer, *(data_values+i));
		assert_int_equal(status, NO_ERROR);
	}
	/*Remove some elements to make space for new elements*/
	for(uint8_t i=0; i<2; i++)
	{
		status = circbuf_remove(test_buffer, &deleted_item);
		assert_int_equal(status, NO_ERROR);
	}
	/*Add more elements to fill up the buffer*/
	for(uint8_t i=0; i<2; i++)
	{
		status = circbuf_add(test_buffer, *(data_values+i));
		assert_int_equal(status, NO_ERROR);
	}
	/*Remove every element in the buffer so that tails rolls over and removes elements*/
	for(uint8_t i=0; i<4; i++)
	{
		status = circbuf_remove(test_buffer, &deleted_item);
		assert_int_equal(status, NO_ERROR);
	}
	status = circbuf_empty(test_buffer);
	assert_int_equal(status, BUFFER_EMPTY);

}


int main(int argc, char **argv)
{
	const struct CMUnitTest tests[] = 
	{
		cmocka_unit_test(test_allocate_free),
		cmocka_unit_test(test_valid_buffer),
		cmocka_unit_test(test_init_buffer),
		cmocka_unit_test(test_buffer_full),
		cmocka_unit_test(test_buffer_empty),
		cmocka_unit_test(test_buffer_overfill),
		cmocka_unit_test(test_buffer_overempty),
		cmocka_unit_test(test_buffer_add_remove),
		cmocka_unit_test(wrap_add),
		cmocka_unit_test(wrap_remove),
	};

  	return cmocka_run_group_tests(tests, NULL, NULL);
}
