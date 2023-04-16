#ifndef CINEX_SEARCH_H
#define CINEX_SEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define HASH_TABLE_SIZE 1000003
#define BUFFER_SIZE 65537

#define TRUE 1
#define FALSE 0

// Forward declarations
typedef struct actor_node Actor_Node;
typedef struct movie_node Movie_Node;

// Actor_Node struct definition
struct actor_node {
    char *actor_name;
    char *actor_id;
    Actor_Node *next;

    unsigned short total_num_movies;
    Movie_Node **movies_list;
    unsigned short total_num_main_movies;
    Movie_Node **main_movies_list;

    Actor_Node *parent_actor;
    Movie_Node *parent_movie;

    /* 0 not, 1 yes */
    /* Grey is needed because  */
    /* OPTIMIZATIONS: Only use 2 bits for black, grey or white */
    int visited : 1;
};

// Movie_Node struct definition
struct movie_node {
    char *movie_id;
    char *movie_name;
    Movie_Node *next;
    unsigned short num_actors_in_movie;
    Actor_Node **actors_list;

    /* 0 not, 1 yes */
    /* OPTIMIZATIONS: Only use a bit for yes or no */
    int visited : 1;
};


extern Actor_Node **actors_initial_hashmap, **actors_final_hashmap;
extern Movie_Node **movies_initial_hashmap;

extern char buffer[BUFFER_SIZE];

unsigned int hash(char *string);

/* Needs to be at the end to get everything */
#include "HashMapCreation.h"
#include "BFSSearch.h"

#endif