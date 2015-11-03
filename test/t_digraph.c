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
    int count, target, from, to, i;
    clock_t clock_time;

    count = digraph_count(digraph);
    target = count / 2;
    clock_time = clock();
    srandom((long)clock_time);

    for(i = 0; i < target; i++){
        from = random() % count;
        to   = random() % count;

        digraph_connect(digraph, from, to);
    }
}

static
void
_dummy_check
(digraph_t digraph,
 int target)
{
    int count, check, from, to, i;
    count = digraph_count(digraph);
    for(i = 0; i < target; i++){
        from = random() % count;
        to   = random() % count;
        check = digraph_is_connect(digraph, from, to);
        if(1 == check){
            printf("\tfrom:%d, to:%d\n", from, to);
            printf("\t\t------------------------ret:%d\n", check);
        }
    }
}

int
main(int argc, char *argv[])
{
    int count, target;
    digraph_t   digraph;

    if(3 != argc){
        _usage();
        return EXIT_FAILURE;
    }


    count   = atoi(argv[1]);
    target  = atoi(argv[2]);

    digraph = digraph_new(count);

    printf("Leaking check 0:\n");
    mem_leak(_inuse, stdout);

    _dummy_connect(digraph);

    printf("Leaking check 1:\n");
    mem_leak(_inuse, stdout);

    _dummy_check(digraph, target);

    printf("Leaking check 2:\n");
    mem_leak(_inuse, stdout);

    digraph_free(&digraph);

    printf("Leaking check 3:\n");
    mem_leak(_inuse, stdout);

    return EXIT_SUCCESS;
}
