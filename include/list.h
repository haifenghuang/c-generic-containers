#ifndef LIST_H_
# define LIST_H_

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
typedef int (*_list_register_comparator_f)(list_t *, generic_comparator_f);
typedef int (*_list_unregister_comparator_f)(list_t *);
typedef void (*_list_sort_f)(list_t *);
typedef int (*_list_prepend_f)(list_t *, void const *);
typedef int (*_list_remove_f)(list_t *, void const *);
typedef int (*_list_remove_at_f)(list_t *, size_t);
typedef size_t (*_list_find_f)(list_t *list, void const *el);

struct _list_s
{
  /* Private properties */
  list_element_t *_begin;
  list_element_t *_end;
  size_t _node_size;

  /* Private methods */
  generic_deleter_f _deleter;
  generic_comparator_f _comparator;

  /* Public properties */
  size_t length;

  /* Public methods */
  _list_append_f append;
  _list_foreach_f foreach;
  _list_register_deleter_f register_deleter;
  _list_unregister_deleter_f unregister_deleter;
  _list_sort_f sort;
  _list_prepend_f prepend;
  _list_remove_f remove;
  _list_remove_at_f remove_at;
  _list_find_f find;
  _list_register_comparator_f register_comparator;
  _list_unregister_comparator_f unregister_comparator;
};

# define new_list(x) list_create(sizeof(x))
# define delete_list(x) list_delete(x)
typedef list_t *list;
list_t *list_create(size_t);
void list_delete(list_t *);

#endif
