#include "operations.h"

u_int16_t summation(u_int16_t a,u_int16_t b)
{
    return (a ^ b);
}

u_int8_t product(u_int8_t a,u_int8_t b,u_int8_t p,u_int8_t n)
{
    u_int8_t mask = 1;
    int i;
    u_int16_t result=0;
    for(i=0;i<n;i++)
    {
      result = result + ((((mask<<i)*a)*b)<<i);
    }
    printf(" first multiplier %d second multiplier %d result %d \n",a,b,result);
    return (u_int8_t)result;
}

u_int8_t divide(u_int16_t poly1,u_int8_t poly2)
{
    while(1)
    {
      u_int16_t mask = 32768;
      int diff;
      int i=0;
      int j=0;
      while(1)
      {
	if(((mask>>i)&(poly1)) != 0 || i>= 16)break;
	i++;
      }
      while(1)
      {
	if(((mask>>j)&((u_int16_t)poly2)) != 0)break;
	j++;
      }
    diff = j-i;
    if(diff<0)break;
    poly1=poly1^(((u_int16_t)poly2)<<diff);
    }
    printf("Result : %d \n",(u_int8_t)poly1);
    return (u_int8_t)poly1;
    
}