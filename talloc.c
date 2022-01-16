#include <stdlib.h>
#include <stdio.h>
#include "value.h"

struct Node{
  void *item; // = (Value*)malloc(sizeof(Value));
  struct Node *next; 
};

typedef struct Node Node;

Node *insert_front(Node *head, void *item) {
  Node *newNode = malloc(sizeof(Node));
  newNode->item = item;
  newNode->next = head;
  return newNode;
}

// void display(Node *head){
//   puts("(");
//   while(head != NULL){
//     printf(" %d,", head->item);
//   }
//   puts(")");
// }

Node *ACTIVE_LIST = NULL; 

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size) { 
  void *alloc = malloc(size);
  //malloc?
  ACTIVE_LIST = insert_front(ACTIVE_LIST, alloc);
  //save pointer to list
  return alloc;
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree() {
  while (ACTIVE_LIST != NULL) {
    Node *temp = ACTIVE_LIST;
    ACTIVE_LIST = ACTIVE_LIST->next;
    free(temp->item);
    free(temp);
    //printf("freed item...\n");
  }
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status) {
  tfree();
  exit(status);
}

/*int main(){
  int* p1 = talloc(sizeof(int)*2);
  p1[0] = 1;
  p1[1] = 2;
  int* p2 = talloc(sizeof(int)*2);
  p2[0] = 3;
  p2[1] = 4;

  display(ACTIVE_LIST);

  tfree();
} */