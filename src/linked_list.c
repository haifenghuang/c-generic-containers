#ifdef DEBUG
# include <stdio.h>
#endif
#include <assert.h>
#include <string.h>
#include "containers.h"

#ifdef DEBUG
static void
print_debug(char const *s)
{
  fprintf(stderr, "libcontainers: %s\n", s);
}
#else
# define print_debug(s) (void)0
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
  header->prev = list->end;
  header->list = list;
  if (list->end)
    {
      list->end->_header->next = element;
    }
  list->end = element;
  if (!list->begin)
    {
      list->begin = element;
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
  header->next = list->begin;
  header->prev = NULL;
  header->list = list;
  if (list->begin)
    {
      list->begin->_header->prev = element;
    }
  list->begin = element;
  if (!list->end)
    {
      list->end = element;
    }
  list->length++;
  print_debug("Exiting list::prepend, return 0");
  return 0;
}

static size_t
list_find(list_t *list, void const *el, generic_comparator_f comparator)
{
  list_element_t *element;
  size_t i = 0;

  print_debug("Entering list::remove");
  element = list->begin;
  while (element)
    {
      if (comparator(element->data, el) == 0)
	{
	  print_debug("Exiting list::remove, return index");
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
list_remove(list_t *list, void const *el, generic_comparator_f comparator)
{
  list_element_t *element;

  print_debug("Entering list::remove");
  element = list->begin;
  while (element)
    {
      if (comparator(element->data, el) == 0)
	{
	  if (element->_header->prev)
	    {
	      element->_header->prev->_header->next = element->_header->next;
	    }
	  else
	    {
	      list->begin = element->_header->next;
	    }
	  if (element->_header->next)
	    {
	      element->_header->next->_header->prev = element->_header->prev;
	    }
	  else
	    {
	      list->end = element->_header->prev;
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
  element = list->begin;
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
	      list->begin = element->_header->next;
	    }
	  if (element->_header->next)
	    {
	      element->_header->next->_header->prev = element->_header->prev;
	    }
	  else
	    {
	      list->end = element->_header->prev;
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
  el = list->begin;
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

static void
list_sort(list_t *list, generic_comparator_f comparator)
{
  /* TODO : Implement another sorting algorithm */
  list_element_t *element;
  void *tmp_data;

  print_debug("Entering list::sort");
  element = list->begin;
  while (element && element->_header->next)
    {
      if (comparator(element->data, element->_header->next->data) < 0)
	{
	  tmp_data = element->data;
	  element->data = element->_header->next->data;
	  element->_header->next->data = tmp_data;
	  element = list->begin;
	}
      else
	{
	  element = element->_header->next;
	}
    }
  print_debug("Exiting list::sort");
}

list_t *
list_create(size_t node_size)
{
  list_t *list;

  print_debug("Entering list constructor");
  if ((list = malloc(sizeof(*list))) == NULL)
    {
      print_debug("Cannot allocate memory for list, return NULL");
      return NULL;
    }
  list->begin = NULL;
  list->end = NULL;
  list->_node_size = node_size;
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
  print_debug("Exiting list constructor, return list");
  return list;
}

void
list_delete(list_t *list)
{
  list_element_t *element;
  list_element_t *next;

  print_debug("Entering list destructor");
  element = list->begin;
  while (element)
    {
      next = element->_header->next;
      _list_free_element(element);
      element = next;
    }
  free(list);
  print_debug("Exiting list destructor");
}
