// #pragma once
// #include "coroutine_int.h"
// #define MAX_SEG_SIZE 20
// #define INF 0x3f3f3f3f3f3f
// #define cl(x)(x>>1)
// #define cr(x)(x>>1|1)
// struct seg_node {
//     long long exec_time;
//     int pos;
//     int having;
// };
// struct seg{
//     int head;
//     int tail;
//     struct seg_node seg_tree[4*MAX_SEG_SIZE];
//     struct task_struct *seg_task_arr[MAX_SEG_SIZE];
// };
// void seg_init(struct seg *seg);
// void seg_update(struct seg_node *tree,int idx,int l,int r,int obj,int val,int mode);
// void seg_build(struct seg_node *tree,int idx,int l,int r);
// int seg_insert(struct seg *seg,struct task_struct *task);
// struct task_struct* seg_extract_min(struct seg*seg);
