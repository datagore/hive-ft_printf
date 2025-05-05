// clang emits a warning if you try to print a NULL string. That warning is
// disabled here, so that we can test with NULL strings.
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wformat-overflow"
#endif

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ft_printf.h"

// ANSI escape codes.
#define ANSI_GREEN  "\e[1;32m" // Set the text color to green.
#define ANSI_RED    "\e[1;31m" // Set the text color to red.
#define ANSI_YELLOW "\e[1;33m" // Set the text color to yellow.
#define ANSI_RESET  "\e[0m"    // Reset to default color.
#define ANSI_CLEAR  "\e[2J"    // Clear the screen.

// Color-coded OK and KO strings.
#define GREEN_OK (ANSI_GREEN "[OK]" ANSI_RESET)
#define RED_KO   (ANSI_RED   "[KO]" ANSI_RESET)

// Helper macro for printing to standard error output. This is used for
// displaying the results, since standard output is redirected during testing.
#define PRINT(...) fprintf(stderr, __VA_ARGS__)

// Run a test case.
#define TEST_CASE(...) do {													\
																			\
		/* Write the output of ft_printf to buffer `a` */					\
		char a[1000] = {0};													\
		ftruncate(1, 0);													\
		lseek(1, 0, SEEK_SET);												\
		int a_return = ft_printf(__VA_ARGS__);								\
		lseek(1, 0, SEEK_SET);												\
		int a_length = read(1, a, sizeof(a));								\
																			\
		/* Write the output of printf to buffer `b` */						\
		char b[1000] = {0};													\
		ftruncate(1, 0);													\
		lseek(1, 0, SEEK_SET);												\
		int b_return = printf(__VA_ARGS__);									\
		fflush(stdout);														\
		lseek(1, 0, SEEK_SET);												\
		int b_length = read(1, b, sizeof(b));								\
																			\
		/* Compare the output of the two functions */						\
		int lengths_match = a_return == b_return && a_length == b_length;	\
		int match = lengths_match && memcmp(a, b, a_length) == 0;			\
																			\
		/* Print OK or KO next to the function input */						\
		PRINT("%s ", match ? GREEN_OK : RED_KO);							\
		PRINT("printf(%s)\n", #__VA_ARGS__);								\
																			\
		/* Print the full output of ft_printf and printf */					\
		PRINT("    ft_printf: \"" ANSI_YELLOW "%.*s", a_length, a);			\
		PRINT(ANSI_RESET "\" (returned %d)\n", a_return);					\
		PRINT("       printf: \"" ANSI_YELLOW "%.*s", b_length, b);			\
		PRINT(ANSI_RESET "\" (returned %d)\n\n", b_return);					\
																			\
		/* Keep a tally of tests/passes */									\
		total_passed += !!match;											\
		total_tested++;														\
	} while (0)

int main()
{
	// Variables for tallying how many test cases passed or failed.
	int total_passed = 0;
	int total_tested = 0;

	// Clear the screen.
	printf(ANSI_CLEAR);
	fflush(stdout);

	// Open a temporary file and redirect standard output to that file.
	int temporary_file = open("temp", O_RDWR | O_CREAT, 0666);
	unlink("temp"); // Remove the file (it disappears the moment it's closed).
	dup2(temporary_file, 1); // All writes to 1 actually go to temporary_file.

	// Run test cases (see macro above).
	TEST_CASE("string");
	TEST_CASE("percent sign: 100%%");
	TEST_CASE("character: %c", '@');
	TEST_CASE("string: %s", "hello");
	TEST_CASE("null string: %s", (char*) NULL);
	TEST_CASE("zero: %d", 0);
	TEST_CASE("positive decimal: %i", 69);
	TEST_CASE("negative decimal: %d", -42);
	TEST_CASE("hexadecimal: %x", 0xc0de);
	TEST_CASE("hexadecimal uppercase: %X", 0xBEEF);
	TEST_CASE("hexadecimal negative: %X", -0xff);
	TEST_CASE("valid pointer: %p", main);
	TEST_CASE("null pointer: %p", NULL);
	TEST_CASE("LONG_MIN pointer: %p", (void*) LONG_MIN);
	TEST_CASE("LONG_MAX pointer: %p", (void*) LONG_MAX);
	TEST_CASE("+ULONG_MAX pointer: %p", (void*) +ULONG_MAX);
	TEST_CASE("-ULONG_MAX pointer: %p", (void*) -ULONG_MAX);
	TEST_CASE("INT_MAX: %d", INT_MAX);
	TEST_CASE("INT_MIN: %i", INT_MIN);
	TEST_CASE("UINT_MAX: %u", UINT_MAX);
	TEST_CASE("UINT_MAX hexadecimal: %x", UINT_MAX);

	// Print a summary of all test cases.
	PRINT("%d/%d tests passed", total_passed, total_tested);
	PRINT(" %s\n", total_passed == total_tested ? GREEN_OK : RED_KO);
}
