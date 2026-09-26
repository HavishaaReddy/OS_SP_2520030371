#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
char input[256];
char cwd[256];
char prev[256];

if(getcwd(cwd, sizeof(cwd)) == NULL) {
perror("getcwd");
exit(1);
}
prev[0] = '\0';

while(1) {
printf("shell:%s$ ", cwd);
if(fgets(input, sizeof(input), stdin) == NULL) break;
input[strcspn(input, "\n")] = '\0';

if(strncmp(input, "cd", 2) == 0) {
char *path = input+2;
while(*path == ' ') path++;
if(strcmp(path, "-") == 0 && prev[0] != '\0') {
if(chdir(prev) == 0) {
strcpy(prev, cwd);
getcwd(cwd, sizeof(cwd));
} else perror("cd");
} else if(strlen(path) == 0) {
char *home = getenv("HOME");
if(home && chdir(home) == 0) {
strcpy(prev, cwd);
getcwd(cwd, sizeof(cwd));
} else perror("cd");
} else {
if(chdir(path) == 0) {
strcpy(prev, cwd);
getcwd(cwd, sizeof(cwd));
} else perror("cd");
}
} else if(strcmp(input, "exit") == 0) {
break;
} else {
printf("Unknown command: %s\n", input);
}
}
return 0;
}
