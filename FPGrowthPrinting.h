/*
 * FPGrowthPrinting.h
 *
 *  Created on: Apr 19, 2016
 *      Author: durhamsm
 */

#ifndef FPGROWTHPRINTING_H_
#define FPGROWTHPRINTING_H_

#include "SwapSort.h"

void print_edge_to_file(node_t* parent_node, node_t* child_node, int item_id, FILE* tring_print_file);

void recursive_print_node_to_file(node_t* node_to_print, FILE* tring_print_file);

void print_root_node_label(node_t* root_node, FILE* tring_print_file);

void dot_to_ps_image(char* file_name);

void print_tree_to_file(tree_t* tree_to_print, char* file_name);

void print_frequent_itemsets_fpgrowth(frequent_itemsets_list_t* frequent_itemsets_list, item_count_t* item_count_array);
transaction_t get_translation_transaction(item_count_t* item_count_array);

#endif /* FPGROWTHPRINTING_H_ */
