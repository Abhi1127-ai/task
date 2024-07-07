#include<stdio.h>
int main()
{
     int randomno=5555;
     int number;
     int attempt=0;
     printf("enter your number\n");
     scanf("%d",&number);
     while(number>0)
     {
          if(randomno!=number)
             {
                printf("enter your number\n");
                scanf("%d",&number);
             }
          else
             {
                printf("congratulation you guess the correct number in %d attempts.\n",attempt);
                break;
             }
     }
   return 0;

}