/*
   This assignment is to help you learn how to debug
   compiler warnings/errors and other common errors
   in your code. For each part labeled P(n), there is
   a warning/error/problem that goes with it. Write
   down what the issue was in the `Error:` section of
   each problem. Submit `segfault.c` along with your
   fixes and error comments.
 */

// P0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Error:
	P0 missing following headers:
		1. <string.h> for defines string handling functions and strpcy()
 */

void fib(int *A, int n);

int
main(int argc, char *argv[]) {
	int buf[10];
	unsigned int i;
	char *str;
	char *printThisOne;
	char word[] = "Part 3";
	int *integers;
	int foo;
	int *bar;
	char *someText;
	
	// P1
	for (i = 0; i < 10; ++i) {
		buf[i] = i;
	}
	for (i = 0; i < 10; ++i) {
		printf("Index %u = %d\n", i, buf[i]);
	}
	/* Error:
		P1 occured two warning: 
			1. segfault.c:37:29: warning: format specifies type 'char *' but the argument has type 'unsigned int' [-Wformat]
					printf("Index %s = %s\n", i, buf[i]);
				
				Format specifies type "char *" doesn't support the argument type "unsigned int"
				Fix: Replace the first format specifiers %s with %u (for unsigned integer) in the printf statement.


			2. segfault.c:37:32: warning: format specifies type 'char *' but the argument has type 'int' [-Wformat]
					printf("Index %s = %s\n", i, buf[i]);

				Format specifies type "char *" doesn't support the argument type "int"
				Fix: Replace the second format specifiers %d with %u (for unsigned integer) in the printf statement.

		Occured abort trap: 6 on for loop conditional when i <= 10.
			Fix: Chagne to i < 10			
	 */

	// P2
	str = (malloc(sizeof(char*) * 10));
	printThisOne = (malloc(sizeof(char*) * 10));

	strcpy(str, "Something is wrong");
	strcpy(printThisOne, str);

	printf("%s\n", printThisOne);
	/* Error:
		P2 occured two warning: 
			1. segfault.c:57:2: warning: implicitly declaring library function 'strcpy' with type 'char *(char *, const char *)' [-Wimplicit-function-declaration]
        		strcpy(str, "Something is wrong");

			Fix: Declare <string.h> at the header
			
			2. segfault.c:62:17: warning: variable 'printThisOne' is uninitialized when used here [-Wuninitialized]
        		printf("%s\n", printThisOne);

		Occured (null) when compiler because printThisOne has no value. 
			Fix: To assign "Something is wrong" to printThisOne, use malloc() to dynamically allocate a block of memory with the specified size and use strcpy() 
		function to copy the value from str to printThisOne.

		Missing "strcpy" library functions
			Fix: Add "string.h" at headers


			Occured Bus error: 10 from prinf() statemetn becuase is it not legal to modify a string literal. The processor cannot attempt to access the memory area requested.
			Fix: 

	 */

	// P3
	*(word + 4) = '-';
	printf("%s\n", word);
	/* Error:
		P3 occured Bus error: 10:
			The issues was the declaration of char* word. We cannot modify it because of the pointer type declaration.
				Fix: Declare array notations instead of pointer.
	 */

	// P4
	integers = (malloc(sizeof(int*) * 10));
	*(integers + 10) = 10;
	printf("Part 4: %d\n", *(integers + 10));
	free(integers);
	/* Error:
		P4 occured one warning: 
				warning: variable 'integers' is uninitialized when used here [-Wuninitialized]
					*(integers + 10) = 10;
				
				Fix: declare malloc() to dynamically allocate a block of memory with the specified size
	 */

	// P5
	printf("Print this whole line\n");
	/* Error:
		P5 occured an warning:
			segfault.c:91:27: warning: format string contains '\0' within the string body [-Wformat]
				printf("Print this whole \0 line\n");
			
			\0 is null character, is a control character with the value zero.
			Fix: To get off the warning, simply remove the \0
	 */

	// P6
	int x;
	x = 2147483647;
	printf("%d is positive\n", x);
	x += 1000000000;
	printf("%d is positive\n", x);
	/* Error:
		P6 occured four errors:
			1. segfault.c:102:2: error: use of undeclared identifier 'x'
        		x = 2147483647;

			2. segfault.c:103:29: error: use of undeclared identifier 'x'
       			 printf("%d is positive\n", x);

			3. segfault.c:104:2: error: use of undeclared identifier 'x'
        		x += 1000000000;

			4. segfault.c:105:29: error: use of undeclared identifier 'x'
        		printf("%d is positive\n", x);

			The system didn't recognize the variable x because it did not declare.
			1 - 4 Fix: Declare the x with int type variable at the beginning of the main function.
	 */

	// P7
	printf("Cleaning up memory from previous parts\n");
	free(str);
	free(printThisOne);
	/* Error:
		P7 issue:
			The free() deallocates the memory previously allocated by a call to malloc(). str being malloc() but buf is array. 
			printThisOne being allocated but did not dealoocates the memory.
				Fix: Replace free(buf) with free(printThisOne).
	 */

	// P8
	fib(&foo, 7);
	printf("fib(7) = %d\n", foo);
	/* Error:
		P8 occured two warning:
			1. segfault.c:135:6: warning: incompatible integer to pointer conversion passing 'int' to parameter of type 'int *'; take the address with & [-Wint-conversion]
        		fib(foo, 7);

			2. segfault.c:136:26: warning: format specifies type 'int' but the argument has type 'int *' [-Wformat]
        		printf("fib(7) = %d\n", foo);
			
			The foo missing the asterisk(*) and the format specifies type "int" invalid for the argument type "int *".
			Fix: Add asterisk to the declared variable of foo and replace prinf() with scanf() because scanf() .
	 */

	// P9
	bar = (int*)malloc(sizeof(int));
	*bar = 123;
	printf("bar = %d\n", *bar);
	/* Error:
		bar = 0; caused the segnebtatui fault.
			Fix: bar should use malloc to initializes the allocated memory.
	 */

	// P10
	someText = malloc(sizeof(char) * 10);
	strcpy(someText, "testing");
	printf("someText = %s\n", someText);
	free(someText);
	/* Error:
		1. Sizeof operator needed for malloc. Because it helps the code more readable.

		2. Free() should be after printf statement. If free() before the print statement, the printf would will not print the assign's value.
		Therefore, the free() should declare after the printf statemetn to dealoocates the memory when don't need the value not before the printf statement.
	 */

	exit(0);
}

// fib calculates the nth fibonacci number and puts it in A.
// There is nothing wrong with this function.
void fib(int *A, int n)
{
	int temp;
	if (n == 0 || n == 1)
		*A = 1;
	else {
		fib(A, n - 1);
		temp = *A;
		fib(A, n - 2);
		*A += temp;
	}
}
