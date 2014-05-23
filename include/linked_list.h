#ifndef LINKED_LIST_H_
# define LINKED_LIST_H_

# include <stdlib.h>

typedef struct
{
  void *next;
  void *prev;
  void *list;
} header_list_t;

typedef struct
{
  header_list_t *_header;
} element_list_t;

typedef struct
{
  void *begin;
  void *end;
  size_t length;
  size_t _node_size;
} list_t;

typedef int (*iterator_f)(size_t, void *, list_t *);

list_t *create_list(size_t);
int add_element_to_list(list_t *, void const *);
int for_each(list_t *, iterator_f);

#endif
