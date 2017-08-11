# calculator

Basic calculator to do calculations

Currently implemented functions

+ - * / ^ 

sin(), cos(), tan(), log(), ln(), sqrt(), avg(), min(), max(), floor(), ceil(), round(), numerical derivation, integration, solver

Current known bugs:

copy pasting integral causes free() failure. ex. "integral((x^2+123),x,0,9,100000)", typing out will give the correct answer, pasting will cause an error: free(): invalid next size (fast)

when running a script, if integral is done in the script, "clear" must be run before the script can be run again or integral is given as an unknown function