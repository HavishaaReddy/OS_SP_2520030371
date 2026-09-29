#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
FILE *fp;
fp = fopen("missing.txt", "r");
if (fp == NULL) {
fprintf(stderr, "Runtime Error: %s\n", strerror(errno));
FILE *log = fopen("error.log", "a");
if (log != NULL) {
fprintf(log, "Error: Unable to open missing.txt\n");
fclose(log);
}
printf("Program recovered gracefully.\n");
return 0;
}
fclose(fp);
return 0;
}
