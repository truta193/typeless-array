#ifndef TA_DYNAMIC_ARRAY
#define TA_DYNAIMIC_ARRAY

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

typedef unsigned char ta_uint8;
typedef unsigned int ta_uint32;
typedef signed int ta_int32;
#define TA_EMPTY_START_SLOTS 2

typedef struct ta_array_t {
    void *data;             //Pointer to data block
    size_t size;            //Size in bytes of occupied space
    size_t space;           //Size in bytes of available space
    size_t element_size;    //Size in bytes of a single element
} ta_array_t;

void ta_array_init(ta_array_t *array, size_t element_size);
void ta_array_clear(ta_array_t *array);
void ta_array_resize(ta_array_t *array, size_t new_size);
void ta_array_push(ta_array_t *array, ta_uint32 count, void *elements);
void ta_array_pop(ta_array_t *array, ta_uint32 count);
void ta_array_insert(ta_array_t *array, ta_uint32 pos, ta_uint32 count, void *elements);
void ta_array_erase(ta_array_t *array, ta_uint32 pos, ta_uint32 count);

//Initialized with TA_EMPTY_START_SLOTS empty slots, cuts down a few reallocs and doesn't occupy much memory
void ta_array_init(ta_array_t *array, size_t element_size) {
    array->data = malloc(TA_EMPTY_START_SLOTS * sizeof(element_size));
    assert(array->data);
    array->size = 0;
    array->space = TA_EMPTY_START_SLOTS * sizeof(element_size);
    array->element_size = element_size;
};

void ta_array_clear(ta_array_t *array) {
    free(array->data);
    memset((void*)array, 0, sizeof(ta_array_t));
};

//Grows by the powers of 2, will optimize for memory usage if necessary in the future
//NOTE: Optimal growth rate is phi (1.618) ~= 1.5
void ta_array_resize(ta_array_t *array, size_t new_size) {
    if (new_size <= array->space) return;
    size_t sz_size = sizeof(size_t) * 8; //Size in bits of size_t

    //Calculates next power of 2 after new_size
    new_size--;
    for (size_t i = 1; i < sz_size; i *= 2) new_size |= new_size >> i;
    new_size++;

    void *buffer = realloc(array->data, new_size);
    assert(buffer);
    array->space = new_size;
    array->data = buffer;
};

void ta_array_push(ta_array_t *array, ta_uint32 count, void *elements) {
    size_t needed_size = array->size + array->element_size * (size_t)count;
    //Resize if needed
    ta_array_resize(array, needed_size);
    //Append element list to data array
    memcpy(array->data + array->size, elements, array->element_size * (size_t)count);
    array->size += array->element_size * (size_t)count;
};

void ta_array_pop(ta_array_t *array, ta_uint32 count) {
    array->size -= array->element_size * (size_t)count;
};

void ta_array_insert(ta_array_t *array, ta_uint32 pos, ta_uint32 count, void *elements) {
    //REVIEW: Could limit insertions to happen to array bound by array->size instead of allowing it anywhere
    size_t insertion_size = array->element_size * (size_t)count; //Number of bytes needed to be inserted
    size_t offset = array->element_size * (size_t)pos; //Offset from the start of the array
    ta_int32 padding = pos * array->element_size - array->size; //If value is negative, insertion is either connected or in the array
    ta_int32 move_size = array->size - offset; //If value is negative, insertion does not happen in the array so there's no need to move stuff around
    size_t needed_size = array->size + insertion_size + (padding > 0 ? padding : 0); //New required size for the array
    //Resize array if needed and move data around as required
    ta_array_resize(array, needed_size);
    memcpy(array->data + offset + insertion_size, array->data + offset, move_size > 0 ? move_size : 0);
    memcpy(array->data + offset, elements, insertion_size);
    array->size = needed_size; 
};

void ta_array_erase(ta_array_t *array, ta_uint32 pos, ta_uint32 count) {
    size_t deletion_size = array->element_size * (size_t)count; //Number of bytes needed to be removed
    size_t offset = array->element_size * (size_t)pos; //Offset from the start of the array
    size_t move_size = array->size - offset - deletion_size; //Number of bytes to be moved
    assert(move_size >= 0);
    memcpy(array->data + offset, array->data + offset + deletion_size, move_size);
    array->size -= deletion_size;
};
#endif
