#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {


    FILE *file;
    char buffer[250];
    int line_count = 0;
    clock_t start, end;
    double total_time_passed;
    int max_line = 0, curr_line;

    file = fopen("data.tsv", "r");
    if (file == NULL) return 1;

    start = clock();
    while (fgets(buffer, 250, file)) {
        curr_line = strlen(buffer);
        if (curr_line > max_line) max_line = curr_line;
        line_count++;
    };
    end = clock();

    /* END - START give the total number of clocks that tick in a time period */
    /* CLOCKS_PER_SEC gives the total number of clocks per second */
    /* Which means that if we had 100 clocks and CLOCKS_PER_SEC = 200, total_time_passed = 0.5 */
    total_time_passed = ((double)(end - start) / CLOCKS_PER_SEC);

    printf("Max line: %d\n", max_line);
    printf("%d lines took %f\n", line_count, total_time_passed);

    

    /* FILE* fp;
    char filename[] = "data.tsv";
    char buffer[250];
    int line_count = 0;

    clock_t start_time, end_time;
    double elapsed_time;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    start_time = clock();

    while (fgets(buffer, 250, fp)) line_count++;

    fclose(fp);
    end_time = clock();
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Read %d lines in %.2f seconds.\n", line_count, elapsed_time);
    printf("The file %s contains %d lines.\n", filename, line_count);
    return 0;
    */
    return 0;   
}