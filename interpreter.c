#include "value.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

Value *primitivePlus(Value *args) {
  int total = 0;
  double totalD = 0;
  bool isDouble = 0;
  Value *sum = makeNull();
  while (1) {
    if (isNull(args)) {
      if (isDouble == 0) {
        sum->type = INT_TYPE;
        sum->i = total;
        return sum;
      } else {
        sum->type = DOUBLE_TYPE;
        sum->d = totalD + total;
        return sum;
      }
    } else if (car(args)->type == INT_TYPE) {
        total = total + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        if(isDouble == 1) {
          // already a double 
          totalD = totalD + car(args)->d;
        } else {
          // double = previous total + current arg
          isDouble = 1;
          totalD = car(args)->d;
        }  
        //add to total
    } else {
      // eval error?
      puts("Evaluation error: args of Plus may not be either int or a double.");
      evaluationError();
    }
    args = cdr(args);
  }
}

Value *primitiveMinus(Value *args) {
  //printf("ENtered primitive minus\n");
  int total = 0;
  double totalD = 0;
  bool isDouble = 0;
  Value *sum = makeNull();

  if (length(args) == 0) {
    puts("Evaluation error: wrong number of arguments to minus");
    evaluationError();
  } else if (length(args) == 1) {
    if(car(args)->type == INT_TYPE) {
      total = total - car(args)->i;
      sum->type = INT_TYPE;
      sum->i = total;
      return sum;
    } else if (car(args)->type == DOUBLE_TYPE) {
      totalD = totalD - car(args)->d;
      sum->type = DOUBLE_TYPE;
      sum->d = totalD;
      isDouble = 1;
      return sum;
    } else {
      puts("Evaluation error: args of Minus may not be either int or double.");
      evaluationError();
    }
  //length of args is 2 or more. 
  } else if (length(args) >= 1) {
    //first term is positive
    if(car(args)->type == INT_TYPE) {
      total = total + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      isDouble = 1;
      totalD = totalD + car(args)->d;
    } else {
      puts("Evaluation error: first arg for minus is not int or double");
      evaluationError();
    }
    args = cdr(args);

    while(1) {
      if (isNull(args)) {
        if (isDouble == 0) {
        sum->type = INT_TYPE;
        sum->i = total;
        return sum;
      } else {
        sum->type = DOUBLE_TYPE;
        sum->d = totalD + total;
        return sum;
      }
      } else if (car(args)->type == INT_TYPE) {
        total = total - car(args)->i;
      } else if (car(args)->type == DOUBLE_TYPE) {
          isDouble = 1;
          totalD = totalD - car(args)->d;
      } else {
        puts("Evaluation error: args of Plus may not be either int or a double.");
        evaluationError();
      }
      args = cdr(args);
    }

  }
  return sum;
}

Value *primitiveLessThan(Value *args) {
  Value *boolLess = makeNull();
  boolLess->type = BOOL_TYPE;
  boolLess->s = "#t";
  Value *currValue = makeNull();

  if(!isNull(args)) {
    if (car(args)->type == INT_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = 0.00 + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = car(args)->d;
    } else {
      puts("Evaluation error: wrong type argument in position 1");
      evaluationError();
    }
    args = cdr(args);
  }

  while(1) {
    if(isNull(args)) {
      return boolLess;
    } else if (car(args)->type == INT_TYPE) {
      if(car(args)->i <= currValue->d) {
        boolLess->s = "#f";
        return boolLess;
      }
      //update currValue so comparing on later iterations is to the left term 
      currValue->d = 0.00 + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      if(car(args)->d <= currValue->d) {
        boolLess->s = "#f";
        return boolLess;
      }
      currValue->d = car(args)->d;
    } else {
      puts("Evaluation error: wrong type argument to less than");
      evaluationError();
    }
    args = cdr(args);
  }

}

Value *primitiveGreaterThan(Value *args) {
  Value *boolGreat = makeNull();
  boolGreat->type = BOOL_TYPE;
  boolGreat->s = "#t";
  Value *currValue = makeNull();

  if(!isNull(args)) {
    if (car(args)->type == INT_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = 0.00 + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = car(args)->d;
    } else {
      puts("Evaluation error: wrong type argument in position 1");
      evaluationError();
    }
    args = cdr(args);
  }

  while(1) {
    if(isNull(args)) {
      return boolGreat;
    } else if (car(args)->type == INT_TYPE) {
      if(car(args)->i >= currValue->d) {
        boolGreat->s = "#f";
        return boolGreat;
      }
      //update currValue so comparing on later iterations is to the left term 
      currValue->d = 0.00 + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      if(car(args)->d >= currValue->d) {
        boolGreat->s = "#f";
        return boolGreat;
      }
      currValue->d = car(args)->d;
    } else {
      puts("Evaluation error: wrong type argument to less than");
      evaluationError();
    }
    args = cdr(args);
  }
}

Value *primitiveEqual(Value *args) {
  Value *boolEqual = makeNull();
  boolEqual->type = BOOL_TYPE;
  boolEqual->s = "#t";
  Value *currValue = makeNull();

  if(!isNull(args)) {
    if (car(args)->type == INT_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = 0.00 + car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      currValue->type = DOUBLE_TYPE;
      currValue->d = car(args)->d;
    } else {
      puts("Evaluation error: wrong type argument in position 1");
      evaluationError();
    }
    args = cdr(args);
  }

  while(1) {
    if(isNull(args)) {
      return boolEqual;
    } else if (car(args)->type == INT_TYPE) {
      if (car(args)->i != currValue->d) {
        boolEqual->s = "#f";
        return boolEqual;
      }
    } else if (car(args)->type == DOUBLE_TYPE) {
      if (car(args)->d != currValue->d){
        boolEqual->s = "#f";
        return boolEqual;
      } 
    } else {
      puts("Evaluation error: wrong type argument in equal");
      evaluationError();
    }
    args = cdr(args);
  }
}

Value *primitiveCar(Value *args) {
  if (length(args) != 1) {
    puts("Evaluation error: wrong number of arguments to car.");
    evaluationError();
  }
  // check if passed in arg is a list:
  if(car(args)->type != CONS_TYPE) {
    puts("Evaluation error: passed in a nonpair argument to car.");
    evaluationError();
  }
  // check to make sure there is atleast one elemnt in args list.
  if(car(car(args))->type == NULL_TYPE) {
    puts("Evaluation error: wrong type argument, expecting pair");
    evaluationError();
  }
  return car(car(args));
} 

Value *primitiveCdr(Value *args) {
  if (length(args) != 1) {
    puts("Evaluation error: wrong number of arguments to cdr.");
    evaluationError();
  }
  if(car(args)->type != CONS_TYPE) {
    puts("Evaluation error: passed in a nonpair argument to cdr.");
    evaluationError();
  }
  if(car(car(args))->type == NULL_TYPE) {
    puts("Evaluation error: wrong type argument, expecting pair");
    evaluationError();
  }
  if (cdr(car(args))->type == NULL_TYPE) {
    return cons(cdr(car(args)), makeNull());
  } 
  return cdr(car(args));
}

Value *primitiveNull(Value *args){
  if (length(args) != 1) {
    puts("Evaluation error: wrong number of arguments to Null.");
    evaluationError();
  }
  Value *return_bool = (Value *)talloc(sizeof(Value));
    return_bool->type = BOOL_TYPE;
    return_bool->s = (char *)talloc(3); 
    return_bool->s = "#f"; 
  if(car(args)->type != CONS_TYPE) {
    return return_bool;
  } else {
     if (length(car(args)) == 0) {
        return_bool->s = "#t";
        return return_bool;
      }
  }
  return return_bool; 
}

Value *primitiveCons(Value *args) {
  if (length(args) != 2) {
    puts("Evaluation error: wrong number of arguments to cons");
    evaluationError();
  }
  return cons(car(args), car(cdr(args)));
}

Value *primitiveMultiply(Value *args) {
  bool isDouble = 0;
  int total = 1;
  double totalD = 1.0;
  Value *product = makeNull();
  while(1) {
    if(isNull(args)) {
      if (isDouble == 0) {
        product->type = INT_TYPE;
        product->i = total;
      } else {
        product->type = DOUBLE_TYPE;
        product->d = totalD * total;
      }
      return product;
    } else if (car(args)->type == INT_TYPE) {
      total = total * car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
      isDouble = 1;
      totalD = totalD * car(args)->d;
    } else {
      puts("Evaluation error: args of multiply may not be either int or double");
      evaluationError();
    }
    args = cdr(args);
  }
}

Value *primitiveDivide(Value *args) {
  Value *quotient = makeNull();
  int total = 1;
  double totalD = 1.0;

  if (length(args) != 2) {
    puts("Evaluation error: Wrong number of arguments to /");
    evaluationError();
  }
   
  if (car(args)->type == DOUBLE_TYPE 
      && car(cdr(args))->type == DOUBLE_TYPE) {
        quotient->type = DOUBLE_TYPE;
        quotient->d = car(args)->d / car(cdr(args))->d;
  } else if (car(args)->type == DOUBLE_TYPE 
      && car(cdr(args))->type == INT_TYPE) {
        quotient->type = DOUBLE_TYPE;
        quotient->d = car(args)->d / car(cdr(args))->i;
  } else if (car(args)->type == INT_TYPE 
      && car(cdr(args))->type == DOUBLE_TYPE) {
        quotient->type = DOUBLE_TYPE;
        quotient->d = car(args)->i / car(cdr(args))->d;
  } else {
    if ((car(args)->i % car(cdr(args))->i) != 0) {
      quotient->type = DOUBLE_TYPE;
      quotient->d = (float)car(args)->i / car(cdr(args))->i;
    } else {
      quotient->type = INT_TYPE;
      quotient->i = car(args)->i / car(cdr(args))->i;
    }
  }
  return quotient;
}

Value *primitiveModulo(Value *args) {
  Value *mod = makeNull();
  int intA;
  int intB;
  if(length(args) != 2) {
    puts("Evaluation error: wrong number of input to modulo");
    evaluationError();
  }
  //first term 
  if(car(args)->type == INT_TYPE) {
    intA = car(args)->i;
  } else {
    puts("Evaluation error: wrong type argument in position 1 (not int)");
  }
  //second term
   if(car(cdr(args))->type == INT_TYPE) {
    intB = car(cdr(args))->i;
  } else {
    puts("Evaluation error: wrong type argument in position 2 (not int)");
  }
  mod->type = INT_TYPE;
  mod->i = intA % intB;
  return mod;
}

void bind(char *name, Value *(*function)(Value *), Frame *frame) {  
  Value *result = (Value*)talloc(sizeof(Value));
  result->pf = function;
  result->type = PRIMITIVE_TYPE;
  Value *nameVal = (Value*)talloc(sizeof(Value));
  nameVal->type = SYMBOL_TYPE;
  nameVal->s = name;
  frame->bindings = cons(cons(nameVal, cons(result, makeNull())), frame->bindings);
}


void interpret(Value *tree) { 
  
  Frame *GLOBAL_FRAME = (Frame*)talloc(sizeof(Frame));
  GLOBAL_FRAME->bindings = makeNull();
  GLOBAL_FRAME->parent = NULL;
  Value *evaluated = makeNull();
  bind("=", primitiveEqual, GLOBAL_FRAME);
  bind("<", primitiveLessThan, GLOBAL_FRAME);
  bind(">", primitiveGreaterThan, GLOBAL_FRAME);
  bind("+", primitivePlus, GLOBAL_FRAME);
  bind("-", primitiveMinus, GLOBAL_FRAME);
  bind("car", primitiveCar, GLOBAL_FRAME);  
  bind("cdr", primitiveCdr, GLOBAL_FRAME);
  bind("null?", primitiveNull, GLOBAL_FRAME);
  bind("cons", primitiveCons, GLOBAL_FRAME);
  bind("*", primitiveMultiply, GLOBAL_FRAME);
  bind("/", primitiveDivide, GLOBAL_FRAME);
  bind("modulo", primitiveModulo, GLOBAL_FRAME);
  
  while(!isNull(tree)) {
    evaluated = cons(eval(car(tree), GLOBAL_FRAME), makeNull());
    tree = cdr(tree);
    printTree(evaluated);
  }
}

Value *eval(Value *tree, Frame *frame) {
 // printf("Begin eval --------------\n");
 // printTree(tree);
 // printf("\n-----------------------\n");
  switch (tree->type)  {
     case INT_TYPE: {

        return tree;
        break;
     }
     case BOOL_TYPE: {
        return tree;
        break;
     }  
     case DOUBLE_TYPE: {
       return tree;
       break;
     }
     case STR_TYPE: {
        return tree;
        break;
     }  
     case CLOSURE_TYPE: {
       return tree;
       break;
     }
     case SYMBOL_TYPE: {
      // printf("Here line 463\n");
       Value *temp = lookUpSymbol(tree, frame);
       if (isNull(temp)) {
         printf("Evaluation error: symbol '%s' not found.\n", tree->s);
         evaluationError();
       }
      return temp;
      break;
     } 

      
     case CONS_TYPE: {
        Value *first = car(tree);
        Value *args = cdr(tree);

        if (!strcmp(first->s,"if")) {
          return evalIf(args,frame);
        }
 
        if (!strcmp(first->s,"let")) {
          return evalLet(args, frame);
        }

        if(!strcmp(first->s, "let*")) {
          return evalLetStar(args, frame);
        }

        if(!strcmp(first->s, "letrec")) {
          return evalLetrec(args, frame);
        }

        if(!strcmp(first->s, "set!")) {
          return evalSet(args, frame);
        }

        if (!strcmp(first->s,"quote")) {
          return evalQuote(args, frame);
        }

        if (!strcmp(first->s,"define")) {
          return evalDefine(args, frame);
        }

        if (!strcmp(first->s,"lambda")) {
          return evalLambda(args, frame);
        }

        if (!strcmp(first->s, "and")) {
  
          return evalAnd(args, frame);
        }

        if (!strcmp(first->s, "or")) {
          return evalOr(args, frame);
        }

        if(!strcmp(first->s, "begin")) {
          return evalBegin(args, frame);
        }

        if(!strcmp(first->s, "cond")) {
          return evalCond(args, frame);
        }

         if (first->type == CONS_TYPE) {

          Value* tempArgs = makeNull();
          Value* args2 = args;
          while (!isNull(args2)) {
            tempArgs = cons(eval(car(args2),frame), tempArgs);
            args2 = cdr(args2);
          }
          //REverse tempArgs
          Value *reversedArgs = makeNull();
          while (tempArgs->type != NULL_TYPE) {
            reversedArgs = cons(car(tempArgs), reversedArgs);
            tempArgs = cdr(tempArgs);
          }
          //We should check if eval(first, frame) is a primitive or Symbol 
          Value *temp = eval(first, frame);
          if (temp->type == PRIMITIVE_TYPE) {
           // printf("primitive eval/apply\n");
            return temp->pf(reversedArgs);
          } else {

            return apply(temp, reversedArgs);
          }
      //     return apply(eval(first, frame), reversedArgs);
        }

        if (first->type == SYMBOL_TYPE) {
       //   printf("Looking up symbol in Eval, %s\n", first->s);
          //printf("This is where the symbol is eval'd\n");
          Value *temp = lookUpSymbol(first, frame);

          if (isNull(temp)) {
            printf("Evaluation error: symbol '%s' not found.\n", tree->s);
            evaluationError();
          }
            
          Value* tempArgs = makeNull();
          Value* argsDup = args;
          while (!isNull(argsDup)) {

            tempArgs = cons(eval(car(argsDup),frame), tempArgs);
            argsDup = cdr(argsDup);
          }
          //REverse tempArgs
          Value *reversedArgs = makeNull();
          while (tempArgs->type != NULL_TYPE) {
            reversedArgs = cons(car(tempArgs), reversedArgs);
            tempArgs = cdr(tempArgs);
          }

          if(temp->type == PRIMITIVE_TYPE) {
       //     printf("Primitive after evaluating? %s\n", temp->s);
            return temp->pf(reversedArgs);
          } 

        //    printf("using Appl")
          return apply(temp, reversedArgs); 
        } 

        else { 
           puts("Evaluation Error: not a recognized special form.");
           evaluationError();
        }
        break;
     } default: {
       printf("reached default\n");
     }
    }    
    return makeNull();
}

Value *evalIf(Value *args, Frame *frame) {

  if(eval(car(args), frame)->type == BOOL_TYPE && length(args) == 3) { 
    if (!strcmp(eval(car(args), frame)->s,"#t")) {
      return eval(car(cdr(args)), frame);
    } else {
      return eval(car(cdr(cdr(args))), frame);
    }
  } else  { 
    if (eval(car(args), frame)->type == BOOL_TYPE) {
      printf("Evaluation error: Bad form in IF\n");
    } else {
      printf("Evaluation error: no consequent following an if.\n");
    }
    evaluationError();
    return makeNull();
  }
}

Value *evalLet(Value *args, Frame *frame) {
 // printf("------ENtered evalLet\n");
  Frame *newFrame = (Frame *)talloc(sizeof(Frame));
  newFrame->parent = frame;
  newFrame->bindings = makeNull();

  /* bindings */
  Value *bind_args = car(args);
  
  while(!isNull(bind_args)){
    if(bind_args->type != CONS_TYPE) {
    puts("Evaluation error: Let expression's args not nested list.");
    evaluationError();
    }
    if (isNull(car(bind_args))) {
      bind_args = cdr(bind_args);
    } else if (car(bind_args)->type != CONS_TYPE) {
      puts("Evaluation error: binding not a pair ln 159\n");
      evaluationError();
    } else {
      //Must be CONS_TYPE
      if (length(car(bind_args)) !=2) {
        puts("Evaluation error: args length != 2\n");
        evaluationError();
      }
      if(car(car(bind_args))->type != SYMBOL_TYPE){
        puts("Evaluation error: first arg not a symbol\n");
        evaluationError();
      }
      /* make temporarry instance of current working frame */
   
      Frame *temp_frame = (Frame*) talloc(sizeof(Frame));
      temp_frame->bindings = newFrame->bindings;
      temp_frame->parent = NULL;

      /* check if bind symbol exists*/
      if(!isNull(lookUpSymbol(car(car(bind_args)), temp_frame))) {
        puts("Evaluation error: already bound symbol\n");
        evaluationError();
      }

  //  printf("Before creating binding in evalLet\n");
    Value * newBinding = cons(car(car(bind_args)), cons(eval(car(cdr(car(bind_args))), newFrame->parent), makeNull()));
    newFrame->bindings = cons(newBinding, newFrame->bindings);

    // Value* currBinding = car(bind_args);
    // printf("Have not eval'd the cdr of the binding yet\n");
    // currBinding->c.cdr = cons(eval(car(cdr(currBinding)), newFrame->parent), makeNull());
    // printf("\n made it past the eval of binding\n");
    // newFrame->bindings = cons(currBinding, newFrame->bindings);
    
    bind_args = cdr(bind_args);
    
    }
  }
  
 // printf("Completed making bindings in EvalLet\n");
  /* evaluating expressions */
  Value *exp_args = cdr(args);

  if(isNull(exp_args)) {
    printf("Evaluation error: expression args is empty \n");
    evaluationError();
  }

  while(!isNull(cdr(exp_args))) {
    eval(car(exp_args), newFrame);
    exp_args = cdr(exp_args);
  }
  //When broken, we reached last expression in body
  return eval(car(exp_args), newFrame);
}

Value *evalLetStar(Value *args, Frame *frame) {
  Frame *newFrame = (Frame *)talloc(sizeof(Frame));
  newFrame->parent = frame;
  newFrame->bindings = makeNull();

  /* bindings */
  //puts("entered let");
  Value *bind_args = car(args);
  
  while(!isNull(bind_args)){

    if(bind_args->type != CONS_TYPE) {
    puts("Evaluation error: Let expression's args not nested list.");
    evaluationError();
    }
    if (isNull(car(bind_args))) {
      bind_args = cdr(bind_args);
    } else if (car(bind_args)->type != CONS_TYPE) {
      puts("Evaluation error: binding not a pair ln 159\n");
      evaluationError();
    } else {
      //Must be CONS_TYPE
      if (length(car(bind_args)) !=2) {
        puts("Evaluation error: args length != 2\n");
        evaluationError();
      }
      if(car(car(bind_args))->type != SYMBOL_TYPE){
        puts("Evaluation error: first arg not a symbol\n");
        evaluationError();
      }

      // Frame *temp_frame = (Frame*) talloc(sizeof(Frame));
      // temp_frame->bindings = newFrame->bindings;
      // temp_frame->parent = NULL;

      // if(!isNull(lookUpSymbol(car(car(bind_args)), temp_frame))) {
      //   puts("Evaluation error: already bound symbol\n");
      //   evaluationError();
      // }
   // printf("parent's bindings (the ones we r checking\n");
   // printTree(newFrame->parent->bindings);
   // printf("\n");

    //printf("adding binding to symbol: %s ------\n", car(car(bind_args))->s);
    Value * newBinding = cons(car(car(bind_args)), cons(eval(car(cdr(car(bind_args))), newFrame->parent), makeNull()));

    newFrame->bindings = cons(newBinding, newFrame->bindings);
      // printf("Printing binding added to newFrame)\n");
      // printTree(newFrame->bindings);
      // printf("-----finished this binding\n");
    Frame *tempOldNewFrame = newFrame;
      // printf("^^^ temp frame's bindings\n");
      // printTree(tempOldNewFrame->bindings);
      // printf("^^^\n");
    newFrame = (Frame *)talloc(sizeof(Frame));
    newFrame->parent = tempOldNewFrame;
    newFrame->bindings = makeNull();
      // printf("newFrame->parent->bindings\n");
      // printTree(newFrame->parent->bindings);
      // puts("");
    // Value* currBinding = car(bind_args);
    // currBinding->c.cdr = cons(eval(car(cdr(currBinding)), newFrame), makeNull());
    // newFrame->bindings = cons(currBinding, newFrame->bindings);
     bind_args = cdr(bind_args);
    
    }
  }
  // printf("Bindings after we exit the assigning");
  // printTree(newFrame->bindings);
   /* evaluating expressions */
  Value *exp_args = cdr(args);

  if(isNull(exp_args)) {
    printf("Evaluation error: expression args is empty \n");
    evaluationError();
  }

  while(!isNull(cdr(exp_args))) {
    eval(car(exp_args), newFrame);
    exp_args = cdr(exp_args);
  }

  return eval(car(exp_args), newFrame);

}

Value *evalLetrec(Value *args, Frame *frame) {
  // puts("reached letrec");
    Frame *newFrame = (Frame *)talloc(sizeof(Frame));
  newFrame->parent = frame;
  newFrame->bindings = makeNull();

  /* bindings */
  //puts("entered let");
  Value *bind_args = car(args);
  Value *evaluatedArgs = makeNull();
  
  while(!isNull(bind_args)){
    // puts("entered whilke");
    if(bind_args->type != CONS_TYPE) {
    puts("Evaluation error: Let expression's args not nested list.");
    evaluationError();
    }
    if (isNull(car(bind_args))) {
      bind_args = cdr(bind_args);
    } else if (car(bind_args)->type != CONS_TYPE) {
      puts("Evaluation error: binding not a pair ln 159\n");
      evaluationError();
    } else {
      //Must be CONS_TYPE
      if (length(car(bind_args)) !=2) {
        puts("Evaluation error: args length != 2\n");
        evaluationError();
      }
      if(car(car(bind_args))->type != SYMBOL_TYPE){
        puts("Evaluation error: first arg not a symbol\n");
        evaluationError();
      }

      Frame *temp_frame = (Frame*) talloc(sizeof(Frame));
      temp_frame->bindings = newFrame->bindings;
      temp_frame->parent = NULL;

      // if(!isNull(lookUpSymbol(car(car(bind_args)), temp_frame))) {
      //   puts("Evaluation error: already bound symbol\n");
      //   evaluationError();
      // }
      /* first we want to eval e1...en in env' */
      Value* currBinding = car(bind_args);
    // currBinding->c.cdr = cons(eval(car(cdr(currBinding)), newFrame), makeNull());
    // newFrame->bindings = cons(currBinding, newFrame->bindings);
    //  printf("About to evaluate");
    //  printTree(currBinding);
    //  printf("\n");
      evaluatedArgs = cons(eval(car(cdr(currBinding)), newFrame), evaluatedArgs);
      bind_args = cdr(bind_args);
    }
     

  }
  //puts("reached line 779");

  Value *reversedArgs = makeNull();
  while (evaluatedArgs->type != NULL_TYPE) {
    reversedArgs = cons(car(evaluatedArgs), reversedArgs);
    evaluatedArgs = cdr(evaluatedArgs);
  }
  //puts("reached line 785");
  /* bind them in enironment env' */
  bind_args = car(args);
  while(!isNull(bind_args)){
    //Value* currBinding = car(bind_args);

    Value * newBinding = cons(car(car(bind_args)), cons(car(reversedArgs), makeNull()));
  //  printTree(newBinding);
    newFrame->bindings = cons(newBinding, newFrame->bindings);
    
    // currBinding->c.cdr = cons(car(reversedArgs), makeNull());
    // newFrame->bindings = cons(currBinding, newFrame->bindings);
    reversedArgs = cdr(reversedArgs);
    bind_args = cdr(bind_args);
  }

  /* evaluating expressions */
  Value *exp_args = cdr(args);

  if(isNull(exp_args)) {
    printf("Evaluation error: expression args is empty \n");
    evaluationError();
  }

  while(!isNull(cdr(exp_args))) {
    eval(car(exp_args), newFrame);
    exp_args = cdr(exp_args);
  }

  return eval(car(exp_args), newFrame);
}

Value *evalSet(Value *args, Frame *frame) {
 // printf("*******SETBANGEVAL*******\n");
  if(length(args) != 2 ) {
    puts("Evaluation error: wrong number of arguments to set!");
    evaluationError();
  }
  Frame *currentFrame = frame;
  while(currentFrame!=NULL) {
    Value *currentBindings = currentFrame->bindings;
    while(!isNull(currentBindings)) {
      if (!strcmp(car(args)->s, car(car(currentBindings))->s)) {
       // printf("Reassigning amt \n");
        currentBindings->c.car->c.cdr = cons(eval(car(cdr(args)), frame), makeNull());  
      }
      currentBindings = cdr(currentBindings);
    }
    currentFrame = currentFrame->parent;
  }
  // Value *returned = makeNull();
  // returned->type = VOID_TYPE;
  // return returned;

  return makeNull();
  
}


Value *evalQuote(Value *args, Frame *frame) {
  if(length(args) != 1){ 
    puts("Evaluation error in Quote\n");
    evaluationError();
  }
  return car(args);
}

Value *evalDefine(Value *args, Frame *frame) {
  if(length(args) != 2) {
    puts("Evaluation error: wrong number of args in define\n");
    evaluationError();
  }
  if(car(args)->type != SYMBOL_TYPE) {
    puts("Evaluation error: first arg in define not a symbol\n");
    evaluationError();
  }
  if(car(cdr(args))->type == SYMBOL_TYPE) {
    if(isNull(lookUpSymbol(car(cdr(args)), frame))) {
      puts("Evaluation error: second arg is unbound symbol\n");
      evaluationError();
    }
  } else { // it is a non symbol type Value
    args->c.cdr->c.car = eval(car(cdr(args)), frame);
    frame->bindings = cons(args, frame->bindings);

  }
  Value* void_type = (Value*)talloc(sizeof(Value));
  void_type->type = VOID_TYPE;
  return cons(void_type, makeNull());
}

Value *evalLambda(Value *args, Frame *frame) {
 // printf("Evaluate Lambda\n");
  //printf("Entered evalLambda\n");
  if(length(args) != 2) {
    printf("Evaluation error: wrong number of args in define\n");
    evaluationError();
  }
  Value* tempParams = car(args);
  if (car(tempParams)->type != NULL_TYPE) {
    while (!isNull(tempParams)) {
      if (car(tempParams)->type != SYMBOL_TYPE) {
        puts("Evaluation error: formal parameters for lambda must be symbols.\n");
        evaluationError();
      }
      Value* searchParams = cdr(tempParams);
      //found SYMBOL_TYPE
      while(!isNull(searchParams)) {
        if (!strcmp(car(searchParams)->s,car(tempParams)->s)){
          //exit 
          printf("Evaluation error: duplicate identifier in lambda.\n");
          evaluationError();
        }
        searchParams = cdr(searchParams);
      }
      tempParams = cdr(tempParams);
    }
  } 
////  printf("This is the lambda body we have to somehow check???\n");
 // printTree(car(cdr(args)));
  Value* currentClosure = makeNull();
  currentClosure->type = CLOSURE_TYPE;
  currentClosure->cl.paramNames = car(args);
  currentClosure->cl.functionCode = car(cdr(args));
  currentClosure->cl.frame = frame;
  return currentClosure;
}

Value *evalAnd(Value *args, Frame *frame) {
  Value *returnBool = makeNull();
  returnBool->type = BOOL_TYPE;
  returnBool->s = "#t";
  while(!isNull(args)) {
    if (!strcmp(eval(car(args), frame)->s, "#f")) {
      returnBool->s = "#f";
      return returnBool;
    } 
    args = cdr(args);
  }
  return returnBool;
}

Value *evalOr(Value *args, Frame *frame) {
  Value *returnBool = makeNull();
  returnBool->type = BOOL_TYPE;
  returnBool->s = "#t";
  while(!isNull(args)) {
    if(!strcmp(eval(car(args), frame)->s, "#f")) {
      returnBool->s = "#f";
    } else {
      returnBool->s = "#t";
      return returnBool;
    }
    args = cdr(args);
  }
  return returnBool;
}

Value *evalBegin(Value *args, Frame *frame) {
  Value *lastResult = makeNull();
  while(!isNull(args)) {
    if (isNull(cdr(args))) {
      lastResult = eval(car(args), frame);
      return lastResult;
    } else {
      eval(car(args), frame);
    }
    args = cdr(args);
  }  
  lastResult->type = VOID_TYPE;
  return lastResult;
}

Value *evalCond(Value *args, Frame *frame) {
  Value *returnValue = makeNull();
  while(!isNull(args)) {  
    if (car(args)->type == CONS_TYPE) {
      if (!strcmp(car(car(args))->s, "else")) {
        if(cdr(car(args))->type == NULL_TYPE) {
          puts("Evaluation error: no body to else statement");
          evaluationError();
        } else {
          returnValue = eval(car(cdr(car(args))), frame);
        }
        return returnValue;
      } else if (!strcmp(eval(car(car(args)), frame)->s, "#t")) {
        if(cdr(car(args))->type == NULL_TYPE) {
          returnValue->s = "#t";
          returnValue->type = BOOL_TYPE;
        } else {
       
          returnValue = eval(car(cdr(car(args))), frame);
        }
        return returnValue;
      }  
    }
  args = cdr(args);
  }
return returnValue;
}

Value *apply(Value *function, Value *args){
//  printf("APPLY function to args\n");
 // printf("Entering apply");
 // puts("entered apply\n  ");
  // printf(">>> length(args) %d\n", length(args) );
  // puts("printing len args ");
  // printf(">>> length(function->cl.paramNames) %d\n",length(function->cl.paramNames) );
  // printf("Entering apply statement\n ----len args == len func->paramnames: %d\n",length(function->cl.paramNames) == length(args));
  // printTree(args);
  // printf("\n");
  // puts("\n printed function->cl.paramNames.\n----");
 // printTree(function->cl.paramNames);
//  p//rintf("\n");

  if(length(function->cl.paramNames) != length(args)) {
    puts("Evaluation error: wrong number of args for closure\n");
    evaluationError();
  }

 // printf("\nCorrect number of arguments\n");

  Frame *applyFrame = (Frame*)talloc(sizeof(Frame));
  applyFrame->bindings = makeNull();
  applyFrame->parent = function->cl.frame;

  // puts("ln 998: applyFrame bindings...:");
  // printTree(applyFrame->bindings);
  // puts("");
  // puts("ln 1001: func closure bindings...:");
  // printTree(function->cl.frame->bindings);
  // puts("");
  Value* tempParamNames = function->cl.paramNames;
  while (!isNull(tempParamNames)) {
    if(length(tempParamNames) != 0) {
      applyFrame->bindings = cons(cons(car(tempParamNames), cons(car(args), makeNull())), applyFrame->bindings);
      args = cdr(args);
    }
    tempParamNames = cdr(tempParamNames);
  }


 // printf("\nCompleted making the binding frame\n");
  // puts("ln 1015: applyFrame bindings...:");
 // printTree(applyFrame->bindings);
  // puts("");
    return eval(function->cl.functionCode, applyFrame);
}

Value *lookUpSymbol(Value *tree, Frame *frame) {
  // puts("entered lookUpSymbol");
  // puts("looking for symbol: ");
  // printf("%s\n", tree->s);
  Frame* temp_frame = frame;
  int symbol_found = 0;
  while (temp_frame!=NULL && symbol_found == 0) {
    Value* temp_bindings = temp_frame->bindings;

    while (!isNull(temp_bindings) && symbol_found == 0) {
      if(!strcmp(tree->s, car(car(temp_bindings))->s)){
        symbol_found = 1;
        // puts("symbol found");
        return car(cdr(car(temp_bindings)));
      }
      temp_bindings = cdr(temp_bindings);
    }
    temp_frame = temp_frame->parent;
  }
  // puts("symbol not found");
  return makeNull();
}

void evaluationError(){
  texit(0);
}


