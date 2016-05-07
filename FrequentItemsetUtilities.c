

#include "FrequentItemsetUtilities.h"
#include "ReadData.h"
#include <time.h>


struct timespec time_start, time_stop;

transaction_t allocate_new_transaction() {
	return malloc(sizeof(int)*NUM_ITEMS);
}

transaction_t allocate_null_transaction() {
	transaction_t new_trans = allocate_new_transaction();
	memset(new_trans, 0, NUM_ITEMS * sizeof(int));
	return new_trans;
}

void print_transaction(transaction_t transaction) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		printf("%4d ", transaction[item_id]);
	}

}

void print_all_transactions(transaction_t* transactions) {
	int trans_id;

	for (trans_id = 0; trans_id < NUM_TRANS; ++trans_id) {

		print_transaction(transactions[trans_id]);
		putchar('\n');
	}

	putchar('\n');
	putchar('\n');
}

void deallocate_item(void* item) {
	printf("Item deallocated\n");
	free(item);
}



long get_time_diff_as_nanoseconds(struct timespec time1, struct timespec time2) {
	return 1000000000 * (time2.tv_sec - time1.tv_sec) + time2.tv_nsec - time1.tv_nsec;
}

void start_timer() {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
}

long get_elapsed_time() {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stop);
	return get_time_diff_as_nanoseconds(time_start, time_stop);
}

float get_random_number_between_zero_and_one() {
	return (float)rand() / RAND_MAX;
}

int is_transactions_equal(transaction_t first_transaction, transaction_t second_transaction) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (first_transaction[item_id] != second_transaction[item_id]) {
			return 0;
		}
	}

	return 1;
}


