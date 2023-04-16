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


unsigned int hash(char *string) {
    /* Needs to be unsigned due to special chars */
    unsigned int current_index = 0, last_ascii_character = 127;
    /* Loop até chegar ao final da string */
    for (; *string != '\0'; string++) {
        /* The multiplication serves to increase the current_index number */
        current_index = (current_index*last_ascii_character + *string) % HASH_TABLE_SIZE;
    }
    return current_index;
}


Actor_Node **actors_initial_hashmap, **actors_final_hashmap;
Movie_Node **movies_initial_hashmap;


char buffer[BUFFER_SIZE];

Actor_Node *save_actor(char *actor_id) {
    unsigned int actor_index = hash(actor_id);
    Actor_Node *curr_node = actors_initial_hashmap[actor_index];

    while (curr_node != NULL) {
        if (strcmp(curr_node->actor_id, actor_id) == 0) return curr_node;
        curr_node = curr_node->next;
    }
    Actor_Node *new_actor = malloc(sizeof(Actor_Node));
    new_actor->actor_id = strdup(actor_id);
    new_actor->total_num_movies = 0;
    new_actor->movies_list = NULL;
    new_actor->actor_name = NULL;
    new_actor->next = actors_initial_hashmap[actor_index];
    actors_initial_hashmap[actor_index] = new_actor;
    return new_actor;
}

Movie_Node *save_movie(char *movie_id) {
    unsigned int movie_index = hash(movie_id);
    Movie_Node *curr_node = movies_initial_hashmap[movie_index];

    while (curr_node != NULL) {
        if (strcmp(curr_node->movie_id, movie_id) == 0) return curr_node;
        curr_node = curr_node->next;
    }
    Movie_Node *new_movie = malloc(sizeof(Movie_Node));
    new_movie->movie_id = strdup(movie_id);
    new_movie->num_actors_in_movie = 0;
    new_movie->actors_list = NULL;
    new_movie->movie_name = NULL;
    new_movie->next = movies_initial_hashmap[movie_index];
    movies_initial_hashmap[movie_index] = new_movie;
    return new_movie;
}

void exchange_pointers(Actor_Node *actor_node, Movie_Node *movie_node) {
    actor_node->total_num_movies++;
    movie_node->num_actors_in_movie++;

    if (actor_node->total_num_movies == 1) {
        actor_node->movies_list = malloc(sizeof(Movie_Node*));
        actor_node->movies_list[0] = movie_node;
    }
    else {
        actor_node->movies_list = realloc(actor_node->movies_list, actor_node->total_num_movies * sizeof(Movie_Node*));
        actor_node->movies_list[actor_node->total_num_movies - 1] = movie_node;
    }

    if (movie_node->num_actors_in_movie == 1) {
        movie_node->actors_list = malloc(sizeof(Actor_Node*));
        movie_node->actors_list[0] = actor_node;
    }
    else {
        movie_node->actors_list = realloc(movie_node->actors_list, movie_node->num_actors_in_movie * sizeof(Actor_Node*));
        movie_node->actors_list[movie_node->num_actors_in_movie - 1] = actor_node;
    }
}

void make_all_connections() {
    char *movie_id, *order, *actor_id;
    FILE *common_data;
    common_data = fopen("common_data.tsv", "r");
    /* The first line does not matter */
    fgets(buffer, BUFFER_SIZE, common_data);
    Actor_Node *actor_node;
    Movie_Node *movie_node;

    while(fgets(buffer, BUFFER_SIZE, common_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        movie_id = strtok(buffer, "\t");
        order = strtok(NULL, "\t");
        actor_id = strtok(NULL, "\t");

        actor_node = save_actor(actor_id);
        movie_node = save_movie(movie_id);

        exchange_pointers(actor_node, movie_node);
    }
    fclose(common_data);

}


void save_movie_name(char *movie_id, char *title) {
     unsigned int movie_index = hash(movie_id);
    Movie_Node *curr_node = movies_initial_hashmap[movie_index];

    while (curr_node != NULL) {
        if (strcmp(curr_node->movie_id, movie_id) == 0) {
            curr_node->movie_name = strdup(title);
            break;
        }
        curr_node = curr_node->next;
    }
}

void save_actor_name(char *actor_id, char *name) {
    unsigned int actor_index = hash(actor_id);
    Actor_Node *curr_node = actors_initial_hashmap[actor_index];

    while (curr_node != NULL) {
        if (strcmp(curr_node->actor_id, actor_id) == 0) {
            curr_node->actor_name = strdup(name);
            break;
        }
        curr_node = curr_node->next;
    }
}

void save_all_movies_names() {
    char *movie_id, *type, *title;
    FILE *titles_data;
    titles_data = fopen("titles_data.tsv", "r");
    /* The first line does not matter */
    fgets(buffer, BUFFER_SIZE, titles_data);
    Movie_Node *movie_node;

    while(fgets(buffer, BUFFER_SIZE, titles_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        movie_id = strtok(buffer, "\t");
        type = strtok(NULL, "\t");
        title = strtok(NULL, "\t");

        save_movie_name(movie_id, title);

    }
    fclose(titles_data);

}

void save_all_actors_names() {
    char *actor_id, *name;
    FILE *actors_data;
    actors_data = fopen("actors_data.tsv", "r");
    /* The first line does not matter */
    fgets(buffer, BUFFER_SIZE, actors_data);
    Movie_Node *movie_node;

    while(fgets(buffer, BUFFER_SIZE, actors_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        actor_id = strtok(buffer, "\t");
        name = strtok(NULL, "\t");

        save_actor_name(actor_id, name);
    }
    fclose(actors_data);
}

void save_actor_node_final_hashmap(char *actor_id, char *actor_name) {
    unsigned int actor_index = hash(actor_id);
    Actor_Node *curr_node = actors_initial_hashmap[actor_index];

    while (curr_node != NULL) {
        if (strcmp(curr_node->actor_id, actor_id) == 0) {
            unsigned int actor_final_index = hash(actor_name);
            if (actors_final_hashmap[actor_final_index] == NULL) {
                actors_final_hashmap[actor_final_index] = curr_node;
                curr_node->next = NULL;
            } else {
                curr_node->next = actors_final_hashmap[actor_final_index];
                actors_final_hashmap[actor_final_index] = curr_node;
            }
            break;
        }
        curr_node = curr_node->next;
    }

}

void make_final_actor_hashmap() {
    char *actor_id, *name;
    FILE *actors_data;
    actors_data = fopen("actors_data.tsv", "r");
    /* The first line does not matter */
    fgets(buffer, BUFFER_SIZE, actors_data);
    Movie_Node *movie_node;

    while(fgets(buffer, BUFFER_SIZE, actors_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        actor_id = strtok(buffer, "\t");
        name = strtok(NULL, "\t");

        save_actor_node_final_hashmap(actor_id, name);
    }
    fclose(actors_data);
}

int main() {

    unsigned int curr_num, i = 0;

    clock_t start, end;
    double total_time_passed;
    start = clock();


    actors_initial_hashmap = malloc(HASH_TABLE_SIZE * sizeof(Actor_Node*));
    actors_final_hashmap = malloc(HASH_TABLE_SIZE * sizeof(Actor_Node*));
    movies_initial_hashmap = malloc(HASH_TABLE_SIZE * sizeof(Movie_Node*));

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        actors_initial_hashmap[i] = NULL;
        actors_final_hashmap[i] = NULL;
        movies_initial_hashmap[i] = NULL;
    }


    printf("The Clock Started!\n");
    make_all_connections();
    save_all_movies_names();
    save_all_actors_names();

    make_final_actor_hashmap();

    /*
    free(actors_initial_hashmap);
    free(movies_initial_hashmap);
    */

    /* No final, fazer 1 hashmap novo mas desta vez com a entrada com o nome dos atores */

    end = clock();
    total_time_passed = ((double)(end - start) / CLOCKS_PER_SEC);

    printf("It took %f\n", total_time_passed);
    printf("Please search for an actor!\n");

    while(fgets(buffer, BUFFER_SIZE, stdin)) {
        char *actor_name = strtok(buffer, "\n");
        unsigned int actor_index = hash(actor_name);
        Actor_Node *current_actor = actors_final_hashmap[actor_index];
        Movie_Node *current_movie;

        while (current_actor != NULL) {
            printf("Actor: %s", current_actor->actor_name);
            if (strcmp(current_actor->actor_name, actor_name) == 0) {
                printf("%s has made the following movies: ", current_actor->actor_name);
                printf("Total num movies: %d\n", current_actor->total_num_movies);
                for (int i = 0; i < current_actor->total_num_movies; i++) {
                    printf("%s ", current_actor->movies_list[i]->movie_name);
                }
                printf("\n");
            }
            current_actor = current_actor->next;
        }
    }

    return 0;
}