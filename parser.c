#include "value.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"

// Takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
void printTree(Value *tree);

Value *parse(Value *tokens) {
  Value *stack = makeNull();
  int openParens = 0;
  while (tokens->type != NULL_TYPE) {
    
    // reached a CLOSE_TYPE
    if (tokens->c.car->type == CLOSE_TYPE) {
      
      Value *tree = makeNull();
      bool foundOpenType = 0;
     
     // as long as stack has something and 
      while (stack->type != NULL_TYPE && foundOpenType == 0) {
        if (stack->c.car->type != OPEN_TYPE) {
        tree = cons(stack->c.car, tree); //add top of stack to bottom of tree
        stack = stack->c.cdr; //look deeper in stack
        } else if (stack->c.car->type == OPEN_TYPE) {
          
          foundOpenType = 1;
          openParens--;
          stack = stack->c.cdr;
        }
        
      }

      if (foundOpenType == 0) {
        printf("Syntax error: too many close parentheses\n");
        texit(0);
      }

      if (tree->type == NULL_TYPE) {
        Value *empty = makeNull();
        tree = cons(empty,tree);
      }
      
      //place that tree (cons_type) in the stack
      stack = cons(tree, stack);
      //increment tokens past the CLOSE_TYPE
      

      tokens = tokens->c.cdr;
      
    } else {
     
      // keep track of how many parens we open
      if (tokens->c.car->type == OPEN_TYPE) {
        openParens++;
      }
      stack = cons(tokens->c.car, stack);
      tokens = tokens->c.cdr;
    }
 // printf("remaining tokens:\n");
//  if (tokens->type == NULL_TYPE) {
 //   printf("tokens is null\n");
 // }
 // displayTokens(tokens);
 // printf("stack bewlo\n");
 // displayTokens(stack);
  }
  if (openParens < 0) {
    printf("Syntax error: too many close parentheses\n");
    texit(0);
  } else if(openParens > 0) {
    printf("Syntax error: not enough close parentheses\n");
    texit(0);
  } 
  
  Value *reversed = makeNull();
  while (stack->type != NULL_TYPE) {
    reversed = cons(stack->c.car, reversed);
    stack = stack->c.cdr;
  }
  
  return reversed;
}


// Prints the tree to the screen in a readable fashion. It should look just like
// Scheme code; use parentheses to indicate subtrees.
void printTree(Value *tree) {
  // puts("line 94 - - entering printTree");
  
  while (tree->type != NULL_TYPE) {
    if (tree->type != CONS_TYPE) {
      printf (". ");
      tree = cons(tree, makeNull());
    }
    //printf("entered whileloop 97 in printTree\n");
    switch (tree->c.car->type) {
      case INT_TYPE:
      //  puts("reached int_type");
        printf("%d ", tree->c.car->i);
        break;
      case STR_TYPE:
        printf("%s ", tree->c.car->s);
        break;
      case DOUBLE_TYPE:
        printf("%f ", tree->c.car->d);
        break;
      case SYMBOL_TYPE:
        printf("%s ", tree->c.car->s);
        break;
      case CONS_TYPE:
        if (tree->c.car->c.car->type != VOID_TYPE) {
          printf("(");
          // puts("line 124");
          printTree(tree->c.car);

          printf(") "); 
        }
        break;
      case NULL_TYPE:
      //  printf(") ");
        break;
      case BOOL_TYPE:
        printf("%s \n", tree->c.car->s);
        break;
      case VOID_TYPE:
        break;
      case CLOSURE_TYPE:
        printf("#<procedure>\n");
        break;
      case PRIMITIVE_TYPE:
        printf("REACHED PRIMITIVE_TYPE...\n");
        break;
      default:
        printf("reached default case..\n");
        break; 
    }

    //catch if c.cdr != CONS_TYPE
    // if (tree->c.cdr->type != CONS_TYPE && tree->c.cdr->type != NULL_TYPE) { 
  //     printf(" . ");
  //     //printTree(tree->c.cdr);
  //     //print tree->c.cdr........?
  // }
  //  printf("\n");
    tree = tree->c.cdr;
    
  }
  // puts("exiting printTree.");
}

