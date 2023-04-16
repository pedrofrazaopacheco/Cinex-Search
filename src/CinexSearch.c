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

int check_actor_on_list(Actor_Node **repeated_actors, char *actor_id, int total_num_actors) {
    for (int i = 0; i < total_num_actors; i++) {
        if (strcmp(repeated_actors[i]->actor_id, actor_id) == 0) return TRUE;
    }
    return FALSE;
}

Actor_Node *check_for_collisions_and_return_correct_actor_pointer () {
    /* Ask the user for the name */
    /* Make a list of all the ids */
    /* Loop through the actors_data and grab the 4 most known movies */
    /* Present those */
    /* Ask for a number */
    /* Find in the hashnode the corresponding id and return the pointer */
    Actor_Node *chosen_actor;
    Actor_Node *current_actor;
    Actor_Node **repeated_actors;
    char **repeated_ids;

    char *actor_name;
    int total_num_actors = 0;

    fgets(buffer, BUFFER_SIZE, stdin);
    actor_name = strtok(buffer, "\\N");

    int actor_index = hash(actor_name);
    current_actor = actors_final_hashmap[actor_index];

    while(current_actor != NULL) {
        if (strcmp(current_actor->actor_name, actor_name) == 0) {
            total_num_actors += 1;
            if (total_num_actors == 1) repeated_actors = malloc(total_num_actors * sizeof(Actor_Node*));
            else repeated_actors = realloc(repeated_actors, total_num_actors * sizeof(Actor_Node*));
            repeated_actors[total_num_actors - 1] = current_actor;
        }
        current_actor = current_actor->next;
    }



    if (total_num_actors > 1) {
        repeated_ids = malloc(total_num_actors * sizeof(char*));
        char *actor_id, *name, *birth_year, *death_year, *profession, *movies, *movie_id;
        Movie_Node *curr_movie;

        FILE *actors_file;
        actors_file = fopen("../data/actors_data.tsv", "r");

        int curr_id_index = 0, movie_hash_index;
        while (fgets(buffer, BUFFER_SIZE, actors_file)) {
            actor_id = strtok(buffer, "\t");

            if(check_actor_on_list(repeated_actors, actor_id, total_num_actors)) {
                name = strtok(NULL, "\t");
                birth_year = strtok(NULL, "\t");
                death_year = strtok(NULL, "\t");
                profession = strtok(NULL, "\t");
                movies = strtok(NULL, "\t");
                
                printf("%s option %d: ", actor_name,  curr_id_index + 1);
                movie_id = strtok(movies, ",");
                while (movie_id != NULL) {
                    movie_hash_index = hash(movie_id);
                    curr_movie = movies_initial_hashmap[movie_hash_index];

                    while (curr_movie != NULL) {
                        if (strcmp(curr_movie->movie_id, movie_id) == 0) {
                            printf("%s\t", curr_movie->movie_name);
                            break;
                        }
                        curr_movie = curr_movie->next;
                    }
                    movie_id = strtok(NULL, ",");
                }
                printf("\n");
                curr_id_index++;
            };

        }
        
    }

    else {
        return repeated_actors[0];
    }

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


    printf("Cinex Search: High-Speed Actor Connections Search Engine\n");
    printf("Ensure the necessary files and their locations are properly configured.\n\n");
    printf("Distribution release: April 15, 2023 (00:39:51 GMT+0100)\n");
    printf("Developed by Pedro Frazão Pacheco\n");

    make_all_connections();
    save_all_movies_names();
    save_all_actors_names();

    make_final_actor_hashmap();

    free(actors_initial_hashmap);

    end = clock();
    total_time_passed = ((double)(end - start) / CLOCKS_PER_SEC);

    printf("Elapsed time for graph creation: %.3fs.\n", total_time_passed);
    printf("Source actor name: ");
    check_for_collisions_and_return_correct_actor_pointer();
    printf("\n");

    printf("Destiny actor name: ");
    check_for_collisions_and_return_correct_actor_pointer();

    /* Perguntar ao user o nome da pessoa -  */  

    /* Handle the name collision error. */

    /* Ter 2 arrays que vão aumentando o tamanho */
    /* Visited -> o node já tem a cor */
    /* Queue -> Vai aumentado */
    /* Distância -> Vai aumentado */
    /* Pai -> Vai aumentado */


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