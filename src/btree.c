#include <stdlib.h>
#include "coroutine_int.h"
#include "btree.h"
#include <stdio.h>
void bt_init(struct b_node *bt)
{
    // setup the ring buffer
    // rq->out = 0;
    // rq->in = 0;
    // rq->mask = RINGBUFFER_SIZE - 1;
    bt->cnt=0;
    bt->leaf=1;
    for(int i = 0; i <= MX_NODE_CNT * 2 + 1; i++) {
        bt->child[i] = NULL;
        bt->task_arr[i] = NULL;
    } 
    printf("btree\n");
}
void btree_insert(struct b_node *tree,struct task_struct *task)
{
    struct b_node *r = tree;
    if(r->cnt==MX_NODE_CNT*2-1)
    {
        struct b_node *s=calloc(1, sizeof(struct b_node));
        tree=s;
        tree->leaf=0;
        tree->cnt=0;
        tree->child[1]=r;
        btree_split_child(s,1,r);
        btree_insert_nonfull(s,task);
    }
    else
        btree_insert_nonfull(r,task);
}
struct b_node* btree_extract_min(struct b_node *tree)
{
    if(tree->child[1]->cnt>=1)
    {
        if(tree->child[1]->cnt>=MX_NODE_CNT)
            return btree_extract_min(tree->child[1]);
        else
        {
            if(tree->child[2]->cnt>=MX_NODE_CNT)
            {
                tree->child[1]->task_arr[MX_NODE_CNT]=tree->task_arr[1];
                tree->task_arr[1]= tree->child[2]->task_arr[1];
                // tree->child[2]
                // for(int i=1;i<;i++)
                // {
                //     tree->child[2]->task_arr[1]
                // }
                return btree_extract_min(tree->child[1]);
            }
        }
    }
}
void btree_split_child(struct b_node *x,int k,struct b_node *y)
{
    struct b_node *z=calloc(1,sizeof(struct b_node));
    z->leaf=y->leaf;
    z->cnt=MX_NODE_CNT-1;
    for(int i=1;i<=MX_NODE_CNT-1;i++) 
        z->task_arr[i]=y->task_arr[i+MX_NODE_CNT];
    if(y->leaf!=1)
    {
        for(int j=1;j<=MX_NODE_CNT;j++)
            z->child[j]=y->child[j+MX_NODE_CNT];
    }
    y->cnt=MX_NODE_CNT-1;
    for(int j=x->cnt+1;j>=k+1;j--)
        x->child[j+1]=x->child[j];
    x->child[k+1]=z;
    for(int j=x->cnt;j>=k;j--)
        x->task_arr[j+1]=x->task_arr[j];
    x->task_arr[k]=y->task_arr[MX_NODE_CNT];
    x->cnt+=1;
}
void btree_insert_nonfull(struct b_node*x,struct task_struct *task)
{
    int i=x->cnt;
    if(x->leaf==1)
    {
        while(i>=1 && task->exec_runtime < x->task_arr[i]->exec_runtime)
        {
            x->task_arr[i+1]=x->task_arr[i];
            i = i - 1;
        }
        
        x->task_arr[i + 1] = task;
        //printf("%d %d", i + 1, x->task_arr[1]->exec_runtime);
        // if(i > 2) {
        //     printf("%d %d", i + 1, x->task_arr[3]->exec_runtime);
        //     x->task_arr[3]->exec_runtime = rand();
        // }
        x->cnt = x->cnt + 1;
    }
    else
    {
        while(i>=1 && task->exec_runtime<x->task_arr[i]->exec_runtime) 
            i=i-1;
        i=i+1;
        if(x->child[i]->cnt==2*MX_NODE_CNT-1)
        {
            btree_split_child(x,i,x->child[i]);
            if(task->exec_runtime>x->task_arr[i]->exec_runtime)
                i=i+1;
            btree_insert_nonfull(x->child[i],task);
        }
    }
}

void btree_pt(struct b_node*tree)
{
    for(int i=1;i<=tree->cnt;i++)
    {
        printf("%d %d\n",i,tree->task_arr[i]->exec_runtime);
    }
}