#pragma once
#define MX_NODE_CNT 4
struct b_node {
    int cnt;
    int leaf;
    //int key[MX_NODE_CNT*2+1]; //cost time
    struct task_struct *task_arr[MX_NODE_CNT*2+2]; 
    struct b_node *child[MX_NODE_CNT*2+2];
    struct task_struct *test;
};
void bt_init(struct b_node *bt);
void btree_insert(struct b_node *tree,struct task_struct *task );
void btree_extract_min(struct b_node *tree);
void btree_split_child(struct b_node *x,int k,struct b_node *y);
void btree_insert_nonfull(struct b_node*x,struct task_struct *task);
