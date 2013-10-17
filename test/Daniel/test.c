
#include "mylib.h"


int main(int argc, char ** argv){
	
	u_int8_t a = BIT(6) | BIT(4) | BIT(2);
	u_int8_t b = BIT(6) | BIT(1) | BIT(0);
	
	u_int16_t field8 = BIT(8) | BIT(4) | BIT(3) | BIT(2) | BIT(0);

	print(a);
	print(b);
	print(field8);
	
	print(product(field8 , a , b , 8)); 
	
	return 0;
		
}