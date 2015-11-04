#include<sys/times.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"digraph.h"

static
void
_usage()
{

    printf("Usage: count target\n");
}

static 
void 
_inuse(const void *ptr,
        ssize_t size,
        const char *file,
        const char *func,
        int line,
        void *cl)
{
    FILE *log = cl;

    fprintf(log, "**memory in use at %p\n", ptr);
    fprintf(log, "  This block is %zd bytes long and allocated from"
                 " %s (%s:%d)\n", size, func, file, line);
}


static
void
_dummy_connect
(digraph_t digraph)
{
    int count, from, to, i;
    clock_t clock_time;

    count = digraph_count(digraph);
    clock_time = clock();
    srandom((long)clock_time);

    for(i = 0; i < count; i++){
        from = random() % count;
        to   = random() % count;

        digraph_connect(digraph, from, to, 1.0);
        digraph_connect(digraph, to, from, 1.0);
    }
}

static
void
_dummy_check
(digraph_t digraph)
{
    int count, i, j, k, len;
    double dist;
    count = digraph_count(digraph);
    digraph_sr_t    dsr;
    digraph_path_t  dp;
    for(i = 0; i < count; i++){
        dsr = digraph_dijkstra(digraph, i);

        for(j = 0; j < count; j++){
            if(digraph_sr_has_path(dsr, j)){
                dist = digraph_sr_dist(dsr, j);
                dp   = digraph_sr_path_to(dsr, j);
                len  = digraph_path_length(dp);
                if(len >= 7){
                    printf("from:%d to:%d dist:%f pathlen:%d\n", i, j, dist, len);
                    printf("\tstart:%d->", digraph_sr_start(dsr));
                    for(k = 0; k < len; k++){
                        printf("%d->", digraph_path_get(dp, k));
                    }
                    printf("end:%d\n", j);
                }
                digraph_path_free(&dp);
            }else{
            }
        }
        digraph_sr_free(&dsr);
    }
}

int
main(int argc, char *argv[])
{
    int count;
    digraph_t   digraph;

    if(2 != argc){
        _usage();
        return EXIT_FAILURE;
    }


    count   = atoi(argv[1]);

    digraph = digraph_new(count);

    printf("Leaking check 0:\n");
    mem_leak(_inuse, stdout);

    _dummy_connect(digraph);

    printf("Leaking check 1:\n");
    mem_leak(_inuse, stdout);

    _dummy_check(digraph);

    printf("Leaking check 2:\n");
    mem_leak(_inuse, stdout);

    digraph_free(&digraph);

    printf("Leaking check 3:\n");
    mem_leak(_inuse, stdout);

    return EXIT_SUCCESS;
}
