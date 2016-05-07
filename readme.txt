To Compile:

make clean
make

To Run:

./frequent_itemset

-----------------------

Selecting Itemset generation algorithm:

Go to "FrequentItemSet.c." Within "main" function, uncomment either fp_analysis call or "apriori_generate_frequent_itemsets" call,
depending on whether you want to use FP-Growth or Apriori method.

-----------------------

Changing Data set parameters:

Within "FrequentItemSet.c." go to line 44. There you will see comments for how to change number of transactions, number of items, and min support value.

Changing Data repetitiveness value for generating data set:

At line 27 of ReadData.c, change "max_prob_item_exists" to desired repetitiveness value.


-----------------------

Apriori Code:

See Apriori.c. The main function is "apriori_generate_frequent_itemsets" near the bottom of the file.

-----------------------

FP-Growth Code:

See FPGrowth.c. The main function is "fp_analysis" near the bottom of the file. The recursive function is "find_frequent_itemsets" at roughly line 358.

