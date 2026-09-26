#include <stdio.h>
#include <string.h>

int main() {
char input[256];
printf("Enter a string with escapes: ");
fgets(input, sizeof(input), stdin);
for(int i=0; input[i]!='\0'; i++) {
if(input[i]=='\\') {
i++;
switch(input[i]) {
case 'n': putchar('\n'); break;
case 't': putchar('\t'); break;
case '\\': putchar('\\'); break;
case '\'': putchar('\''); break;
case '\"': putchar('\"'); break;
default: putchar('\\'); putchar(input[i]); break;
}
} else {
putchar(input[i]);
}
}
return 0;
}
