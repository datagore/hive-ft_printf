#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ft_printf.h"

// ANSI escape codes.
#define ANSI_GREEN "\e[1;32m" // Set the text color to green.
#define ANSI_RED   "\e[1;31m" // Set the text color to red.
#define ANSI_RESET "\e[0m"    // Reset to default color.
#define ANSI_CLEAR "\e[2J"    // Clear the screen.

// Color-coded OK and KO strings.
#define GREEN_OK (ANSI_GREEN "[OK]" ANSI_RESET)
#define RED_KO   (ANSI_RED   "[KO]" ANSI_RESET)

// Run a test case.
#define TEST_CASE(...) do { \
		char a[1000] = {0}; \
		ftruncate(1, 0); \
		lseek(1, 0, SEEK_SET); \
		ft_printf(__VA_ARGS__); \
		lseek(1, 0, SEEK_SET); \
		int a_length = read(1, a, sizeof(a)); \
		char b[1000] = {0}; \
		ftruncate(1, 0); \
		lseek(1, 0, SEEK_SET); \
		printf(__VA_ARGS__); \
		fflush(stdout); \
		lseek(1, 0, SEEK_SET); \
		int b_length = read(1, b, sizeof(b)); \
		int match = a_length == b_length && memcmp(a, b, a_length) == 0; \
		fprintf(stderr, "%s ", match ? GREEN_OK : RED_KO); \
		fprintf(stderr, "printf(%s)\n", #__VA_ARGS__); \
		fprintf(stderr, "    ft_printf: %.*s\n", a_length, a); \
		fprintf(stderr, "    printf:    %.*s\n", b_length, b); \
		total_passed += !!match; \
		total_tested++; \
	} while (0)

// Keep a count of how many test cases passed or failed.
int total_passed;
int total_tested;

int main()
{
	// Clear the screen.
	printf(ANSI_CLEAR);
	fflush(stdout);

	// Open a temporary file and redirect standard output to that file.
	int temporary_file = open("temp", O_RDWR | O_CREAT, 0666);
	unlink("temp"); // Remove the file (it disappears the moment it's closed);
	dup2(temporary_file, 1); // All writes to 1 actually goes to temporary_file.

	// Run test cases (see macro above).
	TEST_CASE("");
	TEST_CASE("hello");
	TEST_CASE("world %c", '@');

	// Print a summary of all test cases.
	fprintf(stderr, "\n%d/%d tests passed", total_passed, total_tested);
	fprintf(stderr, " %s\n", total_passed == total_tested ? GREEN_OK : RED_KO);

	// Close the temporary file.
	close(temporary_file);
}
