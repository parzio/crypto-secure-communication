
#include "mylib.h"


int main(int argc, char ** argv){
	
	u_int8_t a = 3;
	u_int8_t b = 255;

	print(a);
	print(b);
	
	print(product(a , a , b , 6)); 
	
	return 0;
		
}