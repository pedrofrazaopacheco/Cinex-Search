#include "CinexSearch.h"

int queue_size = 0;
int queue_index = 0;
Actor_Node **actors_queue;

int num_visited_movies = 0;
Movie_Node **visited_movies_list;

Actor_Node *found_destiny_actor = NULL;
Actor_Node *destiny_actor;

void enqueue(Actor_Node *actor_node, Movie_Node *parent_movie, Actor_Node *parent_actor) {
    queue_size++;
    if (queue_size % 10 == 0) {
        actors_queue = realloc(actors_queue, (queue_size + 10) * sizeof(Actor_Node*));
    }
    actors_queue[queue_size - 1] = actor_node;
    actor_node->parent_actor = parent_actor;
    actor_node->parent_movie = parent_movie;
}

void add_movie_to_movies_list(Movie_Node *current_movie) {
    num_visited_movies++;
    if (num_visited_movies % 10 == 0) {
        visited_movies_list = realloc(visited_movies_list, (num_visited_movies + 10) * sizeof(Movie_Node*));
    }
    visited_movies_list[num_visited_movies - 1] = current_movie;
}

Actor_Node *pop() {
/* OPTIMIZATIONS: No linked lists (less memory), no shifting elements, O(1) */
    Actor_Node *first_element = actors_queue[queue_index];
    queue_size-=1;
    queue_index++;
    return first_element;
}

void clean_visited_nodes() {
    for (int i = 0; i < queue_size; i++) {
        actors_queue[i]->visited = 0;
    }
    for (int i = 0; i < num_visited_movies; i++) {
        visited_movies_list[i]->visited = 0;
    }
    queue_size = 0;
    queue_index = 0;
    num_visited_movies = 0;

    found_destiny_actor = NULL;
    destiny_actor = NULL;

    free(actors_queue);
    free(visited_movies_list);

}

void enqueue_all_actors_in_a_movie(Movie_Node *parent_movie, Actor_Node *parent_actor) {
    for (int i = 0; i < parent_movie->num_actors_in_movie; i++) {
        if (parent_movie->actors_list[i] == destiny_actor) {
            found_destiny_actor = destiny_actor;
            parent_movie->actors_list[i]->visited = 1;
            enqueue(parent_movie->actors_list[i], parent_movie, parent_actor);
            break;
        }
        if (parent_movie->actors_list[i]->visited == 0) {
            parent_movie->actors_list[i]->visited = 1;
            enqueue(parent_movie->actors_list[i], parent_movie, parent_actor);
        }
    }
}

void go_to_all_actor_movies_and_enqueue_actors(Actor_Node *current_actor) {
    for (int i = 0; i < current_actor->total_num_movies; i++) {
        if (current_actor->movies_list[i]->visited == 0) {
            add_movie_to_movies_list(current_actor->movies_list[i]);
            current_actor->movies_list[i]->visited = 1;
            enqueue_all_actors_in_a_movie(current_actor->movies_list[i], current_actor);
        }
    }
}

void bfs_search(Actor_Node *origin_actor, Actor_Node *destiny_actor_arg) {

    visited_movies_list = malloc(10 * sizeof(Movie_Node*));
    actors_queue = malloc(10 * sizeof(Actor_Node*));

    destiny_actor = destiny_actor_arg;
    enqueue(origin_actor, NULL, NULL);
    Actor_Node *current_actor;

    origin_actor->visited = 1;
    while (queue_size != 0 && found_destiny_actor == NULL) {
        current_actor = pop();
        go_to_all_actor_movies_and_enqueue_actors(current_actor);
    }
    Actor_Node *parent_node = found_destiny_actor;
    while (parent_node->parent_actor != NULL) {
        printf("%s participated in %s with %s.\n", parent_node->actor_name, parent_node->parent_movie->movie_name, parent_node->parent_actor->actor_name);
        parent_node = parent_node->parent_actor;
    }
    
    clean_visited_nodes();
    

}