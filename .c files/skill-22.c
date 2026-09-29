#include <stdio.h>
#include <stdlib.h>

int main() {
int *numbers;
numbers = malloc(5 * sizeof(int));
if (numbers == NULL) {
printf("Memory allocation failed.\n");
return 1;
}
for (int i = 0; i < 5; i++)
numbers[i] = i + 1;
printf("Numbers:\n");
for (int i = 0; i < 5; i++)
printf("%d ", numbers[i]);
printf("\n");
free(numbers);
printf("Memory released successfully.\n");
return 0;
}
