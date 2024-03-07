/* typewrite.c--program to read characters from a file and write them to the ter
**              minal screen as if typed.
*/
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

// Function to print usage information
void print_usage(const char *program_name) {
    printf("Usage: %s [option1] [option2] ...\n", program_name);
    printf("Options:\n");
    printf(" -h, --help     Display this help message and exit\n");
    printf(" -v, --version Display version information and exit\n");
    printf(" -f, --file     Specify a file to process\n");
}

int typewrite(char filename[100]) {
    // Set the locale to support wide characters
    setlocale(LC_ALL, "");

    FILE *fd;  // File descriptor

    // 3. Open the file for reading
    fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    // Set the file to be read in wide character mode
    fwide(fd, 1);

    // 4. While there are still characters to be read from the input file
    wchar_t ch;

    //  a. read the character from the file and
    while ((ch = fgetwc(fd)) != WEOF) {
    //  b. write the character to the terminal screen
        // Color code output: https://iq.opengenus.org/print-text-in-color-in-c
        // Add sound to the output. SO:/questions/3127977/
        wprintf(L"\033[1;32m%lc\a", ch);
        fflush(stdout);
        if (ch == ',') {
            usleep(500000);  // .5 sec pause on comma
        } else
        if (ch == '.') {
            usleep(1000000);  // 1 sec pause on period
        } else {
            usleep(30000);
        }
    }

    // 5. Close the input file
    fclose(fd);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    // Check if no arguments were provided
    if (argc == 1) {
        printf("No arguments provided.\n");
        print_usage(argv[0]);
        return 1;
    }

    // Loop through each argument
    for (int i = 1; i < argc; i++) {
        // Check for help option
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        // Check for version option
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("Version 1.0\n");
            return 0;
        }
        // Check for file option
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            // Ensure there's an argument following the option
            if (i + 1 < argc) {
                ++i;
                printf("Processing file: %s\n", argv[i]);
                // Here you can add code to process the file
                typewrite(argv[i]);
            } else {
                printf("Option -f requires an argument.\n");
                return 1;
            }
        }
        else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    // If no errors, program execution continues here
    printf("\n");
    printf("Program completed successfully.\n");

    return EXIT_SUCCESS;
}
