#include<stdlib.h>
#include<float.h>
#include"assert.h"
#include"mem.h"
#include"arena.h"
#include"digraph.h"
#include"indexminpq.h"

#define T digraph_t

struct adj{
    struct adj  *link;
    int         to;
    double      weight;
};

/**
 * 图数据结构
 */
struct T{
    /**
     * 顶点数
     */
    int count;

    /**
     * 图数据及算法附属数据所用的内存池
     */
    arena_t     arena;

    /**
     * 邻接表数组
     * adj_array[i]内容为i节点的邻接表
     * adj项代表以i为起点 to为终点的边
     * weight为权重
     */
    struct adj **adj_array;
};

struct digraph_sr_t{
    int size;
    int start;
    struct adj  **edge_array;
    double      *dist_to;
};

struct digraph_path_t{
    int size;
};

static void _relax(digraph_t digraph, digraph_sr_t dsr, iminpq_t iminpq, int v);

T
digraph_new
(int v)
{
    T digraph;
    int i;
    assert(v >= 0);
    digraph = ALLOC(sizeof(*digraph) + v * sizeof(digraph->adj_array[0]));
    digraph->count = v;
    digraph->arena = arena_new();
    digraph->adj_array = (struct adj **) (digraph + 1);
    for(i = 0; i < digraph->count; i++)
        digraph->adj_array[i] = NULL;
    return digraph;
}


void
digraph_free
(T *digraph)
{
    assert(digraph);
    assert(*digraph);
    arena_free((*digraph)->arena);
    FREE(*digraph);
}


int
digraph_count
(T digraph)
{
    return digraph->count;
}


void
digraph_connect
(T digraph, int from, int to)
{
    struct adj *adj_list, *new_adj;
    assert(digraph);
    assert(from >= 0 && from < digraph->count);
    assert(to >= 0 && to < digraph->count);
    adj_list = digraph->adj_array[from];
    new_adj  = ARENA_ALLOC(digraph->arena, sizeof(*new_adj));
    new_adj->link = adj_list;
    digraph->adj_array[from] = new_adj;
}


int
digraph_is_connect
(T digraph, int from, int to)
{
    struct adj *adj_list;
    assert(digraph);
    assert(from >= 0 && from < digraph->count);
    assert(to >= 0 && to < digraph->count);
    adj_list = digraph->adj_array[from];
    while(adj_list){
        if(to == adj_list->to){
            return 1;
        }
        adj_list = adj_list->link;
    }
    return 0;
}


digraph_sr_t
digraph_dijkstra
(T digraph, int start)
{
    int i;
    digraph_sr_t sr;
    iminpq_t    iminpq;

    assert(digraph);
    assert(start >= 0 && start < digraph->count);

    iminpq = iminpq_new(digraph->count);
    
    sr = ALLOC(sizeof(*sr));
    sr->size = digraph->count;
    sr->start = start;
    sr->dist_to = ALLOC(sr->size * sizeof(double));
    sr->edge_array = ALLOC(sr->size * sizeof(struct adj *));

    for(i = 0; i < sr->size; i++){
        sr->dist_to[i] = DBL_MAX;
        sr->edge_array[i] = NULL;
    }

    sr->dist_to[start] = 0.0;
    iminpq_insert(iminpq, start, 0.0);
    while(!iminpq_is_empty(iminpq)){
        _relax(digraph, sr, iminpq, iminpq_delete_min(iminpq));
    }

    iminpq_free(&iminpq);
    return sr;
}


static
void
_relax
(digraph_t digraph, digraph_sr_t dsr, iminpq_t iminpq, int v)
{
    int         w;
    double      weight;
    struct adj  *adj_list;
    struct adj  **edge_array;
    double      *dist_to;

    dist_to = dsr->dist_to;
    edge_array = dsr->edge_array;

    adj_list = digraph->adj_array[v];
    while(adj_list){
        w = adj_list->to;
        weight = adj_list->weight;
        if(dist_to[w] > dist_to[v] + weight){
            dist_to[w] = dist_to[v] + weight;
            edge_array[w] = adj_list;
        
            if(iminpq_contains(iminpq, w))
                iminpq_change(iminpq, w, dist_to[w]);
            else
                iminpq_insert(iminpq, w, dist_to[w]);
        }

        adj_list = adj_list->link;
    }
}


void
digraph_sr_free
(digraph_sr_t *dsr)
{
    assert(dsr);
    assert(*dsr);
    FREE((*dsr)->dist_to);
    FREE((*dsr)->edge_array);
    FREE(*dsr);
}


int
digraph_sr_start
(digraph_sr_t dsr)
{
    assert(dsr);
    return dsr->start;
}


int
digraph_sr_has_path
(digraph_sr_t dsr, int end)
{
    assert(dsr);
    assert(end >= 0 && end <= dsr->size);
    return dsr->dist_to[end] < DBL_MAX;
}


double
digraph_sr_dist
(digraph_sr_t dsr, int end)
{
    assert(dsr);
    assert(end >= 0 && end <= dsr->size);
    return dsr->dist_to[end];
}


digraph_path_t
digraph_sr_path_to
(digraph_sr_t dsr, int end)
{
    return NULL;
}


void
digraph_path_free
(digraph_path_t *dp)
{

}


int
digraph_path_length
(digraph_path_t dp)
{
    return 0;
}


int
digraph_path_from
(digraph_path_t dp, int pos)
{
    return 0;
}


int
digraph_path_to
(digraph_path_t dp, int pos)
{
    return 0;
}
