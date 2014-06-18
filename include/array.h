#ifndef ARRAY_H_
# define ARRAY_H_

typedef struct
{
  void *_data;
  size_t length;
  size_t _node_size;
} array_t;

# define new_array(s, x) array_create(s, sizeof(x))
# define delete_array(x) array_delete(x)
typedef array_t *array;
array_t *array_create(size_t length, size_t node_size);
void array_delete(array_t *array);

#endif
