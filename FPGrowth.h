

#ifndef FP_GROWTH_H_
#define FP_GROWTH_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Apriori.h"
#include "Utilities.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include "FrequentItemsetUtilities.h"


extern int total_nodes_attached;
extern long deallocate_tree_time;
extern transaction_t translation_transaction;

typedef struct frequent_itemset_list_item_t {
	transaction_t itemset;
	struct frequent_itemset_list_item_t* prev_item;
	struct frequent_itemset_list_item_t* next_item;
	int support;
} frequent_itemset_list_item_t;

typedef struct frequent_itemsets_list_t {
	frequent_itemset_list_item_t* head;
	frequent_itemset_list_item_t* tail;
	int num_elements;
} frequent_itemsets_list_t;

typedef struct node_t {

	int count;
	struct node_t** child_nodes;
	struct node_t* parent_node;
	int item_num;
	int node_id;

} node_t;


typedef struct header_table_item_t {

	node_t* tree_node;
	struct header_table_item_t* prev_item;
	struct header_table_item_t* next_item;

} header_table_item_t;

typedef struct header_table_list_t {
	header_table_item_t* head;
	header_table_item_t* tail;
	int num_elements;
} header_table_list_t;


typedef header_table_list_t* header_table_t;


typedef struct tree_t {

	node_t* root_node;
	header_table_t header_table;
	int max_item_num;

} tree_t;


void fp_analysis();

void remove_item_from_header_table_list(header_table_item_t* item_to_remove, header_table_list_t* header_table_list);


#endif

