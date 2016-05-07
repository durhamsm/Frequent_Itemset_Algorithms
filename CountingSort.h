/*
 * CountingSort.h
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */

#ifndef COUNTINGSORT_H_
#define COUNTINGSORT_H_

/*
 * CountingSort.c
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */


#include "Apriori.h"
#include "Utilities.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include "FrequentItemsetUtilities.h"

int item_counts[50];

int get_max_in_array(int* array, int array_length);

int* get_counting_sort_map();

void counting_sort(transaction_t* transactions_unsorted, transaction_t* transactions_sorted);

transaction_t* sort_transaction_items_by_frequency(transaction_t* transactions);

#endif /* COUNTINGSORT_H_ */
