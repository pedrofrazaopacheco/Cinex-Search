#ifndef HASHMAP_CREATION_H
#define HASHMAP_CREATION_H

Actor_Node *save_actor(char *actor_id);
Movie_Node *save_movie(char *movie_id);

void exchange_pointers(Actor_Node *actor_node, Movie_Node *movie_node);
void make_all_connections();
void save_movie_name(char *movie_id, char *title);
void save_actor_name(char *actor_id, char *name);
void save_all_movies_names();
void save_all_actors_names();
void make_final_actor_hashmap();
void save_actor_node_final_hashmap(char *actor_id, char *actor_name);

void save_actor_main_titles(char *actor_id, char *titles);
Actor_Node *get_actor_pointer(char *actor_id);
Movie_Node *get_movie_pointer(char *movie_id);

#endif
