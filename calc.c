/*******************************************************************************
 * 
 *							John Ahn
 *							CSE 12, Winter 2019
 *							January 28, 2019
 *							cs12xab
 *
 *
 * 
 *				   Assignment 4 
 *
 * File Name: calc.c 
 *
 * Description: This program contains functions that will perform similar 
 *		executions of a calculator using the stack functions from
 *		stack.c. 
 *
 *
 * *****************************************************************************
  struct hw4
  
 * Description: Contains different functions that would perform mathematical 
 *		functions such as addition, subtraction, multiplication, 
 *		division, exponentiation, and factorial as well as containing
 *		functions that would help evaluate mathematical expressions and
 *		convert mathematical operations into values that we can help us
 *		distinguish between a number and an operator in a stack. 
 *
 * Data Fields: 
 *	
 *	- BYTE: represents 8 bits 
 *
 * Functions: 
 *	
 *	- eval: This function uses 2 Stacks in order to solve mathematical 
 *	expressions accordingly based on the associate priorites of 
 *	the operators and user input. 
 *
 *	
 *	- intopost: This function uses 2 Stacks in order to set the order of
 *	mathematical expressions so that it can be correctly 
 *	evaluated in the eval function. 
 *
 *	- exponent: When the user inputs an ^ operator it will perform an 
 *	exponetial expression raising base to the power exponent.
 *
 *	- fact: When the user inputs a ! operator it will perform a factorial
 *        based on xxx
 *
 *	- setupword: Manipulating the bit pattern, this function allows 
 *	operators to be stored in the Stack by converting the bits
 *	into a negative value so that we can distinguish between an
 *	operator and a number which has a positive value. 
 *	
 */

#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE (8)

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
   { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*--------------------------------------------------------------------------
Function Name:         eval

Purpose:               Utilizing 2 stacks in order to solve mathematical 
		       expressions from postfix notation 

Description:           We first create a new stack2 object and reverse stack1
		       unto stack2 until stack 1 is empty. Then we pop stack2
		       until stack2 is empty. During this process stack2 will
		       pop all digits and push those digits directly to stack1.
		       However if a non-digit is encountered in stack2, the 
		       appropriate operand will be popped from stack1 and
		       evaluated using the operator that was popped from stack2.
		       The result of the computation will be pushed to stack1. 
		       Afterwards stack2 will be deallocated and the result will
		       be popped from stack1.
                       
Input:                 *stack1 - pointer to the Stack containing postfix 
		       expressions to be evaluated.

Result:                Returns a long result from stack1 which would be the 
                       final result of the expression  
		       
--------------------------------------------------------------------------*/

long eval (Stack * stack1) {

   /*creates new stack*/
   Stack * stack2 = new_Stack(CALCSTACKSIZE);

   /*preparing variables for character and operends*/
   long character;
   long op1;
   long op2; 
   
   /*reversing stack1 onto stack2*/
   while (!isempty_Stack(stack1)) {
      
      pop(stack1, &character);   
      push(stack2, character);  

   }
   /*while stack2 not empty*/
   while (!isempty_Stack(stack2)) {

      /*topping and popping everything in stack*/
      top (stack2, &character);
      pop(stack2, &character);

      /*if operation at top of the stack*/
      if (character < 0) {
         
	 /*if the character is a factorial then pop once and solve factorial*/
	 if(CHARACTER(character) == '!') {
	    
	    pop(stack1, &op1);
  
	    character = functions[INDEX(character)](op1, op2);
	    /*push the result to stack1*/
	    push(stack1, character);

	 }

         /*for all other operations pop twice and solve the given operation*/
	 else {
	    
	    pop(stack1, &op1);
	    pop(stack1, &op2);
	 
	    character = functions[INDEX(character)](op1, op2);
	    /*push result to stack1*/
	    push(stack1, character);
	 }

      }

      /*pushing all digits into stack1*/
      else {

         push(stack1, character);
      }
   }
    
     /*deallocate stack2 and pop the result from stack1*/
     delete_Stack(&stack2);
     pop(stack1, &character);
     
     /*returns the final result after all appropriate calculation*/
     return character;
}

/*--------------------------------------------------------------------------
Function Name:         intopost 

Purpose:               Utilizing 2 stacks to convert infix mathematical 
		       expressions inputted by the user in order to put
		       the mathematical experssions in order. 

Description:           First we create a new stack2 object. Then we have
		       character that is equal to fgetc in order to keep
		       track of our stdin. If the stdin is EOF, the function 
		       will return EOF. If the character is blank then it will 
		       continue. If a character is a digit we will continue to 
		       read the digits until a non-digit is met. For every digit
		       we will use decin() in order to convert the number to a 
		       decimal. But before we use decin() we use ungetc() in 
		       order to retain the full digit. If the input is '(' then
		       it will be pushed to stack. If the input is ')' then it 
		       will pop stack2 and push all symbols popped from stack2 
		       onto stack 1 until the first '(' is popped from stack 2. 
		       Afterwards '(' and ')' will be discarded. Lastly if none 
		       of these conditions much it means that the input is an 
		       mathematical operation so stack2 will be repeatly popped
		       and these items will be psuhed onto stack1 until stack2 
		       is empty or until stack2's top operation has a lower 
		       priority than the character entered. Then the character 
		       will be pushed onto stack2. All operations, '(', and '('
		       will be convereted into a negative value using the 
		       setupword function in order to help us distinguish 
		       between a operator and a digit within a stack 
                       
Input:                 *stack1 - a pointer to the empty stack that will store 
		       postfix expressions 

Result:                Returns EOF if ^D is entered or a non-zero value which
		       implies that the function succeeded. 
--------------------------------------------------------------------------*/

long intopost (Stack * stack1) {

   /*creating new stack object*/
   Stack * stack2 = new_Stack(CALCSTACKSIZE); 
     
   /*initializing character and operator which will be used later*/
   long character = 0;
     
   while (character != ('\n')) {
      
      /*contains stdin*/
      character = fgetc(stdin);

      /*when ^D is pressed*/
      if (character == EOF) {
	   
         delete_Stack(&stack2);
	   
	 return EOF; 
      }

      /*ignores blank space*/
      if (character == ' ') {
	   
         continue; 
      }
	
      /*discards the newline character value*/
      if (character == '\n') {
		
         break;
      }

      /*if stdin is digit first ungetc and then use decin()*/
      if (character >= '0' && character <= '9') {
	   
         ungetc(character, stdin);
	   
	 /*converting stdin value with decin and pushing that value to stack1*/ 
	 character = decin();
	 push(stack1,character);
	  
      }

      else if (character == '(') {
	   
         /*variable that saves '(' setupword value*/
	 long cValue; 
	   
	 cValue =  setupword(character);
         /*pushes the converted value into stack2*/
	 push(stack2, cValue);
      }

      else if (character == ')') {

         /*pops top item of stack2*/
	 pop(stack2, &character);
	  
	 /*then it will push all popped operators in stack2 to stack1
	  * until the first '(' is at the top of stack */
	 while (CHARACTER(character) != '(') {
              
	    push(stack1, character);
	    pop (stack2, &character);
         }
      }

      else {
	 
	 /*variable that will save the top value from stack2*/
         long topSymbol;
	 
	 /*variable that saves the converted operator value*/
	 long operator = 0;
         
	 /*operator saves the converted value of character */
	 operator = setupword(character); 
	   
	 while (!isempty_Stack(stack2)) { 
	   
	    /*topSymbol will have the address of the top value*/
	    top(stack2, &topSymbol);
	      
	    /*pop from stack2 and push the operator to stack1 until
	     *top symbol has greater prority than the operator*/
	    if (PRIORITY(operator) <= PRIORITY(topSymbol)) {

	       pop(stack2, &topSymbol);
	       push(stack1, topSymbol);
	    }	

	    else {
		
               break;   
	    }
         }
	 /*pushing the character into stack2*/
         push(stack2, operator);
      }
   }

   /*empties the rest of stack2*/
   while (!isempty_Stack(stack2)) {
	   
      pop(stack2, &character);
      push(stack1, character);

   }
 
   /*deallocates stack 2*/
   delete_Stack(&stack2);

   return 1;
}

static long add (long augend, long addend) {
   return augend + addend;
}

static long divide (long divisor, long dividend) {
   return dividend / divisor;
}

/*--------------------------------------------------------------------------
Function Name:         exponent 

Purpose:               Evaluates an exponential expression based on user input 

Description:           If the power is equal to zero it will return one. If not 
		       it will go through a loop and in each iteration base will
		       be multiplied by itself multiple times based on the input
		       power which the user inputs. 
                       
Input:                 long base - will the base value in our exponential 
                       expression
		       
		       long power - will be the power in our exponential 
		       expression 

Result:                Returns a long value result of the exponential expression

--------------------------------------------------------------------------*/

static long exponent (long power, long base) {
     
   /*initializing result and iteration*/
   long result = base;
   long iteration = 0; 

   if (power == 0) {
      
      return 1;
   }

   /*iterates until iteration is less than power*/
   for (iteration = 1; iteration < power; iteration++) {
      
      /*result multiplied by base multiple times*/
      result = result*base; 

   }
   
   return result;
}

/*--------------------------------------------------------------------------
Function Name:         fact

Purpose:               Evaluates a factorial expressions based on input

Description:           Using a few instance variables, the function will use a 
		       for loop and multiply result with iteration and in each
		       iteration of the loop the variable iteration will be 
		       incremented. 


Input:                 long xxx - value that will be used for the factorial 
		       expression.
		       
		       long ignored - a parameter to be ignored  

Result:                Returns a long result from stack1 which would be the 
                       final result of the factorial expression  
		       
--------------------------------------------------------------------------*/


static long fact (long xxx, long ignored) {
     
   /*initializing result and iteration*/
   long result = 1; 
   long iteration = 0;

   /*iterates until iteration is less than or equal to xxx*/
   for (iteration = 1; iteration <= xxx; iteration++) { 
      
      /*multiplies result with subsequent result and iteration*/
      result *= iteration; 
   }
   
   return result;
}

static long mult (long factorx, long factory) {
   return factorx * factory;
}

/*--------------------------------------------------------------------------
Function Name:         setupword

Purpose:               Converts the ASCII value of an operator into a negative
		       value so that we can distinguish it between a digit
		       (which is positive) and an operator which is negative. 

Description:           While true we will check if character can be found in the
		       operators array. If nothing matches it will break, but if
		       a match is found then we will shift the value character 
		       by one BYTE and return the converted value


Input:                 int character - ASCII values for operators entered by the
		       user. 

Result:                Returns the converted value of character
		       
--------------------------------------------------------------------------*/

static long setupword (int character) {
   
   /*initiating index*/
   int index = 0; 
   
   /*while true check if character equals to any of the operators*/
   while(1) {
      
      if (character != operators[index]) {     
	 index++;
      }

      else {
	/*if no match is found then break*/
	break;
      }

   }
   /*shifts character by one BYTE and returns the converted negative value*/
   return SIGN_BIT| (index << BYTE) | character;
}

static long sub (long subtrahend, long minuend) {
   return minuend - subtrahend;
}

