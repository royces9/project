#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileStruct.h"
#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error = 0, length = 0, maxSize = 1024, i = 0, direction = 0, check = 0;
  char **fileString = calloc(maxSize, sizeof(*fileString));
  __MALLOC_CHECK(fileString, error);

  fileTree *tree = createLeaf(), *head = tree;
  fileStack execStack = newFileStack(), stk = newFileStack();

  inputFile = fopen(input[0], "r");
  if(!inputFile){
    free(tree);
    free(fileString);
    return error = -8;
  }
  printf("\n");
  while(fgets(buffer, 1024, inputFile)){
    int offset = 0;
    //removing trailing spaces
    if(buffer[0] == ' '){
      for(offset = 0; buffer[offset] == ' '; ++offset);
    }
    char *bufferHold = buffer;
    bufferHold += offset;
    length = strlen(bufferHold);

    if(!strcmp(bufferHold, "\n") || (bufferHold[0] == '#')){ //skips a blank line, # comments out a line
      continue;
    }

    if(bufferHold[length - 1] == '\n'){ //replaces end new line with a null terminated character
      bufferHold[--length] = '\0'; //update the length of the new string
    }
    fileString[i] = malloc((length+1) * sizeof(**fileString));
    __MALLOC_CHECK(fileString[i], error);
    strcpy(*(fileString+i), bufferHold);
    head->line = fileString[i];

    direction = checkProgramFlow(*(fileString+i));
    switch(direction){
    case 1: //if
    case 2: //while
    case -2: //else
      fPush(&stk, head);
      head->right = createLeaf();
      head = head->right;
      break;

    case -1: //end
      head = fPop(&stk);

    default:
      head->left = createLeaf();
      head = head->left;
      break;
    }

    if(++i >= maxSize){
      maxSize *= 2;
      fileString = realloc(fileString, maxSize * sizeof(*fileString));
    }
  }  

  head = tree;

  while((head != NULL) && (head->line != NULL)){
    direction = checkProgramFlow(head->line);

    //if the line ends with ';', don't print the line, still executes
    if((head->line[strlen(head->line)-1] != ';') && direction == 0){
      printf("> %s\n", head->line);
    }

    switch(direction){
    case 1: //if
      check = checkConditional(head->line, direction, var, ans);
      if(check < 0){ //if there is an error in the if
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return check;
      }

      if(check){
	fPush(&execStack, head->left);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;

    case 2: //while
      check = checkConditional(head->line, direction, var, ans);
      if(check < 0){
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return check;
      }

      if(check){
	fPush(&execStack, head);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;
	
    case -1: //end
      head = fPop(&execStack);
      break;
      
    case -2: //else
      if(check == 0){ //checks the value from the previous if iteration
	fPush(&execStack, head->left);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;

    default:
      error = sya(head->line, ans, var);
      if(error){
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return error;
      }

      if((head->line[strlen(head->line)-1] != ';') && direction == 0){
	printf(">     %lf\n", *ans);
      }

      head = head->left;
      break;
    }
  }


  //free tree
  cutDownTree(tree);

  //free array of strings
  freeString(fileString, maxSize);

  fclose(inputFile);
  return 0;
}

fileStack newFileStack(){
  fileStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}

int checkProgramFlow(char *input){
  if(strstr(input, "else")) return -2;
  if(strstr(input, "end")) return -1;
  if(strstr(input, "if(")) return 1;
  if(strstr(input, "while(")) return 2;
  return 0;
}

char *parseCondition(char *input, int type){
  switch(type){
  case 1: //if
    input = strchr(input, 'i'); //take advantage of the fact that it searches for first occurence
    input += 2;
    return input;
  case 2: //while
    input = strchr(input, 'w');
    input += 5;
    return input;
  }
}

int checkConditional(char *input, int type, vari *var, double *ans){
  input = parseCondition(input, type);
  int error = sya(input, ans, var);
  if(error){
    return error;
  }
  return *ans;
}

void freeString(char **string, int max){
  for(int i = 0; i < max; ++i){
    free(string[i]);
  }
  free(string);
}
