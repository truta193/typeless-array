#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef enum {false, true} bool;

typedef struct ArrayT {
    uint8 *data;
    uint32 length;
    uint32 allocLength;
    uint32 elementSize;
} ArrayT;

void ArrayInit(ArrayT *array, uint32 elementSize);
bool ArrayResize(ArrayT *array, uint32 newsize);
bool ArrayPush(ArrayT *array, void *data, uint32 length);
bool ArrayInsert(ArrayT *array, void *data, uint32 length, uint32 index);
bool ArrayPop(ArrayT *array, uint32 length);
bool ArrayRemove(ArrayT *array, uint32 index, uint32 length);
void ArrayCleanup(ArrayT *array);

#define arrayDataLength(array, length) ((array)->elementSize * (length))
#define arrayDataPos(array, index) ((array)->data + arrayDataLength((array), (index)))
#define arrayRetrieve(array, type, index) (((type*) (void*) (array)->data)[(index)])

void ArrayInit(ArrayT *array, uint32 elementSize) {
    array->data = NULL;
    array->length = 0;
    array->allocLength = 0;
    array->elementSize = elementSize;
};

bool ArrayResize(ArrayT *array, uint32 newLength) {
    if (newLength > array->allocLength) {
        void *buffer = NULL;
        buffer = realloc(array->data, (newLength + (array->allocLength >> 1u))*arrayDataLength(array, 1));
        if (buffer) {
            array->allocLength = newLength + (array->allocLength >> 1u);
            array->data = (uint8*) buffer;
        } else return false;
    };
    return true;
};

bool ArrayPush(ArrayT *array, void *data, uint32 length) {
    if (length >=0) { //Useless check, just keeping the same function type for prettiness
        ArrayResize(array, array->length + length);
        memcpy(arrayDataPos(array, array->length), data, arrayDataLength(array, length));
        array->length += length;
        return true;
    };
    return false;
};

bool ArrayInsert(ArrayT *array, void *data, uint32 length, uint32 index) {
    if (index <= array->length) {
        ArrayResize(array, array->length + length);
        memcpy(arrayDataPos(array, index+length), arrayDataPos(array, index), arrayDataLength(array, length));
        memcpy(arrayDataPos(array, index), data, arrayDataLength(array, length));
        array->length += length;
        return true;
    };
    return false;
};

bool ArrayPop(ArrayT *array, uint32 length) {
    if (length <= array->length) {
        array->length -= length;
        return true;
    };
    return false;
};

bool ArrayRemove(ArrayT *array, uint32 index, uint32 length) {
    if (index + length > array->length) return false;
    memcpy(arrayDataPos(array, index), arrayDataPos(array, index+length), arrayDataLength(array, array->length - length));
    array->length -= length;
    return true;
};

void ArrayCleanup(ArrayT *array) {
    array->length = 0;
    array->elementSize = 0;
    array->allocLength = 0;
    free((void*)array->data);
    array->data = NULL;
};