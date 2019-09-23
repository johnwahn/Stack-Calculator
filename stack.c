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
 * File Name: stack.c 
 *
 * Description: This program contains the functions that would tested at our
 *		calc.c file which would use our stack functions in order to 
 *		perform functions like a calculator. 
 *
 *
 * *****************************************************************************
  struct hw4
  
 * Description: Contains different functions that would modify or give 
 *		information about our stack object. 
 *
 * Functions: 
 *	
 *	- delete_Stack: This function deallocates all memory of the stack and 
 *	  sets pointer in the calling function to NULL
 *	
 *	- empty_Stack: Empties the stack of all its elements  
 *	
 *	- isempty_Stack: Returns true if stack is empty and false otherwise 
 *	
 *	- isfull_Stack: Returns true if stack is full and false otherwise 
 *	
 *	- new_Stack: Initializes a new Stack object and allocates memory
 *	  for it 
 *	
 *	- get_occupancy: Returns the number of elements in the stack 
 *	
 *	- pop: Removes the top item of the stak and returns true upon 
 *	  sucess and false if pop fails
 *	
 *	- top: Gives information about the top item of the stack, returns true
 *	  if success and returns false otherwise. Does not remove anything from 
 *	  stack
 *	
 *	- push: Adds items at the top of the stack 
  */
 

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
   debug = FALSE;
}

void debug_on (void) {
   debug = TRUE;
}

/*--------------------------------------------------------------------------
Function Name:         delete_Stack 

Purpose:               Deallocates all memory for our stack object and sets
		       the pointer to null in the calling function 

Description:           First checks if *spp is null, if it is then it will
		       print the error message. If the stack exists then 
		       the function will use the free function in order to 
		       deallocate the memory which in turn deletes the stack.
		       Then it will decrement stack_counter and set the pointer
		       to null.
                       
Input:                 **spp - a double pointer to the stack we want to delete

Result:                No return value, and prints out debugging messages if 
		       conditions are met
--------------------------------------------------------------------------*/

void delete_Stack (Stack ** spp) {
   
  if (*spp == NULL) {
      
      fprintf(stderr,DELETE_NONEXIST);
      
      /*ends the function*/
      return;
   }

   if(debug == TRUE) {
      
      fprintf(stderr,DEALLOCATE, (long) stack_counter);
   }
   
   /*freeing the memory*/
   free(*spp - STACK_OFFSET);
   
   /*decrementing stack_counter*/
   stack_counter--;
   
   /*setting *spp to null*/
   *spp = NULL;
         
}

/*--------------------------------------------------------------------------
Function Name:         empty_Stack 

Purpose:               Empties all the elements in the stack 

Description:           First checks if this_Stack is null, if it is then it 
                       will print the error message. If the stack exists then
		       the function will set the stack pointer index to zero.

Input:                 *this_Stack - a pointer to the stack we want to empty

Result:                No return value, and prints out debugging messages if 
		       conditions are met
--------------------------------------------------------------------------*/

void empty_Stack (Stack * this_Stack) {
   
   if (this_Stack == NULL) {
      
      fprintf(stderr,EMPTY_NONEXIST); 
   }

   else {
      
      /*setting pinter index to zero*/
      this_Stack[STACK_POINTER_INDEX] = 0;
   }
}

/*--------------------------------------------------------------------------
Function Name:         isempty_Stack 

Purpose:               Checks if the stack is empty  

Description:           First checks if this_Stack is null, if it is then it 
                       will print the error message. If the stack exists then
		       the function will check if the stack pointer index is
		       0. If it is then it will return 1 (true) or 2 (false)
		       if the pointer index is not equal to zero.

Input:                 *this_Stack - a pointer to the stack we want to check

Result:                Returns long value 0 or 1, and prints out debugging
		       messages if conditions are met.
--------------------------------------------------------------------------*/

        
long isempty_Stack (Stack * this_Stack) {
    
   if (this_Stack == NULL) { 
      
      fprintf(stderr, ISEMPTY_NONEXIST); 
       
      return 1;
   }

   /*checks if the pointer index is zero*/
   if (this_Stack [STACK_POINTER_INDEX] != 0) {
      
      /*returns false if it does not equal to zero*/
      return 0;
   }

   else {
      /*returns true of otherwise*/
      return 1; 
   }
}

/*--------------------------------------------------------------------------
Function Name:         isfull_Stack 

Purpose:               Checks if the stack is full  

Description:           First checks if this_Stack is null, if it is then it 
                       will print the error message. If the stack exists then
		       the function will check if the stack pointer index is
		       equal to the size index. If both are equal it will 
		       return true and false if otherwise. 

Input:                 *this_Stack - a pointer to the stack we want to check

Result:                Returns long value 0 or 1, and prints out debugging
		       messages if conditions are met.
--------------------------------------------------------------------------*/


long isfull_Stack (Stack * this_Stack) {
   
   if(this_Stack == NULL) {
      
      if(debug == TRUE) {
         
	 fprintf(stderr, ISFULL_NONEXIST);
      }
      
      /*returns false if this_Stack is null*/
      return 0;
   }

   if (this_Stack[STACK_POINTER_INDEX] == this_Stack[STACK_SIZE_INDEX]) {
      
      /*true if stack pointer and size are equal*/
      return 1;
   }

   else {
      
      /*false if otherwise*/
      return 0;
   }
}

/*--------------------------------------------------------------------------
Function Name:         new_Stack 

Purpose:               Allocates memory and creates a new Stack object.    

Description:           The function will allocate memory to hold stacksize
		       number of longs and creates the stack infrastructure.
		       We use malloc in order to allocate the memory for 
		       the stack object. We add stacksize and STACK_OFFSET
		       in order to not only have stacksize number of memory
		       inputs but also to have access of our stack size, 
		       count, and pointer in our memory.The function will
		       initialize stack pointer, size, and count index to
		       its appropriate values.

Input:                 stacksize- the amount of elements we want to put in our
		       stack.

Result:                Returns a pointer to the first storage in the stack  
--------------------------------------------------------------------------*/


Stack * new_Stack (unsigned long stacksize) {
	
   /*Allocating memory for our stack based on stacksize*/
   void* memory = malloc((stacksize + STACK_OFFSET) * sizeof(long));
   
   /*allows us to access the first stack memory value*/
   Stack*this_Stack = (Stack*)memory + STACK_OFFSET;
   
   /*initializing stack pointer and size index*/
   this_Stack[STACK_POINTER_INDEX] = 0;
   this_Stack[STACK_SIZE_INDEX] = stacksize;
   
   /*incrementing stack count and initialize count index to that value*/
   stack_counter++;
   this_Stack[STACK_COUNT_INDEX] = stack_counter;
   
   if (debug == TRUE) {

      fprintf(stderr, ALLOCATED, this_Stack[STACK_COUNT_INDEX]);
   }
   
   return this_Stack; 
}

/*--------------------------------------------------------------------------
Function Name:         get_occupancy 

Purpose:               Returns the number of elements in the stack

Description:           First checks if *this_Stack is null, if it is then it 
                       will print the error message. If the stack exists then
		       the function will return the value of stack pointer 
		       index.

Input:                 *this_Stack - a pointer to the stack where we want to
		       check the number of elements in the stack

Result:                Returns the number of elements in stack in long
--------------------------------------------------------------------------*/


long get_occupancy (Stack * this_Stack) {
    
   return this_Stack[STACK_POINTER_INDEX];
}

/*--------------------------------------------------------------------------
Function Name:         pop 

Purpose:               Removes the top element of the stack  

Description:           First checks if this_Stack is null or empty, if it is 
                       then it will print the error message. If the stack 
		       exists then *item will be set equal to the stack pointer
		       index subtracted by one. This will give us the array 
		       index that contains the value of the top element of the
		       stack. Then *item will be set equal to the value of the
		       top element of the stack and stack pointer index will be 
		       decremented.Then the function will return true or false 
		       depending if the execution was successful.

Input:                 *this_Stack - a pointer to the stack where we want to
		       pop the stack

		       *item - a pointer to a long that will store the popped
		       element. 

Result:                Returns long value 0 or 1 (true or false), and prints 
		       out debugging messages if conditions are met.
--------------------------------------------------------------------------*/


long pop (Stack * this_Stack, long * item) {
   
   if (this_Stack == NULL) {
      
      fprintf(stderr, POP_NONEXIST);     
      return 0;
   }
  
   /*check if stack is empty*/
   else if (isempty_Stack(this_Stack)) {
       
       fprintf(stderr,POP_EMPTY);
       return 0;
   }

   else {
      /*getting the appropriate index array that contains top value*/
      *item = this_Stack[STACK_POINTER_INDEX] -1;
      
      /*getting the top element value in the stack*/
      *item = this_Stack[*item];
      
      if(debug == TRUE) {
         
	 /*if popping an operator from the stack*/
	 if(*item < 0) {   
	
	   fprintf(stderr,HEXPOP,this_Stack[STACK_COUNT_INDEX],*item);

	 }
	 else {
         
	   fprintf(stderr,POP,this_Stack[STACK_COUNT_INDEX],*item);
	 }
      }
      
      /*decrementing pointer index*/
      this_Stack[STACK_POINTER_INDEX]--;
      
      return 1;
   }
}

/*--------------------------------------------------------------------------
Function Name:         push 

Purpose:               Adds item to the top of this_Stack 

Description:           First checks if this_Stack is null, if it is then it 
                       will print the error message. If the stack exists then
		       stack_pointer will save the current stack pointer index
		       value, so that we can gain the array index where we want
		       to push item into the appropriate array. After pushing
		       item into the appropriate array we increment stack 
		       pointer index.Then the function will return true or false
		       depending if the execution was successful.

Input:                 *this_Stack - a pointer to the stack where we want to
		       push the stack

		       item - a long that will store the pushed into the stack

Result:                Returns long value 0 or 1 (true or false), and prints 
		       out debugging messages if conditions are met.
--------------------------------------------------------------------------*/


long push (Stack * this_Stack, long item) {
    
    if (this_Stack == NULL) {
       
       fprintf(stderr, PUSH_NONEXIST);

       return 0;
    }

    else {
      
      /*saving the array index value where we want to store item*/
      long stack_pointer = this_Stack[STACK_POINTER_INDEX];
    
       /*inserting item into the appropriate array*/
       this_Stack[stack_pointer] = item;
       
       /*incrementing stack pointer index*/
       this_Stack[STACK_POINTER_INDEX]++;
       
       if (debug == TRUE) {
	  
	  /*if pushing an operator from the stack*/
	  if(item < 0) {

	     fprintf(stderr, HEXPUSH, this_Stack[STACK_COUNT_INDEX], item);
	  }   
	  else {
	     
	     fprintf(stderr, PUSH, this_Stack[STACK_COUNT_INDEX], item);
	  }
       }

       /*if stack pointer index is greater then size index then print debug 
	* message and return false */
       if (this_Stack[STACK_POINTER_INDEX] > this_Stack[STACK_SIZE_INDEX]) {

	  fprintf(stderr,PUSH_FULL);

          return 0;
       }
       
       return 1;
    }
}

/*--------------------------------------------------------------------------
Function Name:         top

Purpose:               Get the value of the top element in the stack  

Description:           First checks if *this_Stack is null or empty, if it is 
                       then it will print the error message. If the stack 
		       exists then *item will be set equal to the stack pointer
		       index subtracted by one. This will give us the array 
		       index that contains the value of the top element of the
		       stack. Then *item will be set equal to the value of the
		       top element of the stack 

Input:                 *this_Stack - a pointer to the stack where we want to
		       perform the top function

		       *item - a pointer to a long that will store the top
		       element. 

Result:                Returns long value 0 or 1 (true or false), and prints out
		       debugging messages if conditions are met.
--------------------------------------------------------------------------*/


long top (Stack * this_Stack, long * item) {
   
   /*debug messages*/
   if (this_Stack == NULL) {
	
      fprintf(stderr,TOP_NONEXIST);
      return 0;
   }

   else if(isempty_Stack(this_Stack)) {
      
      fprintf(stderr,TOP_EMPTY);
      return 0;
   }

   else {
      
      /*getting the appropriate index array that contains top value*/
      *item = this_Stack[STACK_POINTER_INDEX]-1;
      
      /*getting the top element value in the stack*/
      *item = this_Stack[*item];
      
      if(debug == TRUE) {
         
	 /*if topping an operator from the stack*/
	 if(*item < 0) {
	    
	 fprintf(stderr,HEXTOP,this_Stack[STACK_COUNT_INDEX],*item); 

	 }
         
         else {
	 
	 fprintf(stderr,TOP,this_Stack[STACK_COUNT_INDEX],*item); 
         
	 }
      }

      return 1;
   }
}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
