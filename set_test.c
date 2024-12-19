#include <stdio.h>
#include <assert.h>
#include "set.h"

#define TEST_SIZE 10
int main(void) {

    // Testing set with numeric types (1 duplicate element)
    int data[TEST_SIZE] = {55642, 68973, 60545, 3931, 41196, 68973, 74737, 74946, 86307, 59230};
    set* numeric_set = init_set("numeric", sizeof(int));
    
    // Pushing elements and checking contains()
    for (int i = 0; i < TEST_SIZE; i++) {
        push(numeric_set, (void*)&data[i]);
        if (!contains(numeric_set, (void*)&data[i])) {
            fprintf(stderr, "Contains failed on element %d\n", data[i]);
            exit(1);
        }
    }   
    // Checking length
    if (numeric_set->len != TEST_SIZE - 1) {
        fprintf(stderr, "Not all elements were inserted");
    }

    set* second_numeric_set = init_set("numeric", sizeof(int));
    int data2[TEST_SIZE] = {75792, 21238, 55642, 17188, 35791, 98879, 55642, 70484, 63273, 72603}; // 2 elements are the same as data1
    for (int i = 0; i < TEST_SIZE; i++) {
        push(second_numeric_set, (void*)&data2[i]);
    }
   
    // Testing union using contains() and length checking
    set_union(numeric_set, second_numeric_set);
    for (int i = 0; i < TEST_SIZE; i++) {
        if (!contains(numeric_set, (void*)&data2[i])) {
            fprintf(stderr, "Union failed");
            exit(1);
        }
    }
    if (numeric_set->len != (TEST_SIZE - 1) + (TEST_SIZE - 2)) {
        printf("%d\n", numeric_set->len);
        fprintf(stderr, "Union failed");
        exit(1);
    }

    // Testing pop() with length checking
    for (int i = 0; i < 4; i++) {
        pop(numeric_set, (void*)&data[i]);
    }
    if (numeric_set->len != (TEST_SIZE - 1) + (TEST_SIZE - 2) - 4) {
        fprintf(stderr, "Elements were not popped properly");
    }

    // Use $ valgrind --leak-check=full --tool=memcheck <executable> 
    free_set(numeric_set);
    free_set(second_numeric_set);
    
}