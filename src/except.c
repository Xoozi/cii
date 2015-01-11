#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include"except.h"
#include"assert.h"

static void default_except_handler(struct except_t *e,
                                    const char *file,
                                    const char *func,
                                    int line);

static void signal_except_handler(int sig);

volatile struct except_context except_ctx = {.default_handler = default_except_handler, 
                                        .except_flag = Except_entered,
                                        .sig = 0,
                                        .stack = NULL};

struct except_t RuntimeException            = {.type = "RuntimeException", .reason = {0}};
struct except_t IndexOutOfBoundsException   = {.type = "IndexOutOfBoundsException", .reason = {0}};
struct except_t IOException                 = {.type = "IOException", .reason = {0}};
struct except_t IllegalArgumentException    = {.type = "IllegalArgumentException", .reason = {0}};
struct except_t ArithmeticException         = {.type = "ArithmeticException", .reason = {0}};
struct except_t NullPointerException        = {.type = "NullPointerException", .reason = {0}};
struct except_t AssertFailedException       = {.type = "AssertFailedException", .reason = {0}};
struct except_t SignalException             = {.type = "SignalException", .reason = {0}};

void
except_set_default_handler(void (*handler)(struct except_t *e,
                                            const char *file,
                                            const char *func,
                                            int line))
{
    except_ctx.default_handler = handler;
}

void 
except_raise_reason(struct except_t *e, 
                const char *reason, 
                const char *file,
                const char *func,
                int line)
{
    struct except_frame *frame = except_ctx.stack;
    assert(e);

    strncpy(e->reason, reason, REASON_LEN);

    if(NULL == frame){
        (*(except_ctx.default_handler))(e, file, func, line);
    }else{

        frame->exception = e;
        frame->file = file;
        frame->func = func;
        frame->line = line;
        except_ctx.stack = except_ctx.stack->prev;
        longjmp(frame->env, Except_raised);
    }
}

void 
except_raise(struct except_t *e, 
                const char *file,
                const char *func,
                int line)
{
    struct except_frame *frame = except_ctx.stack;
    assert(e);

    if(NULL == frame){
        (*(except_ctx.default_handler))(e, file, func, line);
    }else{
        frame->exception = e;
        frame->file = file;
        frame->func = func;
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
                        int line)
{
    fprintf(stderr, "Abort for a uncaught exception type:%s\n"
                    "raised in %s at %s:%d\n"
                    "reason:%.*s\n",
                    e->type, 
                    func, file, line,
                    REASON_LEN, e->reason);
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
                __LINE__);
}
