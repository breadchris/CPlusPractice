#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rson.h"
#include "rson_serialize.h"

char *rson_serialize_dict(rson_dict *dict) {
	size_t i, serial_dict_len, serial_part_len, serial_key_len,
	       serial_value_len;
	char *serial_dict, *serial_value, *serial_key, *serial_dict_tmp;

	serial_dict = allocate_string(1);
	serial_dict[0] = '\0';

	for (i = 0; i < dict->size; i++) {
		serial_key = rson_serialize_type(dict->entries[i]->key);
		serial_value = rson_serialize_type(dict->entries[i]->value);

		serial_key_len = strlen(serial_key);
		serial_value_len = strlen(serial_value);
		serial_dict_len = strlen(serial_dict);
		if (serial_key_len >= SIZE_MAX - serial_value_len) {
			print_error("Unsigned integer overflow");
		}

		serial_part_len = serial_key_len + serial_value_len;
		if (serial_part_len >= SIZE_MAX - serial_dict_len) {
			print_error("Unsigned integer overflow");
		}
		// Each rson dictionary entry is formatted as: "N<key>:<value>"
		serial_part_len += serial_dict_len + 2;
		serial_dict_tmp = allocate_string(serial_part_len);

		if (serial_dict != NULL) {
			sprintf(serial_dict_tmp, "%sN%s%s", serial_dict, serial_key, serial_value);
			free(serial_dict);
		} else {
			sprintf(serial_dict_tmp, "N%s%s", serial_key, serial_value);
		}
		serial_dict = serial_dict_tmp;
	}

	return serial_dict;
}

char *rson_serialize_list(rson_list *list) {
	int i;
	size_t serial_list_len, serial_part_len;
	char *serial_list, *serial_part, *list_size;

	list_size = int_to_string(list->size);

	// rson list begins with: "L<number of list items>:"
	serial_list = allocate_string(strlen(list_size) + 2);
	sprintf(serial_list, "L%s:", list_size);

	for (i = 0; i < list->size; i++) {
		serial_part = rson_serialize_type(list->objects[i]);

		serial_part_len = strlen(serial_part);
		serial_list_len = strlen(serial_list);
		if (serial_part_len >= SIZE_MAX - serial_list_len) {
			print_error("Unsigned integer overflow");
		}
		serial_list = append_to_string(serial_list, serial_part);	
	}

	free(list_size);
	return serial_list;
}

char *rson_serialize_string(char *string) {
	size_t allocate_size;
	char *string_length, *serial_string;

	string_length = int_to_string(strlen(string));
	allocate_size = strlen(string_length) + strlen(string) + 2;
	serial_string = allocate_string(allocate_size);

	sprintf(serial_string, "S%s:%s", string_length, string);

	free(string_length);
	return serial_string;
}

char *rson_serialize_integer(int integer) {
	char *int_string, *serial_int;

	int_string = int_to_string(integer);
	serial_int = allocate_string(strlen(int_string) + 2);

	sprintf(serial_int, "I%s:", int_string);

	free(int_string);
	return serial_int;
}

char *rson_serialize_type(rson_object *object) {
	int integer;
    char *serial_object = NULL, *string;
    rson_dict *dict;
    rson_list *list;

    switch (object->type) {
        case RSON_DICT:
        	dict = (rson_dict *) object->value;
            serial_object = rson_serialize_dict(dict);
            break;
        case RSON_LIST:
        	list = (rson_list *) object->value;
            serial_object = rson_serialize_list(list);
            break;
        case RSON_STR:
        	string = (char *) object->value;
            serial_object = rson_serialize_string(string);
            break;
        case RSON_INT:
        	integer = *(int *) object->value;
            serial_object = rson_serialize_integer(integer);
            break;
        default:
        	print_error("Unrecognized type");
    }

    if (serial_object == NULL) {
        print_error("Unable to serialize object");
    }
    return serial_object;
}

char *rson_serialize(rson_object *object) {
	return rson_serialize_type(object);
}

void rson_serialize_test() {
	rson_object *object;
	char *string;
	int integer;
	rson_list *list;
	rson_dict *dict;

	object = (rson_object *) malloc(sizeof(rson_object));

	// String
	string = allocate_string(strlen("asdf"));
	sprintf(string, "asdf");

	object->type = RSON_STR;
	object->value = string;

	printf("[*] Testing strings: %s\n", rson_serialize(object));

	// Integer
	integer = 42;
	object->type = RSON_INT;
	object->value = &integer;

	printf("[*] Testing integers: %s\n", rson_serialize(object));

	// List
	list = rson_create_list(4);
	list->objects[0] = rson_create_integer(15);
	list->objects[1] = rson_create_string(string);
	list->objects[2] = rson_create_integer(42);
	list->objects[3] = rson_create_string(string);
	object->type = RSON_LIST;
	object->value = list;

	printf("[*] Testing lists: %s\n", rson_serialize(object));

	// Dictionary
	dict = rson_create_dict(4);
	dict->entries[0] = rson_create_dict_entry(rson_create_string(string), rson_create_integer(15));
	dict->entries[1] = rson_create_dict_entry(rson_create_string(string), rson_create_integer(15));
	dict->entries[2] = rson_create_dict_entry(rson_create_string(string), rson_create_integer(15));
	dict->entries[3] = rson_create_dict_entry(rson_create_string(string), rson_create_integer(15));
	object->type = RSON_DICT;
	object->value = dict;

	printf("[*] Testing dictionaries: %s\n", rson_serialize(object));

	rson_free_object(object);
}
