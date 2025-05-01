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

// Run a test case.
#define TEST_CASE(...) do { \
		char a[1000] = {0}; \
		ftruncate(1, 0); \
		lseek(1, 0, SEEK_SET); \
		int a_return = ft_printf(__VA_ARGS__); \
		lseek(1, 0, SEEK_SET); \
		int a_length = read(1, a, sizeof(a)); \
		char b[1000] = {0}; \
		ftruncate(1, 0); \
		lseek(1, 0, SEEK_SET); \
		int b_return = printf(__VA_ARGS__); \
		fflush(stdout); \
		lseek(1, 0, SEEK_SET); \
		int b_length = read(1, b, sizeof(b)); \
		int match = a_return == b_return && a_length == b_length && memcmp(a, b, a_length) == 0; \
		fprintf(stderr, "%s ", match ? GREEN_OK : RED_KO); \
		fprintf(stderr, "printf(%s)\n", #__VA_ARGS__); \
		fprintf(stderr, "    ft_printf: \"" ANSI_YELLOW "%.*s" ANSI_RESET "\" (return: %d)\n", a_length, a, a_return); \
		fprintf(stderr, "    printf:    \"" ANSI_YELLOW "%.*s" ANSI_RESET "\" (return: %d)\n", b_length, b, b_return); \
		total_passed += !!match; \
		total_tested++; \
	} while (0)

int main()
{
	// Keep a count of how many test cases passed or failed.
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
	TEST_CASE(NULL);
	TEST_CASE("");
	TEST_CASE("hello");
	TEST_CASE("percentage: 100%%");
	TEST_CASE("character: %c", '@');
	TEST_CASE("string: %s", "hello");
	TEST_CASE("null string: %s", (char*) NULL);
	TEST_CASE("decimal: %d", -42);
	TEST_CASE("decimal: %i", 420);
	TEST_CASE("hexadecimal: %x", 255);
	TEST_CASE("hex uppercase: %X", 65535);
	TEST_CASE("hex negative: %x", -128);
	TEST_CASE("pointer: %p", main);
	TEST_CASE("null pointer: %p", NULL);
	TEST_CASE("LONG_MIN pointer: %p", (void*) LONG_MIN);
	TEST_CASE("LONG_MAX pointer: %p", LONG_MAX);
	TEST_CASE("+ULONG_MAX pointer: %p", +ULONG_MAX);
	TEST_CASE("-ULONG_MAX pointer: %p", -ULONG_MAX);
	TEST_CASE("INT_MAX: %d", +2147483647);
	TEST_CASE("INT_MIN: %i", -2147483648);
	TEST_CASE("UINT_MAX: %u", 4294967295);
	TEST_CASE("UINT_MIN: %u", 0);
	TEST_CASE("UINT_MAX hex: %x", 4294967295);
	TEST_CASE("UINT_MIN hex: %x", 0);
	TEST_CASE("invalid conversion: %w");
	TEST_CASE("trailing percent: %");

	// Print a summary of all test cases.
	fprintf(stderr, "\n%d/%d tests passed", total_passed, total_tested);
	fprintf(stderr, " %s\n", total_passed == total_tested ? GREEN_OK : RED_KO);
}
