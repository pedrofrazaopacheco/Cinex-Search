#include "CinexSearch.h"

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
    common_data = fopen("../data/common_data.tsv", "r");
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
    titles_data = fopen("../data/titles_data.tsv", "r");
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

Actor_Node *get_actor_pointer(char *actor_id) {
    int actor_index = hash(actor_id);
    Actor_Node *current_actor = actors_initial_hashmap[actor_index];
    while (current_actor != NULL) {
        if (strcmp(current_actor->actor_id, actor_id) == 0) return current_actor;
        current_actor = current_actor->next;
    }
    return NULL;
}

Movie_Node *get_movie_pointer(char *movie_id) {
    int movie_index = hash(movie_id);
    Movie_Node *current_movie = movies_initial_hashmap[movie_index];
    while (current_movie != NULL) {
        if (strcmp(current_movie->movie_id, movie_id) == 0) return current_movie;
        current_movie = current_movie->next;
    }
    return NULL;
}

void save_actor_main_titles(char *actor_id, char *movies_ids) {
    char *movie_id = strtok(movies_ids, ",");
    Actor_Node *chosen_actor = get_actor_pointer(actor_id);
    Movie_Node *chosen_movie;
    if (chosen_actor != NULL) {
        chosen_actor->total_num_main_movies = 0;
        while (movie_id != NULL) {
            chosen_movie = get_movie_pointer(movie_id);
            if (chosen_movie != NULL) {
                chosen_actor->total_num_main_movies++;
                if (chosen_actor->total_num_main_movies == 1) {
                    chosen_actor->main_movies_list = malloc(sizeof(Movie_Node*));
                } else {
                    chosen_actor->main_movies_list = realloc(chosen_actor->main_movies_list, chosen_actor->total_num_main_movies * sizeof(Movie_Node*));
                }
                chosen_actor->main_movies_list[chosen_actor->total_num_main_movies - 1] = chosen_movie;
            }
            movie_id = strtok(NULL, ",");
        }
    }
    
}

void save_all_actors_names() {
    char *actor_id, *name, *birth_year, *death_year, *profession, *titles;
    FILE *actors_data;
    actors_data = fopen("../data/actors_data.tsv", "r");
    /* The first line does not matter */
    // nconst	primaryName	birthYear	deathYear	primaryProfession	knownForTitles
    fgets(buffer, BUFFER_SIZE, actors_data);
    Movie_Node *movie_node;

    while(fgets(buffer, BUFFER_SIZE, actors_data)) {
        buffer[strcspn(buffer, "\n")] = 0;
        actor_id = strtok(buffer, "\t");
        name = strtok(NULL, "\t");
        birth_year = strtok(NULL, "\t");
        death_year = strtok(NULL, "\t");
        profession = strtok(NULL, "\t");
        titles = strtok(NULL, "\t");

        save_actor_name(actor_id, name);
        save_actor_main_titles(actor_id, titles);
    }
    fclose(actors_data);
}

void save_actor_node_final_hashmap(char *actor_id, char *actor_name) {
    unsigned int actor_index = hash(actor_id);
    unsigned int actor_final_index = hash(actor_name);
    Actor_Node *curr_node = actors_initial_hashmap[actor_index];
    Actor_Node *prev_node = NULL;

    while (curr_node != NULL) {
        if (strcmp(curr_node->actor_id, actor_id) == 0) {
            // Update the head pointer of the initial hash map's bucket
            if (prev_node == NULL) {
                actors_initial_hashmap[actor_index] = curr_node->next;
            } else {
                prev_node->next = curr_node->next;
            }
            
            // Move the node to the final hash map
            curr_node->next = actors_final_hashmap[actor_final_index];
            actors_final_hashmap[actor_final_index] = curr_node;
            break;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
}


void make_final_actor_hashmap() {
    char *actor_id, *name;
    FILE *actors_data;
    actors_data = fopen("../data/actors_data.tsv", "r");
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