#include<stdio.h>

typedef enum{false, true} bool;

typedef struct Metadata_tag{
    int size;
    bool free;
    struct Metadata_tag* next;
    struct Metadata_tag* prev;
}Metadata;

#define FREE_SPACE_SIZE 40000
char free_space[FREE_SPACE_SIZE];

Metadata* freeListHead;
int InitialisedFlag = 1;

void Initialise(){
    freeListHead = (void* )free_space;
    freeListHead -> free = 1;
    freeListHead -> size = sizeof(free_space) - sizeof(Metadata);
    freeListHead -> next = NULL;
    freeListHead -> prev = NULL;
    printf("The status is size: %d, free: %d, %d \n", freeListHead -> size, freeListHead -> free, freeListHead -> next);
}

Metadata* divide(Metadata* curr, int required_size){
    Metadata* next = ((void* )curr) + required_size + sizeof(Metadata);
    next -> free = true;
    next -> size = curr -> size - required_size - sizeof(Metadata);
    next -> next = curr -> next;
    if(next -> next != NULL)
        next -> next -> prev = next;
    next -> prev = curr;
    curr -> size = required_size;
    curr -> next = next;
    return curr;
}

void* malloc_manual(int required_size){
    if(InitialisedFlag){
        Initialise();
        InitialisedFlag = 0;
    }
    Metadata* curr = freeListHead;
    void* retVal;
    int flag = 0;
    
    while(curr != NULL && flag == 0){
        if(curr -> free == true && curr -> size >= required_size && curr -> size <= required_size + sizeof(Metadata)){
            flag = 2;
        }
        else if(curr -> free == true && curr -> size >= sizeof(Metadata) + required_size){
            flag = 1;
        }
        else{
            curr = curr -> next;
        }
    }

    if(curr == NULL){
        printf("No free space for memory allocation \n");
        return NULL;
    }
    else{
        printf("%d \n", curr -> size);
        if(flag == 1){
            curr = divide(curr, required_size);
            printf("Divided and allocated space \n");
            curr -> free = false;
            curr++;
            retVal = (void*) curr;
            return retVal;
        }
        else if(flag == 2){
            curr -> free = false;
            printf("Allocated the space \n");
            curr++;
            retVal = (void*) curr;
            return retVal;
        }
    }  
}

void merge_blocks(Metadata* curr){
    //for checking for the block next to the freed block
    if(curr -> next != NULL && curr -> next -> free == true){
        Metadata* next = curr -> next;
        curr -> size += sizeof(Metadata) + next -> size;
        curr -> next = next -> next;
        if(next -> next != NULL){
            next -> next -> prev = curr;
        }
        printf("Merged  ");
    }

    //for checking for the block before the freed block
    if(curr -> prev != NULL && curr -> prev -> free == true){
        Metadata* prev = curr -> prev;
        prev -> size += curr -> size + sizeof(Metadata);
        prev -> next = curr -> next;
        if(curr -> next != NULL){
            curr -> next -> prev = prev;
        }
        printf("Merged \n");
    }

    return ;
}

void free_manual(void* loc){
    if(loc < (void*)(free_space) || loc >= (void*)(free_space + FREE_SPACE_SIZE)){
        printf("Out of bounds \n");
    }
    else{
        Metadata* curr = loc;
        curr--;
        curr -> free = true;
        printf("The memory is free now \n");
        merge_blocks(curr);
    }
}

int main(){
    //void* ptr;
    //free_manual(ptr);
    int* arr1 = (int *)malloc_manual(5*sizeof(int));

    for(int i=0; i<5; i++){
        scanf("%d", &arr1[i]);
    }

    for(int i=0; i<5; i++){
        printf("%d ", arr1[i]);
    }
    printf("\n");
    int* arr2 = (int *)malloc_manual(9988);
    int* arr3 = (int *)malloc_manual(9988);
    int* arr4 = (int *)malloc_manual(9968);
    
    free_manual(arr2);
    free_manual(arr4);
    free_manual(arr3);

    return 0;
}



