#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include"except.h"
#include"assert.h"

static void default_except_handler(struct except_t *e,
                                    const char *file,
                                    const char *func,
                                    const char *reason,
                                    int line);

static void signal_except_handler(int sig);

volatile struct except_context except_ctx = {.default_handler = default_except_handler, 
                                        .except_flag = Except_entered,
                                        .sig = 0,
                                        .stack = NULL};

struct except_t RuntimeException            = {"RuntimeException"};
struct except_t IndexOutOfBoundsException   = {"IndexOutOfBoundsException"};
struct except_t IOException                 = {"IOException"};
struct except_t IllegalArgumentException    = {"IllegalArgumentException"};
struct except_t ArithmeticException         = {"ArithmeticException"};
struct except_t NullPointerException        = {"NullPointerException"};
struct except_t AssertFailedException       = {"AssertFailedException"};
struct except_t SignalException             = {"SignalException"};
struct except_t MemFailedException          = {"MemFailedException"};

void
except_set_default_handler(void (*handler)(struct except_t *e,
                                            const char *file,
                                            const char *func,
                                            const char *reason,
                                            int line))
{
    except_ctx.default_handler = handler;
}


void 
except_raise(struct except_t *e, 
                const char *file,
                const char *func,
                const char *reason,
                int line)
{
    struct except_frame *frame = except_ctx.stack;
    assert(e);

    if(NULL == frame){
        (*(except_ctx.default_handler))(e, file, func, reason, line);
    }else{
        frame->exception = e;
        frame->file = file;
        frame->func = func;
        frame->reason = reason;
        frame->line = line;
        except_ctx.stack = except_ctx.stack->prev;
        longjmp(frame->env, Except_raised);
    }
}

void 
set_signal_except(int sig)
{
   signal(sig, signal_except_handler); 
}

int
get_except_signal()
{
    return except_ctx.sig;
}


static
void 
default_except_handler(struct except_t *e,
                        const char *file,
                        const char *func,
                        const char *reason,
                        int line)
{
    fprintf(stderr, "Abort for a uncaught exception type:%s\n"
                    "raised in %s at %s:%d\n"
                    "reason:%s\n",
                    e->type, 
                    func, file, line,
                    reason);
    fflush(stderr);
    abort();
}

static
void 
signal_except_handler(int sig)
{
    except_ctx.sig = sig;
    except_raise(&SignalException,
                __FILE__,
                __func__,
                "",
                __LINE__);
}
