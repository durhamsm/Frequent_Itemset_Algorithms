/*
 * FPGrowthAllocation.h
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */

#ifndef FPGROWTHALLOCATION_H_
#define FPGROWTHALLOCATION_H_


header_table_item_t* allocate_null_header_table_item();

tree_t* allocate_null_tree();

node_t** allocate_null_child_nodes();

node_t* allocate_null_node();

void deallocate_header_table_list(header_table_list_t* header_table_list);

void deallocate_header_table_item_list(header_table_item_t* header_table_item);

void deallocate_header_table(header_table_t header_table);

void deallocate_tree(tree_t* tree);

frequent_itemset_list_item_t* allocate_null_frequent_itemset_list_item();

frequent_itemset_list_item_t* allocate_frequent_itemset_list_item(transaction_t itemset, int support);

void deallocate_frequent_itemsets_list(frequent_itemsets_list_t* frequent_itemsets_list);

header_table_t allocate_null_header_table();

tree_t* allocate_new_tree(node_t* root_node, int max_item_num);

void remove_node_from_tree(node_t* node_to_remove);

#endif /* FPGROWTHALLOCATION_H_ */
