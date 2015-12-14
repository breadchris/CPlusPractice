#include "rson.h"

#ifndef RSON_SERIAL_H
#define RSON_SERIAL_H

char *rson_serialize_type(rson_object *object);
char *rson_serialize_dict(rson_dict *dict);
char *rson_serialize_list(rson_list *list);
char *rson_serialize_string(char *string);
char *rson_serialize_integer(int integer);
char *rson_serialize_type(rson_object *object);
char *rson_serialize(rson_object *object);
void rson_serialize_test();

#endif