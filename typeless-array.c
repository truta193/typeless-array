//Typeless vector structure implementation
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

//TESTING
int main() {
    ArrayT a;
    float test[3] = {5.045f, 2.56f, 1.005f};
    float t6 = 6.0f;
    float t5 = 5.0f;
    float t4 = 4.0f;
    float t3 = 3.0f;
    float t2 = 2.0f;
    float t1 = 1.0f;
    ArrayInit(&a, sizeof(float));
    ArrayPush(&a, (void*)&t1, 1);
    ArrayPush(&a, (void*)&t2, 1);
    ArrayPush(&a, (void*)&t3, 1);
    ArrayPush(&a, (void*)&t4, 1);
    ArrayPush(&a, (void*)&t5, 1);
    ArrayPush(&a, (void*)&t6, 1);
    ArrayInsert(&a, test, 3, 4);
    ArrayRemove(&a, 1, 2);
    ArrayRemove(&a, 0, 2);
    ArrayPop(&a, 3);
    printf("len %u\n", a.length);
    for (int i = 0; i < a.length; i++) printf("%f\n", arrayRetrieve(&a, float, i));
    ArrayCleanup(&a);
    return 0;
};
