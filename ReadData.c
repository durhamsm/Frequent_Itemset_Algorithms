

#include "ReadData.h"
#include "Utilities.h"
#include "FrequentItemsetUtilities.h"
#include <time.h>

#define ARRAYSIZE(x)  (sizeof(x)/sizeof(*(x)))

dataset_t dataset;
int NUM_ITEMS, NUM_TRANS, MAX_RULE_SIZE;

const char* getfield(char* line, int num) {

    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")) {
        if (!--num)
            return tok;
    }
    return NULL;

}



int is_prob_dependence = 1;
float max_prob_item_exists = 0.7;
float first_item_prob = 0.1;
//float first_item_prob = 0.1;

int is_create_first_item() {
	return get_random_number_between_zero_and_one() < first_item_prob;
}

int is_create_item(int exists_prev_item) {
	if (is_prob_dependence) {
		if (exists_prev_item) {
			return get_random_number_between_zero_and_one() < max_prob_item_exists;
		} else {
			return get_random_number_between_zero_and_one() < ((float)1 - max_prob_item_exists);
		}
	} else {
		return get_random_number_between_zero_and_one() < max_prob_item_exists;
	}

}


void fill_transactions_artificially(dataset_t dataset) {
	int trans_id, item_num, create_item;
	transaction_t* transactions;

	srand(time(NULL));
	transactions = dataset.transactions;

	for (trans_id = 0; trans_id < NUM_TRANS; ++trans_id) {

		transactions[trans_id][0] = is_create_first_item();

		for (item_num = 1; item_num < NUM_ITEMS; ++item_num) {
			create_item = is_create_item(transactions[trans_id][item_num - 1]);
			transactions[trans_id][item_num] = create_item;
		}
	}
}


void initialize_dataset(dataset_params_t dataset_params) {
	dataset.params = dataset_params;
	NUM_ITEMS = dataset_params.num_items;
	NUM_TRANS = dataset_params.num_trans;
	MAX_RULE_SIZE = dataset_params.max_rule_size;
	dataset.transactions = allocate_int_matrix(dataset_params.num_trans, dataset_params.num_items);
}

void read_data(dataset_params_t dataset_params) {

//	initialize_dataset(dataset_params);

	FILE *file = fopen(dataset_params.file_path, "r");
	if ( file ) {

		size_t i, j, k;
		char buffer[BUFSIZ], *ptr;
		/*
		* Read each line from the file.
		*/
		for ( i = 0; fgets(buffer, sizeof buffer, file); ++i ) {
		 /*
		  * Parse the comma-separated values from each line into 'array'.
		  */

			ptr = buffer;
			(int)strtol(ptr, &ptr, 10);
			++ptr;
			for ( j = 0; j < dataset_params.num_items; ++j, ++ptr ) {
				dataset.transactions[i][j] = (int)strtol(ptr, &ptr, 10);
			}
		}
		fclose(file);
		/*
		* Print the data in 'array'.
		*/
//		for ( j = 0; j < i; ++j ) {
//			for ( k = 0; k < dataset_params.num_items; ++k ) {
//				printf("%4d ", dataset.transactions[j][k]);
//			}
//			putchar('\n');
//		}
	}
	else /* fopen() returned NULL */ {
		perror(dataset_params.file_path);
	}


}





void load_data(dataset_params_t dataset_params) {
	initialize_dataset(dataset_params);
	fill_transactions_artificially(dataset);
//	read_data(dataset_params);
//	print_all_transactions(dataset.transactions);


}
