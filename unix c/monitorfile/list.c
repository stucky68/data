#include "list.h"

File g_data;

List *InitList()
{
    List *p = NULL;
    ListNode *pdata = NULL;
    p = (List *)malloc(sizeof(List));
    if (!p)
        return p;
    pdata = NewNode(&g_data);
    p->head = pdata;
    return p;
}

int ListDestroy(List *lp)
{
    if (!lp)
    {
        return 0;
    }
    while (ListDelete(lp, 1));
    free(lp);
    return 1;
}

int ListInsertHead(List *lp, File *data)
{
    ListNode *p = NULL;
    p = NewNode(data);
    if (!p)
    {
        return 0;
    }
    p->next = lp->head->next;
    lp->head->next = p;
    return 1;
}

int ListDelete(List *lp, int n)
{
    int i = 0;
    if (n < 0)
        return 0;
    ListNode *p = lp->head;
    ListNode *p1 = NULL;
    for (;i < n-1 && p; p = p->next, ++i);
    p1 = p->next;
    p->next = p1->next;
    free(p1);
    return 1;
}

ListNode* NewNode(File *data)
{
    ListNode *p = NULL;
    p = (ListNode *)malloc(sizeof(ListNode));
    if (!p)
    {
        return p;
    }
    memcpy(&p->data, data, sizeof(File));
    p->next = NULL;
    return p;
}
