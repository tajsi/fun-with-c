#ifndef SMIT_ALLOCATOR_H
#define SMIT_ALLOCATOR_H

#include "allocator.h"

typedef struct sm_alloc_ctx {
    void  *buffer;
    off_t  pos;
    size_t left;
    size_t size;
    int    flags;
    struct sm_alloc_ctx *next;
    struct sm_alloc_ctx *prev;
} sm_alloc_ctx;

static sm_alloc_ctx *head = NULL;
static sm_alloc_ctx *tail = NULL;

size_t
calc_size_in_pages (
    size_t req_size,
    size_t page_size
    )
{
    if (req_size == 0)
        return 0;
    
    if (req_size % page_size == 0)
        return req_size;
    else
        return (req_size % page_size) * page_size + page_size;
}

sm_alloc_ctx *
smit_alloc_init (
    size_t size,
    int    flags
    )
{
    sm_alloc_ctx *ret = NULL;
    void *buf = NULL;
    size_t calc_size = calc_size_in_pages(size, 4096);

    if (calc_size == 0)
        return ret;

    buf = malloc(calc_size);
    if (buf == NULL)
        return ret;

    ret = (sm_alloc_ctx *) malloc(sizeof(struct sm_alloc_ctx));
    if (ret == NULL) {
        free(buf);
        return ret;
    }

    ret->buffer = buf;
    ret->pos = 0;
    ret->left = calc_size;
    ret->size = calc_size;
    ret->flags = 0; // To Be Used
    ret->next = NULL;
    ret->prev = tail;

    tail = ret;

    if (head == NULL)
        head = ret;

    return ret;
}

void
smit_alloc_free (
    sm_alloc_ctx *ctx
    )
{
    if (ctx == NULL)
        return;


}
#endif
