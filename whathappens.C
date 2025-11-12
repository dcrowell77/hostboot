/*
 What is the value of b?
 What is the effect of the program?
*/

void main()
{
	int* b = 0;
	*(++b) = 5;
	printf("b is %d\n",b);
}
