#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rson.h"
#include "rson_deserialize.h"

size_t get_size(char *serial_string, size_t *offset) {
	size_t i;
	char *size_string;

	size_string = allocate_string(0);

	for (i = 0; *offset < strlen(serial_string) && i < INTEGER_DIGITS; *offset += 1, i++) {
		if (serial_string[*offset] == ':') {
			break;
		}
		if (serial_string[*offset] >= '0' && serial_string[*offset] <= '9') {
			append_char_to_string(size_string, serial_string[*offset]);
		}
		else {
			print_error("error");
		}
	}
	*offset += 1;
	return atoi(size_string);
}

rson_object *rson_deserialize_dict(char *serial_string, size_t *offset) {
	bool have_entry, have_key, have_value;
	rson_object *dict_object;
	rson_dict *dict;
	rson_dict_entry *entry;
	rson_object *(* parse_func)(char *, size_t *);

	dict_object = rson_create_object();
	dict_object->type = RSON_DICT;


	dict = rson_create_dict(0);
	entry = rson_create_dict_entry(NULL, NULL);
	have_entry = true;
	have_key = false;
	have_value = false;

	while (*offset < strlen(serial_string)) {
		switch (serial_string[*offset]) {
			case 'N':
				if (!have_entry && !have_key && !have_value) {
					*offset += 1;
					entry = rson_create_dict_entry(NULL, NULL);
					have_entry = true;
				}
				else {
					print_error("Unexpected dict entry in dict parse");
				}
				break;
			case 'L':
				if (have_entry && have_key && !have_value) {
					*offset += 1;
					entry->value = rson_deserialize_list(serial_string, offset);
					have_value = true;
				}
				else {
					print_error("Unexpected list in dict parse");
				}
				break;
			case 'S':
				parse_func = rson_deserialize_string;
				goto parse_string_int;
			case 'I':
				parse_func = rson_deserialize_integer;
				goto parse_string_int;

			parse_string_int:
				if (!have_entry) {
					print_error("No entry has been created");
				}

				if (!have_key) {
					*offset += 1;
					entry->key = parse_func(serial_string, offset);
					have_key = true;
				}
				else if (!have_value) {
					*offset += 1;
					entry->value = parse_func(serial_string, offset);
					have_value = true;
				}
				else {
					print_error("Unexpected item in dict parse");
				}

				if (have_entry && have_key && have_value) {
					rson_add_to_dict(dict, entry);
					have_entry = false;
					have_key = false;
					have_value = false;
				}
				break;
			default:
				print_error("Unknown item to be parsed in dict parse");
		}
	}

	dict_object->value = dict;
	return dict_object;
}

rson_object *rson_deserialize_list(char *serial_string, size_t *offset) {
	size_t list_size, items;
	rson_object *list_object, *object;
	rson_list *list;

	list_object = rson_create_object();
	list_object->type = RSON_LIST;

	list = rson_create_list(0);
	list_size = get_size(serial_string, offset);

	while (*offset < strlen(serial_string) && items <= list_size) {
		switch (serial_string[*offset]) {
			case 'N':
				*offset += 1;
				object = rson_deserialize_dict(serial_string, offset);
				goto add_item;
			case 'L':
				*offset += 1;
				object = rson_deserialize_list(serial_string, offset);
				goto add_item;
			case 'S':
				*offset += 1;
				object = rson_deserialize_string(serial_string, offset);
				goto add_item;
			case 'I':
				*offset += 1;
				object = rson_deserialize_integer(serial_string, offset);
			add_item:
				rson_add_to_list(list, object);
				items++;
				break;
			default:
				print_error("Unknown item to be parsed in list parse");
		}
	}
	list_object->value = list;
	return list_object;
}

rson_object *rson_deserialize_string(char *serial_string, size_t *offset) {
	size_t string_size;
	char *string;
	rson_object *object;

	object = rson_create_object();
	object->type = RSON_STR;

	string_size = get_size(serial_string, offset);
	if (string_size > strlen(serial_string)) {
		print_error("string length too long");
	}
	string = allocate_string(string_size);
	strncpy(string, serial_string + *offset, string_size);

	*offset += strlen(string);
	object->value = string;
	return object;
}

rson_object *rson_deserialize_integer(char *serial_string, size_t *offset) {
	int integer, neg;
	int *int_pointer;
	char *integer_string;
	rson_object *object;

	object = rson_create_object();
	object->type = RSON_INT;

	integer_string = allocate_string(0);

	neg = 1;
	if (*offset < strlen(serial_string) && serial_string[*offset] == '-') {
		neg = -1;
		*offset += 1;
	}

	for (; *offset < strlen(serial_string); *offset += 1) {
		if (serial_string[*offset] == ':') {
			break;
		}
		if (serial_string[*offset] >= '0' && serial_string[*offset] <= '9') {
			append_char_to_string(integer_string, serial_string[*offset]);
		}
		else {
			print_error("Character is not numeric in integer parse");
		}
	}
	*offset += 1;

	integer = atoi(integer_string) * neg;
	int_pointer = malloc(sizeof(int));
	if (int_pointer == NULL) {
		print_error("Cannot allocate integer");
	}
	*int_pointer = integer;
	object->value = int_pointer;
	return object;
}

rson_object *rson_deserialize(char *serialized_string) {
	size_t i;
	rson_object *object;

	object = NULL;

	if (serial_string[0] == 'N') {
		i = 1;
		object = rson_deserialize_dict(serialized_string, &i);
	}
	else {
		print_error("Given string is not dictionary");
	}

	if (object == NULL) {
		print_error("Unable to deserialize given string");
	}
	return object;
}
