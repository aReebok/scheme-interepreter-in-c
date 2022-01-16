#include "linkedlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "talloc.h"

// Create a new NULL_TYPE value node.
Value *makeNull() {
  /* returns a Value* with type NULL_TYPE */
  Value* null_type = (Value*)talloc(sizeof(Value));
  null_type->type = NULL_TYPE;
  return null_type;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr) { 
  Value * cons_type =  (Value*) talloc(sizeof(Value));
  cons_type->type = CONS_TYPE; 
  /* put newCar in car of cons_type and newCdr in cdr of CONS_TYPE */
  cons_type->c.car = newCar; 
  cons_type->c.cdr = newCdr;

  return cons_type;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list) {
  Value *curr = list;
  //printf("\n(");
  printf("\n");

  while(curr->type != NULL_TYPE){
    switch (curr->c.car->type) {
      case INT_TYPE:
        printf(" %d", curr->c.car->i);
        break;
      case DOUBLE_TYPE:
        printf(" %f", curr->c.car->d);
        break;
      case STR_TYPE:
        printf(" %s", curr->c.car->s);
        break;
      case CONS_TYPE:
        printf("CONS_TYPE\n"); 
        break;
      case NULL_TYPE:
        printf("NULL_TYPE! NOTHING TO PRINT\n");
        //printf(")");
        break;
      case OPEN_TYPE:
        printf("(:open\n");
        break;
      case CLOSE_TYPE:
        printf("):close\n");
        break;
      case SYMBOL_TYPE:
        printf(" %s", curr->c.car->s);
        break;
      default:
        printf("ENTERED DEFAULT\n");
        break;

    }
    curr = curr->c.cdr; /* goes to the next in list ..*/
  }
  //printf(" )");
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list) { 
  //Value *prev = makeNull();
  Value *curr = list; /* pointing at the head of the list */
  Value *rev_list = makeNull();
  while (curr->type != NULL_TYPE) {
    Value *temp_val = (Value*) talloc(sizeof(Value));  
    temp_val -> type = curr -> c.car -> type;
    // display(curr);
    // display(rev_list);
    char * temp;
    switch (curr->c.car->type) { /* assign the correct type value... */
      case INT_TYPE:
        temp_val->i = curr->c.car->i;
        break;
      case DOUBLE_TYPE:
        temp_val->d = curr->c.car->d;
        break;
      case STR_TYPE:
        temp = (char *) talloc(10);
        //strcpy(temp, ((curr-> c.car) -> s));
        temp_val -> s = curr->c.car->s;
        // temp_val->s = curr->c.car->s;
        break;
      case CONS_TYPE: // shoudl not be the case in this assigment..
        break;
      case NULL_TYPE: // do nothing
         break;
      case BOOL_TYPE:
        temp = (char *) talloc(10);
     //   strcpy(temp, ((curr-> c.car) -> s));
        temp_val -> s = curr->c.car->s;
        break;
      case SYMBOL_TYPE:
        temp = (char *) talloc(10);
        //strcpy(temp, ((curr-> c.car) -> s));
        temp_val -> s = curr->c.car->s;
        break;
      default:
        //printf("Default case reached in switch method. \n");
        break;
    }
    rev_list = cons(temp_val, rev_list);
    /* increment previous and curr */
    curr = curr->c.cdr;
  }
  return rev_list;
} 

// Frees up all memory directly or indirectly referred to by list. This includes strings.
//
// FAQ: What if a string being pointed to is a string literal? That throws an
// error when freeing.
//
// ANS: Don't put a string literal into the list in the first place. All strings
// added to this list should be able to be freed by the cleanup function.
//
// FAQ: What if there are nested lists inside that list?
//
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
/*void cleanup(Value *list) {
  if(list->type == NULL_TYPE) {
    free(list);
  }
  else {
    cleanup(cdr(list));
    if(car(list) -> type == STR_TYPE) {
      free(car(list)->s);
    }
    free(car(list));
    free(list);
  }
}*/

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list) { 
  assert(list != NULL && "Error (length): input *value is NULL");
  return list->c.car; 
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list) { 
  assert(list != NULL && "Error (length): input *value is NULL");
  return list->c.cdr; 
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value) { 
  assert(value != NULL && "Error (length): input *value is NULL");
  if (value->type == NULL_TYPE) { return 1; }
  return 0;
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value) { 
 // assert(value != NULL && "Error (length): input *value is NULL");
  int len = 0;
  while(value->type != NULL_TYPE) {
    if(value->c.car->type != NULL_TYPE) {
      len++;
    }
    value = value->c.cdr;
  }
  return len;
}

