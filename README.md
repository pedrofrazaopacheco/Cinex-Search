# Cinex Search
A program to find the shortest path between two actors.
It uses the DFS Algorithm

## How to use
- Download the dataset called "basics.tsv.gz" [here](https://datasets.imdbws.com/).
- The data file must be called **data.tsv**
- Run make and ./search


## WARNING
- The datasets were downloaded at the following date 2023-04-14T23:01:13.815Z
- Running the valgrind --tool=massif shows the following info
--------------------------------------------------------------------------------
  n        time(i)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 76 138,367,154,570    2,978,124,320    2,091,084,096   887,040,224            0
So this program needs about 3GB of memory to create the hashmap
