#include "CinexSearch.h"

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

/*  free(actors_initial_hashmap);
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