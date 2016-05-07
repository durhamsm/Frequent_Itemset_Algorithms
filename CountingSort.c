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

int get_max_in_array(int* array, int array_length) {
	int array_index;
	int max = array[0];

	for (array_index = 0; array_index < array_length; ++array_index) {
		if (array[array_index] > max) {
			max = array[array_index];
		}
	}

	return max;

}


//void remap_item_strings(int* item_ids_sorted_by_frequency) {
//	int item_id;
//
//	printf("Remapped items: \n");
//
//	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
//		item_strings_remapped[item_id] = item_strings[item_ids_sorted_by_frequency[item_id]];
//		printf("%s ", item_strings[item_ids_sorted_by_frequency[item_id]]);
//	}
//
//	printf("\n");
//}

int* get_counting_sort_map() {

	int item_id;

	int max_count = get_max_in_array(item_counts, NUM_ITEMS);
	int length_utility_arrays = max_count + 1;

	int* item_frequency_counts = malloc(sizeof(int)*length_utility_arrays);
	int* cum_item_frequency_counts = malloc(sizeof(int)*length_utility_arrays);
	int* item_ids_sorted_by_frequency = malloc(sizeof(int)*NUM_ITEMS);


	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		++item_frequency_counts[item_counts[item_id]];
	}

	cum_item_frequency_counts[0] = item_frequency_counts[0];

	for (item_id = 1; item_id < NUM_ITEMS; ++item_id) {
		cum_item_frequency_counts[item_id] = cum_item_frequency_counts[item_id - 1] + item_frequency_counts[item_id];
	}

	for (item_id = NUM_ITEMS - 1; item_id >= 0; --item_id) {
		int cum_item_frequency_count = cum_item_frequency_counts[item_counts[item_id]]--;
		item_ids_sorted_by_frequency[cum_item_frequency_count - 1] = item_id;
	}

	free(item_frequency_counts);
	free(cum_item_frequency_counts);


//	remap_item_strings(item_ids_sorted_by_frequency);

	return item_ids_sorted_by_frequency;

}



void counting_sort(transaction_t* transactions_unsorted, transaction_t* transactions_sorted) {
	int sort_map_index, trans_index;
	int* sort_map = get_counting_sort_map();

	for (trans_index = 0; trans_index < NUM_TRANS; ++trans_index) {

		for (sort_map_index = 0; sort_map_index < NUM_ITEMS; ++sort_map_index) {
			transactions_sorted[trans_index][sort_map_index] = transactions_unsorted[trans_index][sort_map[sort_map_index]];
		}

	}

//	print_transaction_bin_vecs(items_sorted, num_transactions);
//	print_array(item_counts, NUM_ITEMS, "item counts");
//	print_array(item_frequency_counts, NUM_ITEMS, "item_frequency_counts");
//	print_array(cum_item_frequency_counts, NUM_ITEMS, "cum_item_frequency_counts");
//	print_array(item_ids_sorted_by_frequency, NUM_ITEMS, "item_ids_sorted_by_frequency");

}

transaction_t* sort_transaction_items_by_frequency(transaction_t* transactions) {

	transaction_t* transactions_sorted = allocate_int_matrix(NUM_TRANS, NUM_ITEMS);

	counting_sort(transactions, transactions_sorted);

	return transactions_sorted;

}


