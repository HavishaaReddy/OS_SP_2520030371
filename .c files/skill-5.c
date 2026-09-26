#include <stdio.h>

int main() {
char input[256];
printf("Enter a string: ");
fgets(input, sizeof(input), stdin);
printf("Preserved string: %s\n", input);
return 0;
}
