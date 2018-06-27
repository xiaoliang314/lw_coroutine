lw_coroutine
============
利用switch-goto实现的纯C轻量级协程，支持在switch中yield。

用例：
-----

```c
#include "bp.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    /* resource */
    short res;

    /* consumer coroutine breakpoint */
    unsigned char consumer_bp;

    /* produce coroutine breakpoint */
    unsigned char producer_bp;
} res_t;

void consumer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->consumer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource > 0 */
        bpd_yield_until(1, res->res > 0);

        /* consume */
        res->res--;
        printf("consume a resource, number of res:%d\n", res->res);

        /* wait next consume */
        bpd_yield(2);
    }

    /* coroutine end */
    bpd_end();
}

void producer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->producer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource < 30 */
        bpd_yield_until(1, res->res < 30);

        /* produce */
        res->res++;
        printf("produce a resource, number of res:%d\n", res->res);

        /* wait next produce */
        bpd_yield(2);
    }

    /* coroutine end */
    bpd_end();
}

int main()
{
    res_t res = {0, BP_INIT_VAL, BP_INIT_VAL};
    int nrand;

    /* initialize random */
    srand(122);

    /* randomly produce and consume */
    while (1)
    {
        nrand = rand() % 16;
        while (nrand--)
        {
            consumer(&res);
        }

        nrand = rand() % 16;
        while (nrand--)
        {
            producer(&res);
        }
    }

    return 0;
}
```

lw_coroutine
============
pure C lightweight coroutines implemented using switch-goto support yield in switch.

usage：
-------

```c
#include "bp.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    /* resource */
    short res;

    /* consumer coroutine breakpoint */
    unsigned char consumer_bp;

    /* produce coroutine breakpoint */
    unsigned char producer_bp;
} res_t;

void consumer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->consumer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource > 0 */
        bpd_yield_until(1, res->res > 0);

        /* consume */
        res->res--;
        printf("consume a resource, number of res:%d\n", res->res);

        /* wait next consume */
        bpd_yield(2);
    }

    /* coroutine end */
    bpd_end();
}

void producer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->producer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource < 30 */
        bpd_yield_until(1, res->res < 30);

        /* produce */
        res->res++;
        printf("produce a resource, number of res:%d\n", res->res);

        /* wait next produce */
        bpd_yield(2);
    }

    /* coroutine end */
    bpd_end();
}

int main()
{
    res_t res = {0, BP_INIT_VAL, BP_INIT_VAL};
    int nrand;

    /* initialize random */
    srand(122);

    /* randomly produce and consume */
    while (1)
    {
        nrand = rand() % 16;
        while (nrand--)
        {
            consumer(&res);
        }

        nrand = rand() % 16;
        while (nrand--)
        {
            producer(&res);
        }
    }

    return 0;
}
```
