#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 10
#define MAX_CMD 100

int main() {
char history[MAX_HISTORY][MAX_CMD];
char command[MAX_CMD];
int count = 0;

printf("Simple Command History\n");
printf("Type 'history' to display commands\n");
printf("Type 'exit' to quit\n\n");

while(1) {
printf("shell> ");
fgets(command, MAX_CMD, stdin);
command[strcspn(command, "\n")] = '\0';

if(strcmp(command, "exit") == 0)
break;

if(strcmp(command, "history") == 0) {
printf("\nCommand History:\n");
for(int i=0; i<count; i++) {
printf("%d %s\n", i+1, history[i]);
}
continue;
}

if(strlen(command) == 0)
continue;

if(count < MAX_HISTORY) {
strcpy(history[count], command);
count++;
} else {
for(int i=0; i<MAX_HISTORY-1; i++)
strcpy(history[i], history[i+1]);
strcpy(history[MAX_HISTORY-1], command);
}
}
return 0;
}
