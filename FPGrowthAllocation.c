/*
 * FPGrowthAllocation.c
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */

#include "FPGrowth.h"
#include "FPGrowthAllocation.h"
#include "AprioriUtilities.h"

header_table_item_t* allocate_null_header_table_item() {
	header_table_item_t* new_header_table_item = malloc(sizeof(header_table_item_t));
	new_header_table_item->prev_item = NULL;
	new_header_table_item->next_item = NULL;
	new_header_table_item->tree_node = NULL;
	return new_header_table_item;
}

header_table_t allocate_null_header_table() {
	int item_num;
	header_table_t new_header_table = malloc(sizeof(header_table_list_t) * NUM_ITEMS);

	header_table_list_t null_list = {NULL, NULL, num_elements: 0};

	for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
		new_header_table[item_num] = null_list;
	}

	return new_header_table;
}

tree_t* allocate_null_tree() {
	static int num_trees = 0;

	tree_t* new_tree = (tree_t*)malloc(sizeof(tree_t));
	new_tree->header_table = allocate_null_header_table();
	new_tree->root_node = NULL;
	new_tree->max_item_num = NUM_ITEMS;

	if (++num_trees % 1000000 == 0) {
		printf("Total Tree Copies: %d\n", num_trees);
	}

	return new_tree;
}

node_t** allocate_null_child_nodes() {
	int item_index;

	node_t** child_nodes = (node_t**)malloc(sizeof(node_t*) * NUM_ITEMS);

	for (item_index = 0; item_index < NUM_ITEMS; ++item_index) {
		child_nodes[item_index] = NULL;
	}


	return child_nodes;
}

node_t* allocate_null_node() {
	node_t* new_node = (node_t*)malloc(sizeof(node_t));

	new_node->parent_node = NULL;
	new_node->count = 0;
	new_node->child_nodes = allocate_null_child_nodes();
	new_node->node_id = ++total_nodes_attached;
	new_node->item_num = -1;

//	if (total_nodes_attached % 200 == 0) {
//		printf("Total Nodes: %d\n", total_nodes_attached);
//	}

	return new_node;
}

void deallocate_header_table_list(header_table_list_t* header_table_list) {
	header_table_item_t* next_item, *current_item;
	current_item = header_table_list->head;

	while (current_item != NULL) {
		next_item = current_item->next_item;
		remove_item_from_header_table_list(current_item, header_table_list);
		current_item = next_item;
	}

//	printf("Num header list items remaining: %d\n", header_table_list->num_elements);

}

void deallocate_header_table(header_table_t header_table) {
	int item_num;

	for (item_num = NUM_ITEMS - 1; item_num >= 0; --item_num) {
		deallocate_header_table_list(&header_table[item_num]);
	}

	free(header_table);
}


void remove_node_from_tree(node_t* node_to_remove) {
	int child_index;
	node_t* child_node, *parent_node;

	parent_node = node_to_remove->parent_node;

	if (parent_node != NULL) {
		parent_node->child_nodes[node_to_remove->item_num] = NULL;
	}

	for (child_index = 0; child_index < NUM_ITEMS; ++child_index) {
		child_node = node_to_remove->child_nodes[child_index];

		if (child_node != NULL) {
			child_node->parent_node = parent_node;
		}

	}

	free(node_to_remove->child_nodes);
	free(node_to_remove);

}

void deallocate_tree(tree_t* tree) {
	start_timer();

	deallocate_header_table(tree->header_table);
	remove_node_from_tree(tree->root_node);


	free(tree);

	deallocate_tree_time += get_elapsed_time();
}

frequent_itemset_list_item_t* allocate_null_frequent_itemset_list_item() {
	frequent_itemset_list_item_t* new_item = malloc(sizeof(frequent_itemset_list_item_t));
	new_item->next_item = NULL;
	new_item->prev_item = NULL;
	return new_item;
}

frequent_itemset_list_item_t* allocate_frequent_itemset_list_item(transaction_t itemset, int support) {
	frequent_itemset_list_item_t* new_item = allocate_null_frequent_itemset_list_item();
	new_item->itemset = itemset;
	new_item->support = support;
	return new_item;
}

void deallocate_frequent_itemset_list_item(frequent_itemset_list_item_t* item_to_dealloc) {
	deallocate_transaction(item_to_dealloc->itemset);
}

void deallocate_frequent_itemsets_list(frequent_itemsets_list_t* frequent_itemsets_list) {
	frequent_itemset_list_item_t* current_item, *next_item;

	current_item = frequent_itemsets_list->head;

	while (current_item != NULL) {
		next_item = current_item->next_item;
		deallocate_frequent_itemset_list_item(current_item);
		current_item = next_item;
	}

}

tree_t* allocate_new_tree(node_t* root_node, int max_item_num) {
	tree_t* new_tree = allocate_null_tree();

	new_tree->root_node = root_node;
	new_tree->max_item_num = max_item_num;

	return new_tree;
}


