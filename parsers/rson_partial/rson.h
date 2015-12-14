#ifndef RSON_H
#define RSON_H

#define DEFAULT_SIZE 128
#define INTEGER_DIGITS 14

typedef enum {RSON_NONE, RSON_STR, RSON_LIST, RSON_DICT, RSON_INT} rson_type;

typedef struct {
	rson_type type;
	void *value;
} rson_object;

typedef struct {
	rson_object *key;
	rson_object *value;
} rson_dict_entry;

typedef struct {
	size_t size;
	rson_dict_entry **entries;
} rson_dict;

typedef struct {
	size_t size;
	rson_object **objects;
} rson_list;


void print_error(const char *error);

void rson_free_object(rson_object *object);
void rson_free_list(rson_list *list);
void rson_free_dict_entry(rson_dict_entry *dict_entry);
void rson_free_dict(rson_dict *dict);

rson_object *rson_create_object();
rson_object *rson_create_string(char *string);
rson_object *rson_create_integer(int integer);
rson_list *rson_create_list(size_t size);
rson_dict *rson_create_dict(size_t size);
rson_dict_entry *rson_create_dict_entry(rson_object *key, rson_object *value);

void rson_add_to_list(rson_list *list, rson_object *object);
void rson_add_to_dict(rson_dict *dict, rson_dict_entry *entry);

char *allocate_string(size_t size);
char *append_to_string(char *string, char *append);
char *append_char_to_string(char *string, char append);
char *int_to_string(int integer);

int main();
#endif
