#include <string.h>

#include "stack.h"

void pushn(double inp, numberStack *st){
  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }
  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

double popn(numberStack *st){
  double out = 0;
  if(st->occ == 1){
    out = st->stk[st->top--];

    if(st->top == -1){
      st->occ = 0;
      st->top = 0;
    }
  }
  return out;
}
//characters
void pushch(operatorStruct inp, operatorStack *st){
  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }

  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

operatorStruct popch(operatorStack *st){
  operatorStruct out;
  if(st->occ == 1){
    out = st->stk[st->top--];

    if(st->top == -1){
      st->stk[0].operator[0] = '\0';
      st->stk[0].argNo = 0;
      st->occ = 0;
      st->top = 0;
    }
  }
  else{
    out.operator[0] = '\0';
    out.argNo = 0;
  }
  return out;
}

numberStack newNumberStack(){
  numberStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}

operatorStack newOperatorStack(){
  operatorStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, '\0', sizeof(out.stk));
  return out;
}
