/*
 ============================================================================
 Name        : FPGrowth.c
 Author      : Sam Durham
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "FPGrowth.h"
#include "FPGrowthPrinting.h"
#include "FPGrowthAllocation.h"
#include "Apriori.h"
#include "Utilities.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include "FrequentItemsetUtilities.h"
#include "CountingSort.h"
#include "FPGrowth.h"
#include "SwapSort.h"

int total_nodes_attached = 0;
long sort_time, build_original_tree_time, tree_copy_time, add_1_items_time, misc_time, deallocate_tree_time;


frequent_itemsets_list_t frequent_itemsets_list = {NULL, NULL, 0};
//header_table_t header_table;

void append_to_header_table_list(node_t* tree_node, header_table_list_t* header_table_list) {

	header_table_item_t* new_header_table_item;

	new_header_table_item = allocate_null_header_table_item();
	new_header_table_item->tree_node = tree_node;

	new_header_table_item->prev_item = header_table_list->tail;

	if (header_table_list->head == NULL) {
		header_table_list->head = new_header_table_item;
		header_table_list->tail = new_header_table_item;
	} else {
		header_table_list->tail->next_item = new_header_table_item;
	}

	header_table_list->tail = new_header_table_item;

	++header_table_list->num_elements;
}

void attach_child(node_t* parent, node_t* child, int item_num, header_table_t header_table) {

	parent->child_nodes[item_num] = child;
	child->parent_node = parent;
	child->item_num = item_num;

	append_to_header_table_list(child, &header_table[item_num]);

}

void add_transaction(tree_t* target_tree, transaction_t transaction) {

	int item_num;

	node_t* current_node = target_tree->root_node;
	node_t* new_child_node;

	for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {

		if (transaction[item_num] == 0) {
			continue;
		}

		if (current_node->child_nodes[item_num] == NULL) {
			new_child_node = allocate_null_node();
			attach_child(current_node, new_child_node, item_num, target_tree->header_table);
		}

		current_node = current_node->child_nodes[item_num];
		++current_node->count;

	}

}

void add_all_transactions_to_tree(transaction_t* transactions, tree_t* target_tree) {

	int trans_index;

	for (trans_index = 0; trans_index < NUM_TRANS; ++trans_index) {
		add_transaction(target_tree, transactions[trans_index]);
	}

}

int get_item_count(header_table_list_t* header_table_list) {
	int total_count = 0;
	header_table_item_t *current_item, *next_item;
	current_item = header_table_list->head;

	while (current_item != NULL) {
		next_item = current_item->next_item;
		total_count += current_item->tree_node->count;

		if (current_item->tree_node->count == 0) {
			remove_item_from_header_table_list(current_item, header_table_list);
		}

		current_item = next_item;
	}

	return total_count;
}


node_t* copy_node(node_t* node_to_copy, tree_t* tree_copy) {
	int child_node_id;
	node_t* child_node, *child_node_to_copy;
	header_table_t header_table;

	header_table = tree_copy->header_table;

	node_t* node_copy = allocate_null_node();
	node_copy->count = node_to_copy->count;

	for (child_node_id = 0; child_node_id < NUM_ITEMS; ++child_node_id) {
		child_node_to_copy = node_to_copy->child_nodes[child_node_id];

		if (child_node_id > tree_copy->max_item_num) {
			break;
		}

		if (child_node_to_copy != NULL) {
			child_node = copy_node(child_node_to_copy, tree_copy);
			attach_child(node_copy, child_node, child_node_id, header_table);
		}

	}

	return node_copy;

}






tree_t* get_tree_copy(tree_t* tree_to_copy, int max_item_num) {

	start_timer();

	tree_t* tree_copy = allocate_null_tree();
	tree_copy->max_item_num = max_item_num;
	tree_copy->root_node = copy_node(tree_to_copy->root_node, tree_copy);

	tree_copy_time += get_elapsed_time();

	return tree_copy;
}

transaction_t get_transaction_copy(transaction_t transaction_to_copy) {

	transaction_t new_transaction = allocate_null_transaction();
	memcpy(new_transaction, transaction_to_copy, sizeof(int) * NUM_ITEMS);

	return new_transaction;
}

void add_to_count_of_all_parents(node_t* start_node) {
	node_t* current_parent_node;

	current_parent_node = start_node->parent_node;

	while (current_parent_node != NULL) {
		current_parent_node->count += start_node->count;
		current_parent_node = current_parent_node->parent_node;
	}

}

//void remove_child_node_from_parent(node_t* node_to_remove) {
//
//
//
//}



void remove_item_from_header_table_list(header_table_item_t* item_to_remove, header_table_list_t* header_table_list) {

	remove_node_from_tree(item_to_remove->tree_node);

	if (item_to_remove == header_table_list->head && item_to_remove == header_table_list->tail) {
		header_table_list->head = NULL;
		header_table_list->tail = NULL;
	} else if (item_to_remove == header_table_list->head) {
		header_table_list->head = item_to_remove->next_item;
		item_to_remove->next_item->prev_item = NULL;
	} else if (item_to_remove == header_table_list->tail) {
		header_table_list->tail = item_to_remove->prev_item;
		item_to_remove->prev_item->next_item = NULL;
	} else {
		item_to_remove->prev_item->next_item = item_to_remove->next_item;
		item_to_remove->next_item->prev_item = item_to_remove->prev_item;
	}

	free(item_to_remove);
	--header_table_list->num_elements;

}

void cut_last_item_num_from_tree(tree_t* tree) {
	header_table_list_t* header_table_list;
	header_table_item_t* item_to_cut, *next_item_to_cut;

	header_table_list = &tree->header_table[tree->max_item_num];
	item_to_cut = header_table_list->head;

	while (item_to_cut != NULL) {
		add_to_count_of_all_parents(item_to_cut->tree_node);

		next_item_to_cut = item_to_cut->next_item;
		remove_item_from_header_table_list(item_to_cut, header_table_list);
		item_to_cut = next_item_to_cut;
	}

}



void append_to_frequent_itemsets_list(frequent_itemset_list_item_t* new_item, frequent_itemsets_list_t* list) {

	if (list->head == NULL) {
		list->head = new_item;
		list->tail = new_item;
	} else {
		new_item->prev_item = list->tail;
		list->tail->next_item = new_item;
		list->tail = new_item;
	}



	++(list->num_elements);

//	if (list->num_elements % 200 == 0) {
//		printf("Num Freq Sets: %d\n", list->num_elements);
//	}

}



void remove_nodes_with_zero_count_from_header_list(header_table_list_t* header_table_list) {
	header_table_item_t *current_header_table_item, *next_header_table_item;
	current_header_table_item = header_table_list->head;

	while (current_header_table_item != NULL) {
		next_header_table_item = current_header_table_item->next_item;

		if (current_header_table_item->tree_node->count == 0) {
			remove_item_from_header_table_list(current_header_table_item, header_table_list);
		}

		current_header_table_item = next_header_table_item;
	}

}

void remove_nodes_with_zero_count(tree_t* tree) {
	int item_num;
	header_table_list_t* current_header_table_list;

	for (item_num = tree->max_item_num - 1; item_num >= 0; --item_num) {
		current_header_table_list = &tree->header_table[item_num];
		remove_nodes_with_zero_count_from_header_list(current_header_table_list);
	}
}

void add_frequent_itemsets_to_list(tree_t* tree, transaction_t base_itemset) {
	int item_num, support, is_header_list_removable;
	header_table_list_t* current_header_table_list;
	transaction_t new_base_itemset;
	frequent_itemset_list_item_t* new_item;

	is_header_list_removable = 1;

	for (item_num = tree->max_item_num - 1; item_num >= 0; --item_num) {
		current_header_table_list = &tree->header_table[item_num];

//		remove_nodes_with_zero_count_from_header_list(current_header_table_list);
		support = get_item_count(current_header_table_list);

		if (support >= MIN_SUP) {
			is_header_list_removable = 0;
			new_base_itemset = get_transaction_copy(base_itemset);
			new_base_itemset[item_num] = 1;

			new_item = allocate_frequent_itemset_list_item(new_base_itemset, support);
			append_to_frequent_itemsets_list(new_item, &frequent_itemsets_list);
		} else if (is_header_list_removable) {
			int c = 3;
//			deallocate_header_table_list(current_header_table_list);
		}
	}

}



//void set_all_parent_nodes_to_zero(node_t* node) {
//	node_t* current_node = node->parent_node;
//
//	while (current_node != NULL) {
//		current_node->count = 0;
//		current_node = current_node->parent_node;
//	}
//
//}
//
//void set_all_upper_nodes_to_zero(header_table_list_t* header_table_list) {
//	header_table_item_t* current_header_table_item;
//	current_header_table_item = header_table_list->head;
//
//	while (current_header_table_item != NULL) {
//		set_all_parent_nodes_to_zero(current_header_table_item->tree_node);
//		current_header_table_item = current_header_table_item->next_item;
//	}
//}
//
//void set_all_upper_nodes_to_zero(tree_t* tree) {
////	header_table_list_t* header_table_list;
//
//}

void set_all_child_node_counts_to_zero(node_t* node, int stop_item_num) {
	int child_node_id;
	node_t* child_node;

	for (child_node_id = 0; child_node_id < NUM_ITEMS; ++child_node_id) {
		child_node = node->child_nodes[child_node_id];

		if (child_node_id == stop_item_num) {
			break;
		}

		if (child_node != NULL) {
			child_node->count = 0;
			set_all_child_node_counts_to_zero(child_node, stop_item_num);
		}

	}
}

int recursive_call_count = 0;

void find_frequent_itemsets(tree_t* tree, int first_item_num_to_prune, transaction_t base_itemset) {
	int item_num_to_cut;
	tree_t* copied_tree;
	transaction_t new_base_itemset;


	for (item_num_to_cut = first_item_num_to_prune; item_num_to_cut > 0; --item_num_to_cut) {

		if (first_item_num_to_prune == NUM_ITEMS - 1) {
			printf("Begin Branch #%d\n", NUM_ITEMS - item_num_to_cut);
		}

		new_base_itemset = get_transaction_copy(base_itemset);
		new_base_itemset[item_num_to_cut] = 1;

		copied_tree = get_tree_copy(tree, item_num_to_cut);

		start_timer();

		set_all_child_node_counts_to_zero(copied_tree->root_node, item_num_to_cut);

//		if (is_transactions_equal(transaction_of_interest, base_itemset)) {
//			print_tree_to_file(copied_tree, "original_tree.dot");
//			int b = 5;
//		}

		cut_last_item_num_from_tree(copied_tree);
//		remove_nodes_with_zero_count(copied_tree);


//		if (is_transactions_equal(transaction_of_interest, base_itemset)) {
//			print_tree_to_file(copied_tree, "original_tree.dot");
//			int b = 5;
//		}

		add_frequent_itemsets_to_list(copied_tree, new_base_itemset);

		misc_time += get_elapsed_time();

		find_frequent_itemsets(copied_tree, item_num_to_cut - 1, new_base_itemset);
	}

	deallocate_tree(tree);
	free(base_itemset);

}



void add_1_item_frequent_itemsets(tree_t* tree, item_count_t* item_count_array) {
	transaction_t transaction;
	frequent_itemset_list_item_t* new_item;
	header_table_list_t* current_header_table_list;
	int item_num, support;

	for (item_num = NUM_ITEMS - 1; item_num >= 0; --item_num) {
		current_header_table_list = &tree->header_table[item_num];
		support = item_count_array[item_num].count;

		if (support >= MIN_SUP) {
			transaction = allocate_null_transaction();
			transaction[item_num] = 1;
			new_item = allocate_frequent_itemset_list_item(transaction, support);
			append_to_frequent_itemsets_list(new_item, &frequent_itemsets_list);
		} else {
			deallocate_header_table_list(current_header_table_list);
		}

	}

}

void print_fp_analysis_report(frequent_itemsets_list_t* frequent_itemset_list, long elapsed_time_nanos) {
	printf("Num Frequent Itemsets: %d\n", frequent_itemset_list->num_elements);
	printf("Elapsed Time (sec): %f\n", (float)elapsed_time_nanos / 1E9);
}

void print_fp_analysis_report_all_times(frequent_itemsets_list_t* frequent_itemset_list, long sort_time, long build_original_tree_time, long tree_copy_time, long add_transactions_time, long misc_time, long deallocate_tree_time) {
	float total_time = sort_time + build_original_tree_time + tree_copy_time + add_transactions_time + misc_time + deallocate_tree_time;

	printf("Num Frequent Itemsets: %d\n", frequent_itemset_list->num_elements);
	printf("%-40s%5f\n", "sort_time (sec):", (float)sort_time / 1E9);
	printf("%-40s%5f\n", "build_original_tree_time (sec):", (float)build_original_tree_time / 1E9);
	printf("%-40s%5f\n", "tree_copy_time (sec):", (float)tree_copy_time / 1E9);
	printf("%-40s%5f\n", "add_transactions_time (sec):", (float)add_transactions_time / 1E9);
	printf("%-40s%5f\n", "misc_time (sec):", (float)misc_time / 1E9);
	printf("%-40s%5f\n", "deallocate_tree_time (sec):", (float)deallocate_tree_time / 1E9);
	printf("%-40s%5f\n", "total_time (sec):", (float)total_time / 1E9);
}


item_count_t* sorted_item_count_array;
transaction_t translation_transaction;

void fp_analysis() {

	transaction_t* sorted_transactions;
	misc_time = 0;
	deallocate_tree_time = 0;
	//all times in units of nanoseconds

	start_timer();

	sorted_item_count_array = get_sorted_item_count_array(dataset.transactions);
	sorted_transactions = get_swap_sorted_transactions(sorted_item_count_array, dataset.transactions);
	translation_transaction = get_translation_transaction(sorted_item_count_array);

	sort_time = get_elapsed_time();

	node_t* root_node = allocate_null_node();
	tree_t* original_tree = allocate_new_tree(root_node, NUM_ITEMS);


	start_timer();

	add_all_transactions_to_tree(sorted_transactions, original_tree);
//	print_tree_to_file(original_tree, "original_tree.dot");
	build_original_tree_time = get_elapsed_time();

	start_timer();

	printf("Total nodes in Original Tree: %d\n", total_nodes_attached);
	add_1_item_frequent_itemsets(original_tree, sorted_item_count_array);
	transaction_t empty_transaction = allocate_null_transaction();

	add_1_items_time = get_elapsed_time();


	tree_copy_time = 0;
	find_frequent_itemsets(original_tree, NUM_ITEMS - 1, empty_transaction);

//	print_frequent_itemsets_fpgrowth(&frequent_itemsets_list, sorted_item_count_array);
//	print_fp_analysis_report(&frequent_itemsets_list, elapsed_time_nanos);
	print_fp_analysis_report_all_times(&frequent_itemsets_list, sort_time, build_original_tree_time, tree_copy_time, add_1_items_time, misc_time, deallocate_tree_time);

	//deallocation
	deallocate_frequent_itemsets_list(&frequent_itemsets_list);
	deallocate_transactions(sorted_transactions, NUM_TRANS);
	deallocate_item_count_array(sorted_item_count_array);
	free(translation_transaction);
//
//
//

	printf("\nSuccessfully Finished FP Analysis\n");
}


