/*
 * SwapSort.h
 *
 *  Created on: Apr 20, 2016
 *      Author: durhamsm
 */

#ifndef SWAPSORT_H_
#define SWAPSORT_H_

#include "FrequentItemSet.h"
#include "ReadData.h"

typedef struct item_count_t {
	int count;
	int item_num;
} item_count_t;

transaction_t* get_swap_sorted_transactions(item_count_t* sorted_item_count_array, transaction_t* transactions);

item_count_t* get_sorted_item_count_array(transaction_t* transactions);

void deallocate_item_count_array(item_count_t* item_count_array);

#endif /* SWAPSORT_H_ */
