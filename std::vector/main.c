#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

struct vector {
    size_t capacity;
    size_t size;
    int* values;
};

void vector_init(struct vector* v) {
    v->capacity = 2;
    v->size = 0;
    v->values = malloc(v->capacity * sizeof(int));
    if (!v->values) {
        v->capacity = 0;
    }
}

bool vector_push_back(struct vector* v, int x) {
    int mult = 2;
    size_t old_capacity = v->capacity;
    if (v->size >= v->capacity) {
        if ((SIZE_MAX / mult) < old_capacity) {
            return false;
        }
        v->capacity = old_capacity * mult;
        if ((SIZE_MAX / sizeof(int)) < v->capacity) {
            v->capacity = old_capacity;
            return false;
        }
        v->values = realloc(v->values, v->capacity * sizeof(int));
        if (!v->values) {
            v->capacity = old_capacity;
            return false;
        }
    }
    v->values[(v->size)++] = x;
    return true;
}

const int* vector_at(const struct vector* v, size_t index) {
    if (index >= v->size) {
        return NULL;
    }
    return &(v->values[index]);
}

void vector_free(struct vector* v) {
    free(v->values);
}

int main() {
    struct vector v;
    vector_init(&v);

    char cmd_buffer[10];
    int digit_buffer;
    size_t index_buffer;

    while (scanf("%s", cmd_buffer) != EOF) {
        if (strcmp(cmd_buffer, "push_back") == 0) {
            scanf("%d", &digit_buffer);
            vector_push_back(&v, digit_buffer);
        } else if (strcmp(cmd_buffer, "at") == 0) {
            scanf("%zu", &index_buffer);
            const int* value = vector_at(&v, index_buffer);
            if (value == NULL) {
                printf("out_of_range\n");
            } else {
                printf("%d\n", *value);
            }
        } else {
            break;
        }
    }

    vector_free(&v);

    return 0;
}