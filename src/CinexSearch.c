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

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard the character
    }
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
    actor_name = strdup(strtok(buffer, "\n"));
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
    Actor_Node* repeated_actors_final[total_num_actors - 1];
    int repeated_actors_final_count = 0;

    if (total_num_actors > 1) {
        for (int i = 0; i < total_num_actors; i++) {
            if (repeated_actors[i]->total_num_main_movies > 0) {
                repeated_actors_final[repeated_actors_final_count] = repeated_actors[i];
                repeated_actors_final_count++;
                printf("\n%s option %d:\t", repeated_actors[i]->actor_name, i + 1);
            }
            for (int j = 0; j < repeated_actors[i]->total_num_main_movies; j++) {
                if (j != 0) printf(", ");
                printf("%s", repeated_actors[i]->main_movies_list[j]->movie_name);
            }
        }

        int chosen_option;
        printf("\nWhich Option?: ");
        scanf("%d", &chosen_option);
        flush_stdin();
        return repeated_actors_final[chosen_option - 1];
    }
    else {
        return repeated_actors[0];
    }

}

void ask_user_actors_and_do_bfs() {
    printf("Source actor name: ");
    Actor_Node *origin_actor = check_for_collisions_and_return_correct_actor_pointer();
    printf("Destiny actor name: ");
    Actor_Node *destiny_actor = check_for_collisions_and_return_correct_actor_pointer();

    clock_t start, end;
    start = clock();
    bfs_search(origin_actor, destiny_actor);

    end = clock();
    double total_time_passed = ((double)(end - start) / CLOCKS_PER_SEC);
    printf("Elapsed time for search: %.5fs.\n", total_time_passed);
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

    ask_user_actors_and_do_bfs();

    printf("Do you want to search again? Y/N\n");
    char c = getchar();
    while (c == 'Y') {
        flush_stdin();
        ask_user_actors_and_do_bfs();
        printf("Do you want to search again? Y/N\n");
        c = getchar();
    }

    /*
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
    */

    return 0;
}