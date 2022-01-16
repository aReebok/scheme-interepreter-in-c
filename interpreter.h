#ifndef _INTERPRETER
#define _INTERPRETER


void interpret(Value *tree);

Value *evalIf(Value *args, Frame *frame);
Value *evalLet(Value *args, Frame *frame);
Value *evalLetrec(Value *args, Frame *frame);
  Value *letrecHelper(Value *unevalBindings, Frame *currFrame);
Value *evalLetStar(Value *args, Frame *frame);
Value *evalSet(Value *args, Frame *frame);
Value *evalQuote(Value *args, Frame *frame);
Value *evalAnd(Value *args, Frame *frame);
Value *evalOr(Value *args, Frame *frame);
Value *evalBegin(Value *args, Frame *frame);
Value *evalCond(Value *args, Frame *frame);
Value *evalDefine(Value *args, Frame *frame);
Value *evalLambda(Value *args, Frame *frame);

Value *eval(Value *expr, Frame *frame);
Value *lookUpSymbol(Value *tree, Frame *frame);
Value *apply(Value *function, Value *args);

//primitive functions here ----
Value *primitivePlus(Value *args);
Value *primitiveCar(Value *args);
Value *primitiveCdr(Value *args);
Value *primitiveNull(Value *args);
Value *primitiveCons(Value *args);
Value *primitiveMinus(Value *args);
Value *primitiveEqual(Value *args);
Value *primitiveGreaterThan(Value *args);
Value *primitiveLessThan(Value *args);
Value *primitiveModulo(Value *args);

void bind(char *name, Value *(*function)(Value *), Frame *frame);

void evaluationError();


#endif

