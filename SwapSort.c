/*
 * SwapSort.c
 *
 *  Created on: Apr 20, 2016
 *      Author: durhamsm
 */

#include "SwapSort.h"
#include "Utilities.h"

void print_item_count_array(item_count_t* item_count_array) {
	int item_num;

	for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
		printf("Item Num: %d, Count: %d\n", item_count_array[item_num].item_num, item_count_array[item_num].count);
	}


}

transaction_t* get_swap_sorted_transactions(item_count_t* sorted_item_count_array, transaction_t* transactions) {
	int item_num, trans_index;
	transaction_t* sorted_transactions;

	sorted_transactions = allocate_int_matrix(dataset_params.num_trans, dataset_params.num_items);
	sorted_transactions[0][0] = 0;

	for (trans_index = 0; trans_index < NUM_TRANS; ++trans_index) {
		for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
			sorted_transactions[trans_index][item_num] = transactions[trans_index][sorted_item_count_array[item_num].item_num];
		}
	}

	return sorted_transactions;

}

item_count_t* get_null_item_count_array() {
	int item_num;
	item_count_t* item_count_array = malloc(sizeof(item_count_t) * NUM_ITEMS);

	for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
		item_count_array[item_num].item_num = item_num;
		item_count_array[item_num].count = 0;
	}

	return item_count_array;

}

item_count_t* get_item_count_array(transaction_t* transactions) {
	int item_num, trans_id;

	item_count_t* item_count_array = get_null_item_count_array();

	for (trans_id = 0; trans_id < NUM_TRANS; ++trans_id) {
		for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
			item_count_array[item_num].count += transactions[trans_id][item_num];
		}
	}

	return item_count_array;
}

void swap_item_counts(item_count_t* item1, item_count_t* item2) {
	item_count_t temp_item_count;

	temp_item_count = *item1;
	*item1 = *item2;
	*item2 = temp_item_count;
}


item_count_t* get_sorted_item_count_array(transaction_t* transactions) {
	int item_num, item_num_start;
	item_count_t* item_count_array = get_item_count_array(transactions);

//	print_item_count_array(item_count_array);

	for (item_num_start = 1; item_num_start < NUM_ITEMS; ++item_num_start) {
		for (item_num = item_num_start; item_num > 0; --item_num) {

			if (item_count_array[item_num].count > item_count_array[item_num - 1].count) {
				swap_item_counts(&(item_count_array[item_num]), &(item_count_array[item_num - 1]));
			} else {
				break;
			}
		}
	}

//	print_item_count_array(item_count_array);

	return item_count_array;

}


void deallocate_item_count_array(item_count_t* item_count_array) {
	free(item_count_array);
}
