#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error = 0, length = 0;

  inputFile = fopen(input[0], "r");
  if(!inputFile){
    return error = -7;
  }
  printf("\n");

  while(fgets(buffer, 1024, inputFile)){
    length = strlen(buffer);

    if(!strcmp(buffer, "\n") || (buffer[0] == '#')){ //skips a blank line, # comments out a line
      continue;
    }

    if(buffer[length - 1] == '\n'){ //replaces end new line with a null terminated character
      buffer[--length] = '\0'; //update the length of the new string
    }

    if(buffer[length - 1] != ';'){ //if the line ends with ';', don't print the line, still executes
      printf("> %s\n", buffer);
    }
    error = sya(buffer, ans, var);

    if(error){
      fclose(inputFile);
      return error;
    }

    if(buffer[length - 1] != ';'){
      printf(">    %lf\n\n", *ans);
    }

  }  
  fclose(inputFile);
  return 0;
}
