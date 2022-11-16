#include <stdlib.h>
#include "coroutine_int.h"
#include <errno.h>
//#include "seg_tree.h"
#include <stdio.h>

void seg_init(struct seg *seg)
{
    seg->head=0;
    seg->tail=0;
    seg_build(&seg->seg_tree,1,0,MAX_SEG_SIZE-1);
    printf("seg\n");
}

void seg_build(struct seg_node *tree,int idx,int l,int r)
{
    if(l==r)
    {
       (*(tree+idx)).exec_time=INF;
        (*(tree+idx)).pos=l;
        (*(tree+idx)).having=0;
        return ;
    }
    int mid=(l+r)>>1;
    seg_build(tree,cl(idx),l,mid);
    seg_build(tree,cr(idx),mid+1,r);
    (*(tree+idx)).having=0;
}

void seg_update(struct seg_node *tree,int idx,int l,int r,int obj,int val,int mode)//mode=1:modify,mode=0:delete
{
    if(l==r)
    {
        (*(tree+idx)).having=mode;
        (*(tree+idx)).pos=l;
        (*(tree+idx)).exec_time=val;
        return;
    }
    int mid=(l+r)>>1;
    if(obj<=mid)
        seg_update(tree,cl(idx),l,mid,obj,val,mode);
    else
        seg_update(tree,cr(idx),mid+1,r,obj,val,mode);
    (*(tree+idx)).having=(*(tree+cl(idx))).having|(*(tree+cr(idx))).having;
    if((*(tree+idx)).having)
    {
        if((*(tree+cl(idx))).having&&(*(tree+cr(idx))).having)
        {
            if( (*(tree+cl(idx))).exec_time< (*(tree+cr(idx))).exec_time)
            {
                (*(tree+idx)).exec_time=(*(tree+cl(idx))).exec_time;
                (*(tree+idx)).pos=(*(tree+cl(idx))).pos;
            }
            else
            {
                (*(tree+idx)).exec_time=(*(tree+cr(idx))).exec_time;
                (*(tree+idx)).pos=(*(tree+cr(idx))).pos;
            }
        }
        else if((*(tree+cl(idx))).having)
        {
            (*(tree+idx)).exec_time=(*(tree+cl(idx))).exec_time;
            (*(tree+idx)).pos=(*(tree+cl(idx))).pos;
        }
        else
        {
            (*(tree+idx)).exec_time=(*(tree+cr(idx))).exec_time;
            (*(tree+idx)).pos=(*(tree+cr(idx))).pos;
        }
    }
    else
        (*(tree+idx)).exec_time=INF;
}

int seg_insert(struct seg *seg,struct task_struct *task)
{
    if(((seg->head)+1)%MAX_SEG_SIZE==seg->tail)
        return -EAGAIN;
    
    seg->seg_task_arr[seg->head]=task;
    seg_update(seg->seg_tree,1,0,MAX_SEG_SIZE-1,seg->head,task->exec_runtime,1);
    seg->head=((seg->head)+1)%MAX_SEG_SIZE;
    return 0;
}

struct task_struct* seg_extract_min(struct seg*seg)
{
    if(seg->tail==seg->head)
        return NULL;
    int idx=seg->seg_tree[1].pos;
    struct task_struct *res=seg->seg_task_arr[idx];
    //printf("min %d\n",res->exec_runtime);
    seg_update(seg->seg_tree,1,0,MAX_SEG_SIZE-1,idx,seg->seg_task_arr[seg->tail]->exec_runtime,1);
    seg_update(seg->seg_tree,1,0,MAX_SEG_SIZE-1,seg->tail,INF,0);
    //printf("idx:%d idx_key:%d tail:%d tail_key:%d\n",idx,seg->seg_task_arr[idx]->exec_runtime,seg->tail,seg->seg_task_arr[seg->tail]->exec_runtime);
    seg->seg_task_arr[idx]=seg->seg_task_arr[seg->tail];
    seg->tail=((seg->tail)+1)%MAX_SEG_SIZE;
    return res;
}
