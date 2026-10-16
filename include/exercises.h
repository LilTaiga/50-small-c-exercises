#ifndef LEARN_C_EXERCISES_H
#define LEARN_C_EXERCISES_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int my_isdigit(int c);
int my_isalpha(int c);
int my_isalnum(int c);
int my_isspace(int c);
int my_toupper(int c);
int my_tolower(int c);
int my_abs(int value);
int clamp_int(int value, int minimum, int maximum);
void swap_int(int *a, int *b);
int array_sum(const int *values, size_t count);
size_t my_strlen(const char *s);
char *my_strcpy(char *destination, const char *source);
int my_strcmp(const char *a, const char *b);
char *my_strchr(const char *s, int c);
char *my_strrchr(const char *s, int c);
char *my_strcat(char *destination, const char *source);
int my_strncmp(const char *a, const char *b, size_t count);
char *my_strncpy(char *destination, const char *source, size_t count);
char *my_strncat(char *destination, const char *source, size_t count);
void reverse_string(char *s);
void *my_memset(void *destination, int value, size_t count);
void *my_memcpy(void *destination, const void *source, size_t count);
void *my_memmove(void *destination, const void *source, size_t count);
int my_memcmp(const void *a, const void *b, size_t count);
void *my_memchr(const void *memory, int value, size_t count);
char *my_strstr(const char *haystack, const char *needle);
size_t my_strspn(const char *s, const char *accepted);
size_t my_strcspn(const char *s, const char *rejected);
char *my_strpbrk(const char *s, const char *accepted);
char *next_token(char *s, const char *delimiters, char **state);
int my_atoi(const char *s);
typedef enum { PARSE_OK, PARSE_INVALID, PARSE_OVERFLOW } ParseResult;
ParseResult parse_int(const char *s, int *result);
int int_to_string(int value, char *destination, size_t capacity);
ParseResult parse_uint_base(const char *s, unsigned base, unsigned *result);
char *my_strdup(const char *s);
void *my_calloc(size_t count, size_t size);
int *array_clone(const int *values, size_t count);
char *string_slice(const char *s, size_t start, size_t length);
char *string_join(const char *const *strings, size_t count, const char *separator);
char **string_split(const char *s, char delimiter, size_t *count);
void string_split_free(char **strings, size_t count);
unsigned bit_count(unsigned value);
uint32_t read_u32_le(const unsigned char bytes[4]);
void write_u32_le(unsigned char bytes[4], uint32_t value);
void array_sort(int *values, size_t count);
void *my_bsearch(const void *key, const void *base, size_t count, size_t size,
                 int (*compare)(const void *, const void *));
typedef struct { int *data; size_t length; size_t capacity; } IntVector;
int vector_push(IntVector *vector, int value);
typedef struct { unsigned char *data; size_t length; size_t capacity; } ByteBuffer;
int buffer_append(ByteBuffer *buffer, const void *data, size_t count);
typedef struct IntNode { int value; struct IntNode *next; } IntNode;
int list_push_back(IntNode **head, int value);
void list_free(IntNode *head);
size_t list_remove_if(IntNode **head, int (*predicate)(int));
/* read_line: 1 = line, 0 = EOF, -1 = error; see exercise 50's contract. */
int read_line(FILE *stream, char **line, size_t *length);

#endif
