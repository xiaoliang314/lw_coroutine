/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

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

#define TRUE    1
#define FALSE   (!TRUE)

int consumer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->consumer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource > 0 */
        while (res->res <= 0)
        {
            bpd_yield(1) FALSE;
        }

        /* consume */
        res->res--;
        printf("consume a resource, number of res:%d\n", res->res);

        /* wait next consume */
        bpd_yield(2) TRUE;
    }

    /* coroutine end */
    bpd_end();
}

int producer(res_t *res)
{
    /* bpd coroutine default breakpoint pointer */
    unsigned char *bpd = &res->producer_bp;

    /* coroutine begin */
    bpd_begin(2);

    while (1)
    {
        /* wait number of resource < 30 */
        while (res->res >= 30)
        {
            bpd_yield(1) FALSE;
        }

        /* produce */
        res->res++;
        printf("produce a resource, number of res:%d\n", res->res);

        /* wait next produce */
        bpd_yield(2) TRUE;
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
            if (consumer(&res) == FALSE)
                break;
        }

        nrand = rand() % 16;
        while (nrand--)
        {
            if (producer(&res) == FALSE)
                break;
        }
    }

    return 0;
}
