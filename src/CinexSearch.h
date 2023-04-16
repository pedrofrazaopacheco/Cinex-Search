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
    int total_num_movies;
    Movie_Node **movies_list;
};

// Movie_Node struct definition
struct movie_node {
    char *movie_id;
    char *movie_name;
    Movie_Node *next;
    int num_actors_in_movie;
    Actor_Node **actors_list;
};


extern Actor_Node **actors_initial_hashmap, **actors_final_hashmap;
extern Movie_Node **movies_initial_hashmap;

extern char buffer[BUFFER_SIZE];

unsigned int hash(char *string);

/* Needs to be at the end to get everything */
#include "HashMapCreation.h"

#endif