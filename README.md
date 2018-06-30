lw_coroutine
============
仅一个bp.h文件，它类似于protothread，但采用了完全不同的实现机制，它使用switch-goto实现，相比protothread有如下优势：

* 最低的资源占用，仅使用1字节进行状态保存
* bp支持在switch中进行yield, 而protothread不支持(仅gcc编译器支持，但移植性差)
* bp有更高效的跳转能力，因为bp中的case是从0连续的，因此可以被优化成最高效的跳转表，而protothread使用行号，具有随机性，因此protothread的跳转性能较差
* bp的断点号是非常容易识别的，因此可以在bp_begin之前可用于判断当前yield的位置，以实现特殊的需求。而protothread使用的行号是不可识别的，因此其状态信息不能在begin之前被解读出来。
* bp更精简，使用时仅依赖一个状态变量，因此可以被用于任何的异步回调中，以实现协程。

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
There is only one bp.h file, which is similar to protothread, but uses a completely different implementation mechanism. It is implemented using switch-goto and has the following advantages over protothread:

* Lowest resource usage, use only 1 byte for state preservation
* bp supports yield in switch, but protothread does not support (only gcc compiler support, but poor portability)
* bp has a more efficient ability to jump, because the case in bp is continuous from 0, so it can be optimized to the most efficient jump table, and protothread use the line number, with randomness, so the protothread jump performance is poor
* The breakpoint number of bp is very easy to identify, so it can be used to determine the current yield position before bp_begin to achieve special needs. The line number used by protothread is unrecognizable, so its status information cannot be interpreted before PT_BEGIN.
* The bp is more streamlined, using only one state variable when used, so it can be used in any asynchronous callback to implement coroutines.

Usage：
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
