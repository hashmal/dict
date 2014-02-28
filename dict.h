// Copyright (c) 2014, Jérémy Pinat.

// Usage
// -----

// Include this file as a header, as usual. Define DICT_IMPL before including
// this file where you want the actual implementation.

//     #define DICT_IMPL
//     #include "dict.h"

#ifndef DICT_H
#define DICT_H

typedef void dict;
// A dictionary associate keys (strings) with values (pointers.) This
// implementation uses a trie instead of a hash table.

dict *dictnew();
// Allocate and initialize an empty dictionary.

void dictfree(dict *d);
// Free a dictionary from memory. Stored values are not freed.

void *dictset(dict *d, char *k, void *v);
// Set a value for a given key. If a value already exists for the key, it is
// replaced by the new one and returned by the function. If no value exists,
// NULL is returned instead. To remove a value from the dictionary, pass NULL
// as the value.

void *dictget(dict *d, char *k);
// Get a value for a given key, or NULL if no value found.

unsigned int dictsize(dict *d);
// Get the number of values stored in the dictionary.

void dictvals(dict *d, void **v);
// Get all values stored in the dictionary using a buffer. Behavior is
// undefined if the buffer is too small (use dictsize to allocate a buffer of
// the appropriate size.)

#endif // DICT_H

#ifdef DICT_IMPL
#undef DICT_IMPL

#include <stdlib.h>

#define dictcast(d) ((struct dict*)d)

struct dict {
    char k;
    void *v;
    char n;
    struct dict *c;
};

dict *dictnew()
{
    return calloc(1, sizeof(struct dict));
}

void dictfree(dict *d)
{
    struct dict *s = (struct dict*)d;
    for (size_t i = 0; i < s->n; ++i)
        dictfree(&s->c[i]);
    free(s->c);
    if(s->k)
        free(s);
}

void *dictset(dict *d, char *k, void *v)
{
    struct dict *s = (struct dict*)d;
    begin:
    while (k[0]) {
        for (size_t i = 0; i < s->n; ++i)
            if (k[0] == s->c[i].k) {
                ++k;
                s = &s->c[i];
                goto begin;
            }
        if (s->c)
            s->c = realloc(s->c, sizeof(struct dict) * (s->n + 1));
        else
            s->c = malloc(sizeof(struct dict));
        s->n++;
        s = &s->c[s->n-1];
        s->k = k[0];
        s->v = NULL;
        s->n = 0;
        s->c = NULL;
        ++k;
    }
    void *prev = s->v;
    s->v = v;
    return prev;
}

void *dictget(dict *d, char *k)
{
    struct dict *s = (struct dict*)d;
    begin:
    while (k[0]) {
        for (size_t i = 0; i < s->n; ++i)
            if (k[0] == s->c[i].k) {
                ++k;
                s = &s->c[i];
                goto begin;
            }
        return NULL;
    }
    return s->v;
}

unsigned int dictsize(dict *d)
{
    struct dict *s = (struct dict*)d;
    int accum = s->v ? 1 : 0;
    for (size_t i = 0; i < s->n; ++i) {
        if (s->c[i].v)
            ++accum;
        accum = accum + dictsize(&s->c[i]);
    }
    return accum;
}

void dictvals(dict *d, void **v)
{
    // TBD
}

void dictprn(dict *d, int indent)
{
    struct dict *s = (struct dict*)d;
    for(size_t i = 0; i < indent; ++i)
        printf(" ");
    printf("%c\n", s->k);
    for(size_t i = 0; i < s->n; ++i)
        dictprn(&s->c[i], indent+1);
}

#endif // DICT_IMPL
