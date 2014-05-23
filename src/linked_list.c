#include <string.h>
#include "linked_list.h"

list_t *
create_list(size_t node_size)
{
  list_t *list;

  list = malloc(sizeof(*list));
  list->begin = NULL;
  list->end = NULL;
  list->_node_size = node_size;
  list->length = 0;
  return list;
}

int
add_element_to_list(list_t *list, void const *element)
{
  void *new_el;
  header_list_t *header;
  
  if ((new_el = malloc(list->_node_size)) == NULL)
    {
      return -1;
    }
  memcpy(new_el, element, list->_node_size);
  if ((((element_list_t *)new_el)->_header = malloc(sizeof(*header))) == NULL)
    {
      free(new_el);
      return -1;
    }
  header = ((element_list_t *)new_el)->_header;
  header->next = NULL;
  header->prev = list->end;
  header->list = list;
  if (list->end)
    {
      ((element_list_t *)list->end)->_header->next = new_el;
    }
  list->end = new_el;
  if (!list->begin)
    {
      list->begin = new_el;
    }
  list->length++;
  return 0;
}

int
for_each(list_t *list, iterator_f iterator)
{
  void *el;
  size_t i;

  el = list->begin;
  i = 0;
  while (el)
    {
      if (iterator(i, el, list) == -1)
	{
	  return -1;
	}
      ++i;
      el = ((element_list_t *)el)->_header->next;
    }
  return 0;
}

