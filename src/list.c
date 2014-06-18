#ifdef DEBUG
# include <stdio.h>
#endif
#include <assert.h>
#include <string.h>
#include "containers.h"

#ifdef DEBUG
# include <stdarg.h>
static void
print_debug(char const *s)
{
  fprintf(stderr, "libcontainers: %s\n", s);
}

static void
printf_debug(char const *str, ...)
{
  va_list ap;
  char *s;

  if ((s = malloc(sizeof(*s) + strlen(str) + 2)) == NULL)
    {
      return;
    }
  strcpy(s, str);
  s[strlen(str)] = '\n';
  s[strlen(str) + 1] = 0;
  va_start(ap, str);
  vfprintf(stderr, s, ap);
  va_end(ap);
  free(s);
}
#else
# define print_debug(s) ((void)0)
# define printf_debug(args...) ((void)0) 
#endif

static int
list_append(list_t *list, void const *data)
{
  list_element_t *element;
  list_header_t *header;
  
  print_debug("Entering list::append");
  if ((element = malloc(sizeof(*element))) == NULL)
    {
      print_debug("Cannot allocate memory for element, return -1");
      return -1;
    }
  if ((element->data = malloc(list->_node_size)) == NULL)
    {
      print_debug("Cannot allocate memory for data, return -1");
      free(element);
      return -1;
    }
  memcpy(element->data, data, list->_node_size);
  if ((element->_header = malloc(sizeof(*header))) == NULL)
    {
      print_debug("Cannot allocate memory for header, return -1");
      free(element->data);
      free(element);
      return -1;
    }
  header = element->_header;
  header->next = NULL;
  header->prev = list->_end;
  header->list = list;
  if (list->_end)
    {
      list->_end->_header->next = element;
    }
  list->_end = element;
  if (!list->_begin)
    {
      list->_begin = element;
    }
  list->length++;
  print_debug("Exiting list::append, return 0");
  return 0;
}

static int
list_prepend(list_t *list, void const *data)
{
  list_element_t *element;
  list_header_t *header;

  print_debug("Entering list::prepend");
  if ((element = malloc(sizeof(*element))) == NULL)
    {
      print_debug("Cannot allocate memory for element, return -1");
      return -1;
    }
  if ((element->data = malloc(list->_node_size)) == NULL)
    {
      free(element);
      print_debug("Cannot allocate memory for data, return -1");
      return -1;
    }
  memcpy(element->data, data, list->_node_size);
  if ((element->_header = malloc(sizeof(*element->_header))) == NULL)
    {
      free(element);
      free(element->data);
      print_debug("Cannot allocate memory for header, return -1");
      return -1;
    }
  header = element->_header;
  header->next = list->_begin;
  header->prev = NULL;
  header->list = list;
  if (list->_begin)
    {
      list->_begin->_header->prev = element;
    }
  list->_begin = element;
  if (!list->_end)
    {
      list->_end = element;
    }
  list->length++;
  print_debug("Exiting list::prepend, return 0");
  return 0;
}

static size_t
list_find(list_t const *list, void const *el)
{
  list_element_t *element;
  size_t i = 0;

  print_debug("Entering list::find");
  element = list->_begin;
  while (element)
    {
      if (list->_comparator(element->data, el) == 0)
	{
	  printf_debug("Exiting list::find, return %i", i);
	  return i;
	}
      element = element->_header->next;
      ++i;
    }
  print_debug("No element found, return -1");
  return -1;
}

static void
_list_free_element(list_element_t *element)
{
  if (element->_header->list->_deleter)
    {
      element->_header->list->_deleter(element);
    }
  free(element->data);
  free(element->_header);
  free(element);
}

static int
list_remove(list_t *list, void const *el)
{
  list_element_t *element;

  print_debug("Entering list::remove");
  element = list->_begin;
  while (element)
    {
      if (list->_comparator(element->data, el) == 0)
	{
	  if (element->_header->prev)
	    {
	      element->_header->prev->_header->next = element->_header->next;
	    }
	  else
	    {
	      list->_begin = element->_header->next;
	    }
	  if (element->_header->next)
	    {
	      element->_header->next->_header->prev = element->_header->prev;
	    }
	  else
	    {
	      list->_end = element->_header->prev;
	    }
	  _list_free_element(element);
	  list->length--;
	  print_debug("Exiting list::remove, return 0");
	  return 0;
	}
      element = element->_header->next;
    }
  print_debug("No element found, return -1");
  return -1;
}

static int
list_remove_at(list_t *list, size_t index)
{
  list_element_t *element;
  size_t i = -1;

  print_debug("Entering list::remove_at");
  if (index >= list->length)
    {
      print_debug("Index out of bound, return -1");
      return -1;
    }
  element = list->_begin;
  while (element)
    {
      if (++i == index)
	{
	  if (element->_header->prev)
	    {
	      element->_header->prev->_header->next = element->_header->next;
	    }
	  else
	    {
	      list->_begin = element->_header->next;
	    }
	  if (element->_header->next)
	    {
	      element->_header->next->_header->prev = element->_header->prev;
	    }
	  else
	    {
	      list->_end = element->_header->prev;
	    }
	  _list_free_element(element);
	  list->length--;
	  print_debug("Exiting list::remove_at, return 0");
	  return 0;
	}
      element = element->_header->next;
    }
  print_debug("Something went wrong, return -1");
  return -1;
}

static int
list_foreach(list_t *list, list_iterator_f iterator)
{
  list_element_t *el;
  size_t i;

  print_debug("Entering list::foreach");
  el = list->_begin;
  i = 0;
  while (el)
    {
      if (iterator(i, el->data, list) == -1)
	{
	  print_debug("Iterator failed, return -1");
	  return -1;
	}
      ++i;
      el = el->_header->next;
    }
  print_debug("Exiting list::foreach, return 0");
  return 0;
}

static int
list_register_deleter(list_t *list, generic_deleter_f deleter)
{
  print_debug("Entering list::register_deleter");
  if (list->_deleter != NULL)
    {
      print_debug("Cannot register deleter, return -1");
      return -1;
    }
  list->_deleter = deleter;
  print_debug("Exiting list::register_deleter, return 0");
  return 0;
}

static int
list_unregister_deleter(list_t *list)
{
  print_debug("Entering list::unregister_deleter");
  if (list->_deleter == NULL)
    {
      print_debug("No deleter registered, return -1");
      return -1;
    }
  list->_deleter = NULL;
  print_debug("Exiting list::unregister_deleter, return 0");
  return 0;
}

static int
list_register_comparator(list_t *list, generic_comparator_f comparator)
{
  print_debug("Entering list::register_comparator");
  if (list->_comparator != NULL)
    {
      print_debug("Cannot register deleter, return -1");
      return -1;
    }
  list->_comparator = comparator;
  print_debug("Exiting list::register_comparator, return 0");
  return 0;
}

static int
list_unregister_comparator(list_t *list)
{
  print_debug("Entering list::unregister_comparator");
  if (list->_comparator == NULL)
    {
      print_debug("No comparator registered, return -1");
      return -1;
    }
  list->_comparator = NULL;
  print_debug("Exiting list::unregister_comparator, return 0");
  return 0;
}

static void
list_sort(list_t *list)
{
  /* TODO : Implement another sorting algorithm */
  list_element_t *element;
  void *tmp_data;

  print_debug("Entering list::sort");
  element = list->_begin;
  while (element && element->_header->next)
    {
      if (list->_comparator(element->data, element->_header->next->data) < 0)
	{
	  tmp_data = element->data;
	  element->data = element->_header->next->data;
	  element->_header->next->data = tmp_data;
	  element = list->_begin;
	}
      else
	{
	  element = element->_header->next;
	}
    }
  print_debug("Exiting list::sort");
}

static list_t *
list_first(list_t const *list, size_t length)
{
  list_t *new_list;
  size_t i;
  struct _list_element_s *element;

  new_list = list_create(list->_node_size, list->_type);
  element = list->_begin;
  for (i = 0;i < length && i < list->length;++i)
    {
      new_list->append(new_list, element->data);
      element = element->_header->next;
    }
  return new_list;
}

static list_t *
list_initial(list_t const *list, size_t length)
{
  list_t *new_list;
  size_t i;
  struct _list_element_s *element;

  new_list = list_create(list->_node_size, list->_type);
  element = list->_begin;
  for (i = 0; i < list->length - length;++i)
    {
      new_list->append(new_list, element->data);
      element = element->_header->next;
    }
  return new_list;
}

static list_t *
list_last(list_t const *list, size_t length)
{
  list_t *new_list;
  struct _list_element_s *element;
  size_t i;

  new_list = list_create(list->_node_size, list->_type);
  element = list->_end;
  for (i = 0;i < length; ++i)
    {
      new_list->prepend(new_list, element->data);
      element = element->_header->prev;
    }
  return new_list;
}

static list_t *
list_rest(list_t const *list, size_t length)
{
  list_t *new_list;
  struct _list_element_s *element;
  size_t i;

  new_list = list_create(list->_node_size, list->_type);
  element = list->_end;
  for (i = 0;i < list->length - length;++i)
    {
      new_list->prepend(new_list, element->data);
      element = element->_header->prev;
    }
  return new_list;
}

static list_t * const *
list_partition(list_t const *list, generic_predicate_f predicate)
{
  list_t **new_list;
  struct _list_element_s *element;

  new_list = malloc(sizeof(*new_list) * 2);
  new_list[0] = list_create(list->_node_size, list->_type);
  new_list[1] = list_create(list->_node_size, list->_type);
  element = list->_begin;
  while (element)
    {
      if (predicate(element))
	{
	  new_list[0]->append(new_list[0], element->data);
	}
      else
	{
	  new_list[1]->append(new_list[1], element->data);
	}
      element = element->_header->next;
    }
  return new_list;
}

static list_t *
list_concat(list_t *list, list_t const *list2)
{
  struct _list_element_s *element;

  if (strcmp(list->_type, list2->_type) != 0)
    {
      return NULL;
    }
  element = list2->_begin;
  while (element)
    {
      list->append(list, element->data);
      element = element->_header->next;
    }
  return list;
}

/*static list_t *
list_unique(list_t const *list)
{
  list_t *new_list;
  struct _list_element_s *element;

  new_list = list_create(list->_node_size);
  element = new_list->_begin;
  while (element)
    {
      if (new_list->find(new_list, element->data) == (size_t)-1)
	{
	  new_list->append(new_list, element->data);
	}
      element = element->_header->next;
    }
  return new_list;
}*/

list_t *
list_create(size_t node_size, char const *type)
{
  list_t *list;

  print_debug("Entering list constructor");
  if ((list = malloc(sizeof(*list))) == NULL)
    {
      print_debug("Cannot allocate memory for list, return NULL");
      return NULL;
    }
  list->_begin = NULL;
  list->_end = NULL;
  list->_node_size = node_size;
  list->_type = type;
  list->length = 0;
  list->_deleter = NULL;
  list->append = list_append;
  list->foreach = list_foreach;
  list->register_deleter = list_register_deleter;
  list->unregister_deleter = list_unregister_deleter;
  list->sort = list_sort;
  list->prepend = list_prepend;
  list->remove = list_remove;
  list->remove_at = list_remove_at;
  list->find = list_find;
  list->register_comparator = list_register_comparator;
  list->unregister_comparator = list_unregister_comparator;
  list->last = list_last;
  list->rest = list_rest;
  list->first = list_first;
  list->initial = list_initial;
  list->partition = list_partition;
  list->concat = list_concat;
  printf_debug("Exiting list constructor, return %p", list);
  return list;
}

void
list_delete(list_t *list)
{
  list_element_t *element;
  list_element_t *next;

  print_debug("Entering list destructor");
  element = list->_begin;
  while (element)
    {
      next = element->_header->next;
      _list_free_element(element);
      element = next;
    }
  free(list);
  print_debug("Exiting list destructor");
}
