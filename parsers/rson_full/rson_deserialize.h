#include "rson.h"

#ifndef RSON_DESERIAL_H
#define RSON_DESERIAL_H

size_t get_size(char *serial_string, size_t *offset);
rson_object *rson_deserialize_dict(char *serial_string, size_t *offset);
rson_object *rson_deserialize_list(char *serial_string, size_t *offset);
rson_object *rson_deserialize_string(char *serial_string, size_t *offset);
rson_object *rson_deserialize_integer(char *serial_string, size_t *offset);
rson_object *rson_deserialize(char *serialized_string);

#endif