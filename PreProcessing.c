#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define HASH_TABLE_SIZE 1000003
#define BUFFER_SIZE 65537

#define TRUE 1
#define FALSE 0

typedef struct actor_node {
    char *id;
    char *name;
    struct actor_node *next;
} Actor_Node;

typedef struct movie_node {
    char *movie_name;
    struct movie_node *next;
    int num_actors_in_movie;
    Actor_Node **actors_list;
} Movie_Node;

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


Actor_Node* actors_hashmap[HASH_TABLE_SIZE];
Movie_Node* movies_hashmap[HASH_TABLE_SIZE];

Actor_Node* store_new_actor(char *actor_name, char *actor_id) {
    unsigned int actor_index = hash(actor_id);
    Actor_Node *new_actor = malloc(sizeof(Actor_Node));
    new_actor->name = strdup(actor_name);
    new_actor->id = strdup(actor_id);

    if (actors_hashmap[actor_index] == NULL) {
        actors_hashmap[actor_index] = new_actor;
        new_actor->next = NULL;
    } else {
        new_actor->next = actors_hashmap[actor_index];
        actors_hashmap[actor_index] = new_actor;
    }

    return new_actor;
}

void store_new_movie(char *movies, Actor_Node *current_actor) {
    /* We will only store the pointers to actors to space a ton of space */

    unsigned int movie_index;
    char *movie_name = strtok(movies, ",");

    while (movie_name != NULL) {
        movie_index = hash(movie_name);

        Movie_Node *new_movie = malloc(sizeof(Movie_Node));
        Movie_Node *current_movie = movies_hashmap[movie_index];

        Movie_Node *found_movie = NULL;

        new_movie->movie_name = strdup(movie_name);

        /* Created a new movie */
        if (movies_hashmap[movie_index] == NULL) {
                new_movie->num_actors_in_movie = 1;
                new_movie->actors_list = malloc(sizeof(Actor_Node*));
                new_movie->actors_list[0] = current_actor;
                new_movie->next = NULL;
                movies_hashmap[movie_index] = new_movie;
        } else {
            /* Search if the movie is already there */
            while (current_movie != NULL) {
                if (strcmp(current_movie->movie_name, movie_name) == 0) {
                    found_movie = current_movie;
                    break;
                }
                current_movie = current_movie->next;
            }
            if (found_movie != NULL) {
                found_movie->num_actors_in_movie = found_movie->num_actors_in_movie + 1;
                found_movie->actors_list = realloc(found_movie->actors_list, found_movie->num_actors_in_movie * sizeof(Actor_Node*));
                found_movie->actors_list[found_movie->num_actors_in_movie - 1] = current_actor;
            }
            /* Created a new movie */
            else {
                new_movie->num_actors_in_movie = 1;
                new_movie->actors_list = malloc(sizeof(Actor_Node*));
                new_movie->actors_list[0] = current_actor;
                new_movie->next = movies_hashmap[movie_index];
                movies_hashmap[movie_index] = new_movie;
            }
        }
        movie_name = strtok(NULL, ",");
    }

}

int main() {

    /* We know that there are about 12 million actor records */
    /* So let's make 2 hashtables with 1,000,003 million entries - This will only be about 16 megabytes of pointers */
    /* We need a high prime number to make less collisions */
    
    /* Steps */
    /* We get the line */
    /* Store the name in a string and store the actor in a hash table */
    /* Go to each movie in the list, and store it in a hashmap if not already there */
    /* Get the actors pointer and reallocate the actors_list array */

    FILE *actors_data, preproccessing_output;
    char buffer[BUFFER_SIZE];

    char *actor_id, *actor_name, *birth_year, *professions, *death_year, *movies;
    Actor_Node* current_actor;
    unsigned int curr_num, i = 0;

    clock_t start, end;
    double total_time_passed;

    start = clock();
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        actors_hashmap[i] = NULL;
        movies_hashmap[i] = NULL;
    }

    actors_data = fopen("data.tsv", "r");
    /* The first line does not matter */
    fgets(buffer, BUFFER_SIZE, actors_data);
    int line = 0;
    while(fgets(buffer, BUFFER_SIZE, actors_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        actor_id = strtok(buffer, "\t");
        actor_name = strtok(NULL, "\t");
        birth_year = strtok(NULL, "\t");
        death_year = strtok(NULL, "\t");
        professions = strtok(NULL, "\t");
        movies = strtok(NULL, "\t");

        current_actor = store_new_actor(actor_name, actor_id);
        store_new_movie(movies, current_actor);
    }
    end = clock();
    total_time_passed = ((double)(end - start) / CLOCKS_PER_SEC);

    printf("It took %f\n", total_time_passed);

    fseek(actors_data, 0, SEEK_SET);

    while(fgets(buffer, BUFFER_SIZE, actors_data)) {

        current_actor = store_new_actor(actor_name, actor_id);
        store_new_movie(movies, current_actor);
    }

    Movie_Node *current_movie;
    // HASH_TABLE_SIZE
    for (int i = 0; i < 100; i++) {
        current_movie = movies_hashmap[i];
        while (current_movie != NULL) {
            printf("The movie %s has %d actors and they are: ", current_movie->movie_name, current_movie->num_actors_in_movie);
            for (int j = 0; j < current_movie->num_actors_in_movie; j++) {
                printf("%s\t", current_movie->actors_list[j]->name);
            }
            printf("\n");
            current_movie = current_movie->next;
        }
    }

    printf("Working!\n");
    return 0;
}