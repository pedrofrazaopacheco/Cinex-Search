#ifndef BFS_SEARCH
#define BFS_SEARCH

void enqueue(Actor_Node *actor_node, Movie_Node *parent_movie, Actor_Node *parent_actor);
void add_movie_to_movies_list(Movie_Node *current_movie);
Actor_Node *pop();
void clean_visited_nodes();
void enqueue_all_actors_in_a_movie(Movie_Node *parent_movie, Actor_Node *parent_actor);
void go_to_all_actor_movies_and_enqueue_actors(Actor_Node *current_actor);
void bfs_search(Actor_Node *origin_actor, Actor_Node *destiny_actor_arg);

#endif