//
// Created by yushigengyu on 2021/4/30.
//

#ifndef C_CLI_TABLE_DOUBLY_LINK_LIST_H
#define C_CLI_TABLE_DOUBLY_LINK_LIST_H

/**
 * 获得包含node的结构体的指针
 * ptr node指针
 * type 包含node的struct的类型
 * struct中 node 的成员名
 */
#define doubly_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define doubly_list_node_entry(node, type, member) \
    doubly_container_of(node, type, member)

typedef struct doubly_list_node{
    struct doubly_list_node *next;
    struct doubly_list_node *prev;
}doubly_list_node,doubly_list_list;

static inline void doubly_list_init(doubly_list_list* pList) {
    pList->next = pList;
    pList->prev = pList;
}

static inline void doubly_insert_after(doubly_list_node* pNode, doubly_list_node* pNodeInsert){
    pNode->next->prev = pNodeInsert;
    pNodeInsert->next = pNode->next;

    pNode->next = pNodeInsert;
    pNodeInsert->prev = pNode;
}

static inline void doubly_insert_before(doubly_list_node* pNode, doubly_list_node* pNodeInsert){
    pNode->prev->next = pNodeInsert;
    pNodeInsert->next = pNode;

    pNodeInsert->prev = pNode->prev;
    pNode->prev = pNodeInsert;
}

static inline void doubly_append(doubly_list_list* pList, doubly_list_node* pNode) {
    doubly_list_node *l = pList;
    while (l->next != pList){
        l = l->next;
    }
    doubly_insert_after(l, pNode);
}

static inline void doubly_list_remove(doubly_list_node *pNode){
    pNode->next->prev = pNode->prev;
    pNode->prev->next = pNode->next;

    pNode->next = pNode->prev = pNode;
}

static inline unsigned int check_doubly_list_empty(const doubly_list_list *plist){
    return plist->next == plist;
}

static inline unsigned int doubly_list_len(const doubly_list_list *pList){
    unsigned int len = 0;
    const doubly_list_list *p = pList;
    while (p->next != pList){
        p = p->next;
        len ++;
    }
    return len;
}
#endif //C_CLI_TABLE_DOUBLY_LINK_LIST_H
