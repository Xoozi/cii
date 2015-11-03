#ifndef DIGRAPH_INCLUDE
#define DIGRAPH_INCLUDE


#define T digraph_t

typedef struct T *T;

typedef struct digraph_sr_t     *digraph_sr_t;
typedef struct digraph_path_t   *digraph_path_t;

extern T                digraph_new         (int v);
extern void             digraph_free        (T *digraph);

extern int              digraph_count       (T digraph);
extern void             digraph_connect     (T digraph, int from, int to);
extern int              digraph_is_connect  (T digraph, int from, int to);
extern digraph_sr_t     digraph_dijkstra    (T digraph, int start);


extern void             digraph_sr_free     (digraph_sr_t *dsr);
extern int              digraph_sr_start    (digraph_sr_t dsr);
extern int              digraph_sr_has_path (digraph_sr_t dsr, int end);
extern double           digraph_sr_dist     (digraph_sr_t dsr, int end);
extern digraph_path_t   digraph_sr_path_to  (digraph_sr_t dsr, int end);

extern void             digraph_path_free   (digraph_path_t *dp);
extern int              digraph_path_length (digraph_path_t dp);
extern int              digraph_path_from   (digraph_path_t dp, int pos);
extern int              digraph_path_to     (digraph_path_t dp, int pos);

#undef T

#endif /*DIGRAPH_INCLUDE*/
