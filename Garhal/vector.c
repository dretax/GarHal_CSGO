#include <stdio.h>
#include "ntifs.h"
#include "vector.h"

void vector_init(vector* v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = (void**)MmAllocateNonCachedMemory(sizeof(void*) * v->capacity);
}

int vector_total(vector* v)
{
    if (v == NULL)
    {
        return -1;
    }

    return v->total;
}

static void vector_resize(vector* v, int capacity)
{
    if (v == NULL || v->items == NULL)
    {
        return;
    }

    void** items = (void**)MmAllocateNonCachedMemory(sizeof(void*) * capacity);
    if (items)
    {
        for (int i = 0; i < v->total; i++)
        {
            items[i] = v->items[i];
        }

        vector_free(v);

        v->items = items;
        v->capacity = capacity;
    }
}

void vector_add(vector* v, void* item)
{
    if (v == NULL || v->items == NULL)
    {
        return;
    }

    if (v->capacity == v->total)
    {
        vector_resize(v, v->capacity * 2);
    }

    v->items[v->total++] = item;
    v->capacity = v->total;
}

void vector_set(vector* v, int index, void* item)
{
    if (v != NULL && v->items != NULL && index >= 0 && index < v->total)
    {
        v->items[index] = item;
    }
}

void* vector_get(vector* v, int index)
{
    if (v != NULL && v->items != NULL && index >= 0 && index < v->total)
    {
        return v->items[index];
    }
    return NULL;
}

void vector_delete(vector* v, int index)
{
    if (v == NULL || v->items == NULL)
    {
        return;
    }

    if (index < 0 || index >= v->total)
    {
        return;
    }

    v->items[index] = NULL;

    for (int i = index; i < v->total - 1; i++)
    {
        v->items[i] = v->items[i + 1];
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
    {
        vector_resize(v, v->capacity / 2);
    }
}

void vector_free(vector* v)
{
    if (v != NULL && v->items != NULL)
    {
        MmFreeNonCachedMemory(v->items, sizeof(void*) * v->capacity);
    }
}