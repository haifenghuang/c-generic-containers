#include <stdio.h>
#include "../include/containers.h"

int
iterator(size_t i, void *el, list list)
{
  int *element = el;

  (void)list;
  printf("%li -> %i\n", i, *element);
  return 0;
}

int
comparator(void const *i1, void const *i2)
{
  return *(int const *)i2 - *(int const *)i1;
}

int
main (void)
{
  list l;
  int i; 

  if ((l = new_list(int)) == NULL)
    {
      return 1;
    }
  l->register_comparator(l, comparator);
  i = 0;
  l->append(l, &i);
  i = 5;
  l->append(l, &i);
  i = 12;
  l->append(l, &i);
  i = 2;
  l->append(l, &i);
  i = 16;
  l->append(l, &i);
  l->sort(l);
  i = 5;
  l->remove(l, &i);
  i = 3;
  l->append(l, &i);
  l->remove_at(l, l->length - 1);
  i = -1;
  l->prepend(l, &i);
  l->foreach(l, &iterator);
  i = 12;
  printf("Index of 12 : %li\n", l->find(l, &i));
  i = 14;
  printf("Index of 14 : %li\n", l->find(l, &i));
  delete_list(l);
  return 0;
}
