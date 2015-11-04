#ifndef DIGRAPH_INCLUDE
#define DIGRAPH_INCLUDE


/**
 * 加权有向图对象
 * 邻接表实现
 * 适用于稀疏图
 */
#define T digraph_t
typedef struct T *T;

/**
 * 图搜索结果数据集对象
 * 在指定的图中, 以指定的节点为起点
 * 维护一颗单源点最短路径树
 * 这颗树维护着该节点到其他所有节点的连通性
 * 和连接权重, 后续使用者可以快速的查询
 * 该节点是否可以到达其他节点
 * 以及路径上的权重之和(距离)
 */
typedef struct digraph_sr_t     *digraph_sr_t;

/**
 * 路径数据集对象
 */
typedef struct digraph_path_t   *digraph_path_t;

/**
 * 新建图对象, 
 */
extern T                digraph_new         (int v);
extern void             digraph_free        (T *digraph);

extern int              digraph_count       (T digraph);
extern void             digraph_connect     (T digraph, int from, int to, double weight);
extern int              digraph_is_connect  (T digraph, int from, int to);
extern digraph_sr_t     digraph_dijkstra    (T digraph, int start);


extern void             digraph_sr_free     (digraph_sr_t *dsr);
extern int              digraph_sr_start    (digraph_sr_t dsr);
extern int              digraph_sr_has_path (digraph_sr_t dsr, int end);
extern double           digraph_sr_dist     (digraph_sr_t dsr, int end);
extern digraph_path_t   digraph_sr_path_to  (digraph_sr_t dsr, int end);

extern void             digraph_path_free   (digraph_path_t *dp);
extern int              digraph_path_length (digraph_path_t dp);
extern int              digraph_path_get     (digraph_path_t dp, int pos);

#undef T

#endif /*DIGRAPH_INCLUDE*/
