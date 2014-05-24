#ifndef CONTAINERS_H_
# define CONTAINERS_H_

# include <stdlib.h>

typedef struct _list_element_s list_element_t;
typedef struct _list_s list_t;

typedef struct
{
  list_element_t *next;
  list_element_t *prev;
  list_t *list;
} list_header_t;

struct _list_element_s
{
  list_header_t *_header;
  void *data;
};

typedef void (*generic_deleter_f)(void *);
typedef int (*list_iterator_f)(size_t, void *, list_t *);
typedef int (*generic_comparator_f)(void const *, void const *);

typedef int (*_list_append_f)(list_t *, void const *);
typedef int (*_list_foreach_f)(list_t *, list_iterator_f);
typedef int (*_list_register_deleter_f)(list_t *, generic_deleter_f);
typedef int (*_list_unregister_deleter_f)(list_t *);
typedef void (*_list_sort_f)(list_t *, generic_comparator_f);

struct _list_s
{
  list_element_t *begin;
  list_element_t *end;
  size_t length;
  size_t _node_size;
  generic_deleter_f _deleter;
  _list_append_f append;
  _list_foreach_f foreach;
  _list_register_deleter_f register_deleter;
  _list_unregister_deleter_f unregister_deleter;
  _list_sort_f sort;
};

# define new_list(x) list_create(sizeof(x))
# define delete_list(x) list_delete(x)
list_t *list_create(size_t);
void list_delete(list_t *);

#endif
