#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
char input[256];
char output[512];
printf("Enter a command: ");
fgets(input, sizeof(input), stdin);

int i=0, j=0;
while(input[i] != '\0') {
if(input[i] == '$') {
i++;
char var[128];
int k=0;
while(input[i] != '\0' && (isalnum(input[i]) || input[i]=='_')) {
var[k++] = input[i++];
}
var[k] = '\0';
char *val = getenv(var);
if(val) {
for(int m=0; val[m]!='\0'; m++) {
output[j++] = val[m];
}
}
} else {
output[j++] = input[i++];
}
}
output[j] = '\0';

printf("Expanded command: %s\n", output);
return 0;
}
