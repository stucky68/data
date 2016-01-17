#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
typedef struct
{
    int max_work_num;
    int min_work_num;
    int cur_work_num;
    int this_work_num;
}Pool;

Pool *thread_pool_init(int maxnum, int minnum);

void thread_pool_destroy(Pool *p);


#define
