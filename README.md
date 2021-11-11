# typeless-array
Typeless dynamic array implementation

## Usage details
```c
typedef struct ta_array_t {
    void *data;             //Pointer to data block
    size_t size;            //Size in bytes of occupied space
    size_t space;           //Size in bytes of available space
    size_t element_size;    //Size in bytes of a single element
} ta_array_t;
```
---
Required initialization procedure (sets everything to 0 & mallocs):
```c
void ta_array_init(ta_array_t *array, size_t element_size);
```
---
Clear procedure (frees allocated memory & sets everything to 0):
```c
void ta_array_clear(ta_array_t *array);
```
---
Push procedure (appends ***elements*** array that has ***count*** elements):
```c
void ta_array_push(ta_array_t *array, ta_uint32 count, void *elements);
```
---
Pop procedure (erases the last ***count*** elements):
```c
void ta_array_pop(ta_array_t *array, ta_uint32 count);
```
---
Insert procedure (inserts the ***elements*** array that has ***count*** elements at the index ***pos***):
```c
void ta_array_insert(ta_array_t *array, ta_uint32 pos, ta_uint32 count, void *elements);
```
>(note: insertion can happen out of the array bounds, eg: inserting (5,6) in the array (1,2,3,4) at index 6 => (1,2,3,4,0,0,5,6)
---
Erase procedure (erases ***count*** elements starting at index ***pos***):
```c
void ta_array_erase(ta_array_t *array, ta_uint32 pos, ta_uint32 count);
```
