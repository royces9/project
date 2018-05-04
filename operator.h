#ifndef OPERATOR
#define OPERATOR

#include "stack.h"
#include "operatorUtility.h"

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
void execNum(numberStack *num, operatorStruct ch);
matrix *matrixOneArg(matrix *a, operatorstruct ch, int *error);
matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct ch, int *error);
double oneArg(double a, int o);
double twoArg(double a, double b, int o);
int findFunction(char *buffer, numberStack *num, operatorStack *ch, matrix *ans, vari *var, int *tok, int *start, char *input);
int findOperator(char *buffer, numberStack *num, operatorStack *ch, matrix ans, vari *var, int *tok);
matrix *extractMatrix(numberStack *num, operatorStack *ch, matrix *ans, vari *var, int *start, char *input, int *error);
int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint();

#endif //OPERATOR
