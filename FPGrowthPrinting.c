/*
 * FPGrowthPrinting.c
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SwapSort.h"
#include "FPGrowth.h"

void print_edge_to_file(node_t* parent_node, node_t* child_node, int item_id, FILE* tring_print_file) {
	char* label_base = "%d [label=\"%s: %d\" color=Blue, fontcolor=Red]";
	char label[100];

	sprintf(label, label_base, child_node->node_id, column_headers[item_id], child_node->count);
	fprintf(tring_print_file, label);
	fprintf(tring_print_file, "\t%d -> %d;\n", parent_node->node_id, child_node->node_id);

}

void recursive_print_node_to_file(node_t* node_to_print, FILE* tring_print_file) {
	int child_index;
	node_t* child_node;


	for (child_index = 0; child_index < NUM_ITEMS; ++child_index) {

		child_node = node_to_print->child_nodes[translation_transaction[child_index]];

		if (child_node != NULL) {
			print_edge_to_file(node_to_print, child_node, child_index, tring_print_file);

			recursive_print_node_to_file(child_node, tring_print_file);
		}

	}

}

void print_root_node_label(node_t* root_node, FILE* tring_print_file) {
	char label[100];
	sprintf(label, "%d [label=\"root\" color=Blue, fontcolor=Red]", root_node->node_id);
	fprintf(tring_print_file, label);
}

void dot_to_ps_image(char* file_name) {
	char command[100];
	sprintf(command, "dot -Tps %s -o %s.ps", file_name, file_name);
	system(command);
}

void print_tree_to_file(tree_t* tree_to_print, char* file_name) {
	FILE* tring_print_file = fopen(file_name, "w");
	fprintf(tring_print_file, "digraph tring_graph {\n");

	print_root_node_label(tree_to_print->root_node, tring_print_file);


	recursive_print_node_to_file(tree_to_print->root_node, tring_print_file);

	fprintf(tring_print_file, "}\n");
	fclose(tring_print_file);

	dot_to_ps_image(file_name);

}

transaction_t get_translation_transaction(item_count_t* item_count_array) {
	int item_id;
	transaction_t translation_transaction;

	translation_transaction = allocate_null_transaction();

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		translation_transaction[item_count_array[item_id].item_num] = item_id;
	}

	return translation_transaction;
}

void print_translated_transaction(transaction_t transaction, transaction_t translation_transaction) {
	int item_id;


	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		printf("%4d ", transaction[translation_transaction[item_id]]);
	}


}

void print_frequent_itemsets_fpgrowth(frequent_itemsets_list_t* frequent_itemsets_list, item_count_t* item_count_array) {
	frequent_itemset_list_item_t* current_item;
//	transaction_t translation_transaction;

//	translation_transaction = get_translation_transaction(item_count_array);
	current_item = frequent_itemsets_list->head;

	while (current_item != NULL) {
//		print_transaction(current_item->itemset);
		print_translated_transaction(current_item->itemset, translation_transaction);
		printf(" Support: %d\n", current_item->support);
		current_item = current_item->next_item;
	}

//	free(translation_transaction);

}
