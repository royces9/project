#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stack.h"
#include "multi.h"
#include "onearg.h"
#include "sya.h"

int numberOfArgs(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], "\0"); ++i);
  return i;
}

double min(char **input, vari *var, int *error){
  char *str2d, *str2d2;
  double out, inter;
  int argNo = numberOfArgs(input);

  *error = sya(input[0], &out, var);
  if(*error) return 0;

  *error = sya(input[1], &inter, var);
  if(*error) return 0;

  out = fmin(out, inter);
  for(int i = 2; i < argNo; i++){
    sya(input[i], &inter, var);
    if(*error) return 0;
    out = fmin(out, inter);
  }
  return out;
}

double max(char **input, vari *var, int *error){
  double out, inter;
  int argNo = numberOfArgs(input);

  *error = sya(input[0], &out, var);
  if(*error) return 0;

  *error = sya(input[1], &inter, var);
  if(*error) return 0;

  out = fmax(out, inter);
  for(int i = 2; i < argNo; i++){
    *error = sya(input[i], &inter, var);
    if(*error) return 0;
    out = fmax(out, inter);
  }
  return out;
}

double avg(char **input, vari *var, int *error){
  double sum = 0, partSum = 0;
  int argNo = numberOfArgs(input);
  for(int i = 0; i < argNo; ++i){
    sya(input[i], &partSum, var);
    if(*error) return 0;
    sum += partSum;
  }
  return sum/argNo;
}

double deri(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, point = 0, h = 0;
  vari varTemp = *var;
  int varIndex = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = point
  input[3] = tolerance 
   */

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  *error = sya(input[2], &point, &varTemp);
  if(*error) return 0;

  *error = sya(input[3], &h, &varTemp);
  if(*error) return 0;
  
  //set up a dummy variable specified by user  
  varIndex = varcheck(&varTemp, input[1]);

  if(varIndex == -1){
    varIndex = 0;
    varTemp.occ = 1;
  }
  else if(varIndex == -2){
    varIndex = ++varTemp.count;
  }

  strcpy(varTemp.name[varIndex], input[1]);

  //sets the dummy variable equal to x+h
  varTemp.value[varIndex] = point + h;
  
  //does f(x+h)
  *error = sya(input[0], &out, &varTemp);
  if(*error) return 0;

  //sets the dummy variable equal to x-h
  varTemp.value[varIndex] = point - h;

  //does f(x-h)
  *error = sya(input[0], &inter, &varTemp);
  if(*error) return 0;

  //this is f(x+h) - f(x-h)
  out -= inter;

  return out/(2*h);
}

double inte(char **input, vari *var, int *error){
  //check number of arguments
  if(numberOfArgs(input) != 5){
    *error = -2;
    return 0;
  }

  char *str2d;
  double out = 0, inter = 0, step = 0, number = 0, a = 0, b = 0, sum = 0;
  vari varTemp = *var;
  int varIndex = 0, iter = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = left bound
  input[3] = right bound
  input[4] = partition count
   */

  //get number of steps, and step size
  *error = sya(input[2], &a, &varTemp);
  if(*error) return 0;

  *error = sya(input[3], &b, &varTemp);
  if(*error) return 0;

  *error = sya(input[4], &number, &varTemp);
  if(*error) return 0;
  
  step = (b-a)/number;

  //set dummy variable
  varIndex = varcheck(&varTemp, input[1]); //checks if variable exists or not, return value used as index
    
  if(varIndex == -1){
    varIndex = 0;
    varTemp.occ = 1;
  }
  else if(varIndex == -2){
    varIndex = ++varTemp.count;
  }
  strcpy(varTemp.name[varIndex],input[1]); //copy the dummy variable into struct

  //calculate integral using composite Simpson's

  number = floor(number/2); //if the number of steps is odd, change it to be even
  
  for(int i = 1; i <= number; ++i){
    varTemp.value[varIndex] = a + (((2 * i) - 2) * step);
    *error = sya(input[0], &out, &varTemp);
    if(*error) return 0;
    sum += out;

    varTemp.value[varIndex] = a + (((2 * i) - 1) * step);
    *error = sya(input[0], &inter, &varTemp);
    if(*error) return 0;
    sum += (4 * inter);

    varTemp.value[varIndex] = a + ((2 * i) * step);
    *error = sya(input[0], &out, &varTemp);
    if(*error) return 0;
    sum += out;
  }

  //return integral
  return sum * (step / 3);
}

double solve(char **input, vari *var, int *error){
  //check number of arguments
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  char *str2d;
  vari varTemp = *var;
  double out = 0, inter = 0, h = 0, test = 0, delta = 0.000001;
  int varc = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = initial guess
  input[3] = tolerance
   */

  //set dummy variable
  varc = varcheck(&varTemp, input[1]);

  if(varc == -1){
    varc = 0;
    varTemp.occ = 1;
  }
  else if(varc == -2){
    varc = ++varTemp.count;
  }
  strcpy(varTemp.name[varc],input[1]);

  *error = sya(input[2], &varTemp.value[varc], &varTemp);
  if(*error) return 0;

  *error = sya(input[3], &h, &varTemp);
  if(*error) return 0;

  test = h + 1; //ensure test is always greater than h

  while(fabs(test) > h){
    *error = sya(input[0],&out, &varTemp);
    if(*error) return 0;

    varTemp.value[varc] -= delta;
    *error = sya(input[0],&inter, &varTemp);
    if(*error) return 0;

    test = (delta*out)/(out-inter);
    varTemp.value[varc] -= test;
  }

  return varTemp.value[varc];
}
void removeSpaces(char *input, int *front, int *back){
  if(input[0] == ' '){
    int i = 1;
    for(i = 1; input[i] != ' '; ++i);
    *front = i;
  }
  int len = strlen(input);
  if(input[len-1] == ' '){
    int i = 1;
    for(i = 1; input[len - i] != ' '; ++i);
    *back = i;
  }
}

int printLine(char **input, vari *var, int *error){
  int argNo = numberOfArgs(input), front = 0, back = 0;

  for(int i = 0; i < argNo; ++i){
    int len = strlen(input[i]), string = 0;

    if(input[i][0] == '"'){
      string = 1;
    }
    else{
      for(front = 0; input[i][front] == ' '; ++front){
	if(input[i][front+1] == '"'){
	  string = 1;
	}
      }
    }

    if(input[i][len-(back+1)] == '"'){
      string += 1;
    }
    else{
      for(back = 0; input[i][len-(back+1)] == ' '; ++back){
	if(input[i][len-(back+1)] == '"'){
	  string += 1;
	}
      }
    }

    if(string){
      input[i][len-(back+1)] = '\0';
      if(string == 2){
	if((input[i][len-(back+3)] == '\\') && (input[i][len-(back+2)] == 'n')){
	  input[i][len-(back+3)] = '\0';
	  printf("%s\n", input[i]+front+1);
	}
	else{
	  printf("%s", input[i]+front+1);
	}
      }
      else{
	return -9;
      }
    }
    else{
      double out;
      *error = sya(input[i], &out, var);
      if(*error) return 0;
      printf("%lf", out);

    }
  }
  return 0;
}

char **separateString(char *input, char delimiter, int *start, int *error){
  char *tok;
  int leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, delimiterCount = 0, i = 0;  

  char strDelimiter[2];
  strDelimiter[0] = delimiter;
  strDelimiter[1] = '\0';
  
  input += (*start+1);
  
  for(length = 0; input[length]; ++length){
    //increment count if char is left or right end parenthesis
    leftParenthesisCount += (input[length] == '(');
    rightParenthesisCount += (input[length] == ')');

    //increment count if char is the delimiter
    delimiterCount += (input[length] == delimiter);

    if(leftParenthesisCount == rightParenthesisCount){
      break;
    }
  }

  char *input2 = malloc((length + 3)* sizeof(*input2));
  __MALLOC_CHECK(input2, *error);

  strcpy(input2,input);
  input2[length] = 0;

  //allocate double array output and populate it the strings
  char **separatedString = malloc((delimiterCount + 2) * sizeof(*separatedString));
  __MALLOC_CHECK(separatedString, *error);

  *start += (length+1);
  tok = strtok(input2, strDelimiter);

  separatedString[0] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
  __MALLOC_CHECK(separatedString[0], *error);
  strcpy(separatedString[0], ++tok);

  tok = strtok(NULL, strDelimiter);

  for(i = 1; tok != NULL; ++i){
    separatedString[i] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
    __MALLOC_CHECK(separatedString[i], *error);
    strcpy(separatedString[i], tok);
    tok = strtok(NULL, strDelimiter);
  }

  separatedString[i] = malloc(sizeof(**separatedString)); //allocate an end string that is just a char
  __MALLOC_CHECK(separatedString[i], *error);

  separatedString[i][0]= '\0'; //end string

  free(input2);
  return separatedString;
}
