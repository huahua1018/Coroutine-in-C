#ifndef __COROUTINE_INT_H__
#define __COROUTINE_INT_H__

#include "rbtree.h"
#include "context.h"
#include "btree.h"
//#include "seg_tree.h"

typedef int (*job_t)(struct context *__context, void *args);

/*
 * task_struct maintain the coroutine or task object.
 */
struct task_struct {
    /* job information */
    struct cr *cr;
    int tfd; /* task fd */
    job_t job;
    void *args;
    struct context context; /* defined at context.h */
    
    /* bt info*/
    long exec_runtime;

    /* default info */
    struct {
        struct rb_node node;
        long sum_exec_runtime;
        long exec_start;
    };
    };

#ifndef container_of
#define container_of(ptr, type, member)                        \
    __extension__({                                            \
        const __typeof__(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member));     \
    })
#endif

#define task_of(__context) container_of(__context, struct task_struct, context)

/* runqueue */

// Need to be power of two
#define RINGBUFFER_SIZE 16

struct rq {
    unsigned int out, in; /* dequeue at out, enqueue  at in*/
    unsigned int mask; /* the size is power of two, so mask will be size - 1 */
    struct task_struct *r[RINGBUFFER_SIZE];
};

void rq_init(struct rq *rq);
int rq_enqueue(struct rq *rq, struct task_struct *task);
struct task_struct *rq_dequeue(struct rq *rq);

/*btree*/
// #define MX_NODE_CNT 4
// struct b_node {
//     int cnt;
//     int key[MX_NODE_CNT*2+1]; //cost time
//     struct task_struct * task_arr[MX_NODE_CNT*2+1]; 
//     struct b_node *child[MX_NODE_CNT*2+1];
// };

// void btree_insert(struct b_node *now,struct task_struct *task );

#define MAX_SEG_SIZE 20
#define INF 0x3f3f3f3f3f3f
#define cl(x)(x>>1)
#define cr(x)(x>>1|1)
struct seg_node {
    long long exec_time;
    int pos;
    int having;
};
struct seg{
    int head;
    int tail;
    struct seg_node seg_tree[4*MAX_SEG_SIZE];
    struct task_struct *seg_task_arr[MAX_SEG_SIZE];
};
void seg_init(struct seg *seg);
void seg_update(struct seg_node *tree,int idx,int l,int r,int obj,int val,int mode);
void seg_build(struct seg_node *tree,int idx,int l,int r);
int seg_insert(struct seg *seg,struct task_struct *task);
struct task_struct* seg_extract_min(struct seg*seg);


/* main data structure */

#define MAX_CR_TABLE_SIZE 10

struct cr {
    unsigned long size; /* number of the task in this scheduler */
    int crfd; /* coroutine fd number */
    int flags; /* Which type of scheduler, FIFO or CFS */
    struct task_struct *current; /* the job currently working */

    /* scheduler - chose by the flags */
    struct rq rq; /* FIFO */
    struct rb_root root; /* Default */

    struct b_node* b_root; /* SJF */

    /*seg*/
    struct seg seg_root;

    /* sched operations */
    int (*schedule)(struct cr *cr, job_t func, void *args);
    struct task_struct *(*pick_next_task)(struct cr *cr);
    int (*put_prev_task)(struct cr *cr, struct task_struct *prev);
};

struct cr_struct {
    int size;
    struct cr *table[MAX_CR_TABLE_SIZE];
};

void sched_init(struct cr *cr);

#endif /* __COROUTINE_INT_H__ */
