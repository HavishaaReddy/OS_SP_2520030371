#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
char input[256];
char cwd[256];

while(1) {
if(getcwd(cwd, sizeof(cwd)) == NULL) {
perror("getcwd");
exit(1);
}
printf("shell:%s$ ", cwd);

if(fgets(input, sizeof(input), stdin) == NULL) break;
input[strcspn(input, "\n")] = '\0';

if(strcmp(input, "pwd") == 0) {
printf("%s\n", cwd);
} else if(strcmp(input, "exit") == 0) {
printf("Exiting shell...\n");
break;
} else {
printf("Unknown command: %s\n", input);
}
}
return 0;
}
