#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rson.h"
#include "rson_serialize.h"
#include "rson_deserialize.h"

void print_error(const char *error) {
	printf("[-] Error: %s\n", error);
	exit(1);
}

void rson_free_object(rson_object *object) {
	switch (object->type) {
		case RSON_INT:
		case RSON_STR:
			free(object->value);
			break;
		case RSON_LIST:
			rson_free_list((rson_list *) object->value);
			break;
		case RSON_DICT:
			rson_free_dict((rson_dict *) object->value);
			break;
		default:
			break;
	}
	free(object);
}

void rson_free_list(rson_list *list) {
	int i;

	for (i = 0; i < list->size; i++) {
		rson_free_object(list->objects[i]);
	}

	free(list);
}

void rson_free_dict_entry(rson_dict_entry *dict_entry) {
	rson_free_object(dict_entry->key);
	rson_free_object(dict_entry->value);

	free(dict_entry);
}

void rson_free_dict(rson_dict *dict) {
	int i;

	for (i = 0; i < dict->size; i++) {
		rson_free_dict_entry(dict->entries[i]);
	}

	free(dict);
}

rson_object *rson_create_object() {
	rson_object *object;

	object = (rson_object *) malloc(sizeof(rson_object));
	if (object == NULL) {
		print_error("Unable to allocate rson object");
	}

	return object;
}

rson_object *rson_create_string(char *string) {
	char *new_string;
	rson_object *object;

	object = rson_create_object();
	object->type = RSON_STR;

	new_string = strdup(string);
	if (new_string == NULL) {
		print_error("Unable to create new rson string");
	}
	object->value = new_string;

	return object;
}

rson_object *rson_create_integer(int integer) {
	int *int_pointer;
	rson_object *object;

	object = rson_create_object();
	int_pointer = (int *) malloc(sizeof(int));
	if (int_pointer == NULL) {
		print_error("Unable to allocate integer");
	}
	*int_pointer = integer;

	object->type = RSON_INT;
	object->value = int_pointer;
	return object;
}

rson_list *rson_create_list(size_t size) {
	rson_list *list;

	list = (rson_list *) malloc(sizeof(rson_list));
	if (list == NULL) {
		print_error("Unable to allocate rson list");
	}
	list->objects = (rson_object **) malloc(sizeof(rson_object *) * size);
	if (list->objects == NULL) {
		print_error("Unable to allocate rson list");
	}
	list->size = size;
	return list;
}

rson_dict *rson_create_dict(size_t size) {
	rson_dict *dict;

	dict = (rson_dict *) malloc(sizeof(rson_dict));
	if (dict == NULL) {
		print_error("Unable to allocate rson dict");
	}

	if (size != 0) {
		dict->entries = (rson_dict_entry **) malloc(sizeof(rson_dict_entry *) * size);
		if (dict->entries == NULL) {
			print_error("Unable to allocate rson dict");
		}
	}
	else {
		dict->entries = NULL;
	}
	dict->size = size;
	return dict;
}

rson_dict_entry *rson_create_dict_entry(rson_object *key, rson_object *value) {
	rson_dict_entry *dict_entry;

	dict_entry = (rson_dict_entry *) malloc(sizeof(rson_dict_entry));
	if (dict_entry == NULL) {
		print_error("Unable to allocate rson dict entry");
	}
	dict_entry->key = key;
	dict_entry->value = value;

	return dict_entry;
}

void rson_add_to_list(rson_list *list, rson_object *object) {
	size_t new_list_size;

	new_list_size = (list->size + 1) * sizeof(rson_object);
	if (new_list_size <= list->size) {
		print_error("Unsigned integer overflow in adding object to list");
	}

	list->objects = realloc(list->objects, new_list_size);
	if (list->size + 1 < list->size) {
		print_error("Unsigned integer overflow in adding object to list");
	}
	list->size += 1;
	list->objects[list->size - 1] = object;
}

void rson_add_to_dict(rson_dict *dict, rson_dict_entry *entry) {
	size_t new_dict_size;

	new_dict_size = (dict->size + 1) * sizeof(rson_dict_entry);
	if (new_dict_size <= dict->size) {
		print_error("Unsigned integer overflow in adding entry to dict");
	}

	dict->entries = realloc(dict->entries, new_dict_size);
	if (dict->size + 1 < dict->size) {
		print_error("Unsigned integer overflow in adding entry to dict");
	}
	dict->size += 1;
	dict->entries[dict->size - 1] = entry;
}

char *allocate_string(size_t size) {
	char *string;

	string = (char *) malloc(size + 1);
	if (string == NULL) {
		print_error("Unable to allocate string");
	}
	memset(string, '\0', size + 1);
	return string;
}

char *append_to_string(char *string, char *append) {
   	size_t string_length, append_length;

    string_length = strlen(string);
    append_length = strlen(append);

    printf("%s :: %s\n", string, append);

    if (string_length >= SIZE_MAX - append_length) {
    	print_error("Unsigned integer overflow in appending to string");
    }

    string = realloc(string, string_length + append_length);
    if (string == NULL) {
    	print_error("Unable to realloc string");
    }
    strcpy(string + string_length, append);

    free(append);
    return string;
}

char *append_char_to_string(char *string, char append) {
   	size_t string_length, append_length;

    string_length = strlen(string);

    string = realloc(string, string_length + 1);
    if (string == NULL) {
    	print_error("Unable to realloc string");
    }

    string[string_length] = append;
    string[string_length + 1] = '\0';
    return string;
}

char *int_to_string(int integer) {
	char *int_string;

	int_string = allocate_string(INTEGER_DIGITS);
	snprintf(int_string, INTEGER_DIGITS, "%d", integer);
	return int_string;
}

int main() {
	char s[1024];

    while(1) {
        read(1, s, 1024);
        s[strlen(s)-1] = '\0';
        rson_object *test = rson_deserialize(s);
        printf("%s\n", rson_serialize(test));
    }
}
