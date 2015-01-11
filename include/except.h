#ifndef EXCEPT_INCLUDE
#define EXCEPT_INCLUDE

#include <setjmp.h>

#define REASON_LEN 256

struct except_t{
    char *type;
	char reason[REASON_LEN];
}; 

extern struct except_t RuntimeException;
extern struct except_t IndexOutOfBoundsException;
extern struct except_t IOException;
extern struct except_t IllegalArgumentException;
extern struct except_t ArithmeticException;
extern struct except_t NullPointerException;
extern struct except_t AssertFailedException;
extern struct except_t SignalException;


struct except_frame{
	struct except_frame *prev;
	jmp_buf env;
	const char *file;
    const char *func;
	int line;
	struct except_t *exception;
};

struct except_context{
	int except_flag; 
    int sig;
    struct except_frame *stack;
    void (*default_handler)(struct except_t *e,
                            const char *file,
                            const char *func,
                            int line);
};

enum { Except_entered=0, Except_raised,
       Except_handled,   Except_finalized };

extern volatile struct except_context except_ctx;


void except_set_default_handler(void (*handler)(struct except_t *e,
                                                const char *file,
                                                const char *func,
                                                int line));
void except_raise_reason(struct except_t *e, 
                const char *reason, 
                const char *file,
                const char *func,
                int line);
void except_raise(struct except_t *e, 
                const char *file,
                const char *func,
                int line);

void set_signal_except(int sig);

int get_except_signal();

#define EXCEPT_SIGNAL get_except_signal()

#define RAISE(e, r) except_raise_reason(&(e), r, __FILE__, __func__, __LINE__)


#define RERAISE except_raise(frame.exception, \
	frame.file, frame.func, frame.line)


#define TRY do { \
	struct except_frame frame; \
	frame.prev = except_ctx.stack; \
	except_ctx.stack = &frame;  \
	except_ctx.except_flag = setjmp(frame.env); \
	if (except_ctx.except_flag == Except_entered) {


#define CATCH(e) \
		if (except_ctx.except_flag == Except_entered) except_ctx.stack = except_ctx.stack->prev; \
	} else if (frame.exception == &(e)) { \
		except_ctx.except_flag = Except_handled;


#define ELSE \
		if (except_ctx.except_flag == Except_entered) except_ctx.stack = except_ctx.stack->prev; \
	} else { \
		except_ctx.except_flag = Except_handled;


#define FINALLY \
		if (except_ctx.except_flag == Except_entered) except_ctx.stack = except_ctx.stack->prev; \
	} { \
		if (except_ctx.except_flag == Except_entered) \
			except_ctx.except_flag = Except_finalized;


#define END_TRY \
		if (except_ctx.except_flag == Except_entered) except_ctx.stack = except_ctx.stack->prev; \
		} if (except_ctx.except_flag == Except_raised) RERAISE; \
} while (0)


#endif /*EXCEPT_INCLUDE*/
