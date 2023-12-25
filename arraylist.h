
struct arraylist {
    int size;
    struct extensionCount extensions[1000];
};

void appendToArray(struct arraylist *array, struct extensionCount extension) {
    array->extensions[array->size] = extension;
    array->size++;
}

void extensionTypes(char extensionName[], struct arraylist *array) {
    int flag = 0;
    for (int i = 0; i < array->size; i++) {
        if (strcmp(array->extensions[i].extension, extensionName) == 0) {
            array->extensions[i].count++;
            flag = 1;
        }
    }
    if(flag == 0){
        struct extensionCount ex;
        strcpy(ex.extension, extensionName);
        ex.count = ex.count + 1;
        appendToArray(array, ex);
    }
}