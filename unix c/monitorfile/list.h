#ifndef _LIST_H
#define _LIST_H

#include <malloc.h>
#include <string.h>
#include <sys/types.h>

typedef struct
{
    time_t mtime;
    char name[256];
}File;

typedef struct _ListNode
{
    File data;
    struct _ListNode *next;
}ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;
}List;

List* InitList();

int ListDestroy(List *lp);

int ListInsertHead(List *lp, File *data);

int ListDelete(List *lp, int n);

int ListEmpty(List *lp);

ListNode* NewNode(File *data);

#endif

