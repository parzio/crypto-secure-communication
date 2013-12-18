#include "StreamChiper/toy.h"
#include "PRNG/PRNG.h"
int main(int argc, char ** argv){

	if(argc < 2)
	{
		printf("\n\nError!!! usage : ./correlation [known bit num]\n\n");
		return 0;
	}
	
	/*
	 *	int stream_length = 256;
	 *	bit plain_stream[stream_length];
	 * hexToBianry(plain_stream , hex);	//hex to binary
	 * int c_length = 10000;
	 * bit ciphertext[c_length];
	 * hexToBianry(ciphertext, hex);
	 */
	
	
	int stream_length = atoi(argv[1]);
	bit plain_stream[stream_length];
	
	TOY toy;
	TOY_init(&toy);
	TOY_print_intial_state(&toy);
	TOY_getStream(&toy , plain_stream , stream_length);

	bit reg_5_test_state[8];
	bit reg_5_test_stream[stream_length];
	
	bit reg_4_test_state[7];
	bit reg_4_test_stream[stream_length];

	bit reg_3_test_state[4];
	bit reg_3_test_stream[stream_length];

	bit reg_2_test_state[3];
	bit reg_2_test_stream[stream_length];

	bit reg_1_test_state[2];
	bit reg_1_test_stream[stream_length];	
	
	int z; //variable used to move from a number to a state
	int find = 0;	
	int reg_5_state_index = 1;	
	do
	{
		for(z = 0; z < 8; z++)
			reg_5_test_state[z] = (reg_5_state_index & (1 << z)) ? 1 : 0;	//to state		

		LFSR reg_lfsr_5 = {NULL , NULL , 0};
		LFSR_init(&reg_lfsr_5 , reg_5_test_state , TOY_poly5 , 8);
		LFSR_getStream(&reg_lfsr_5 , reg_5_test_stream , stream_length);	//get stream
		
		int count_plain_zero = 0;
		int count_reg5_zero = 0;
		int j = 0 , exit = -1;

		do{
			if(plain_stream[j] == 1 && reg_5_test_stream[j] != 1)	//if plain[j] == 1 also reg5_out[j] must be 1
				exit = 1;
			else
				j++;
			if(plain_stream[j] == 0)
				count_plain_zero ++;
			
			if(reg_5_test_stream[j] == 0)
				count_reg5_zero ++;
			
		}while(exit != 1 && j < stream_length);
		
		if(j == stream_length && (count_reg5_zero > count_plain_zero/2))	//at last 1/2 of the zero must be in common
		{
			printf("Reg 5 test state  : ");
			printArray(reg_5_test_state , 8);	
			int reg_4_state_index = 1;	
			do			
			{
				for(z = 0; z < 7; z++)
					reg_4_test_state[z] = (reg_4_state_index & (1 << z)) ? 1 : 0;	//to state		

				LFSR reg_lfsr_4 = {NULL , NULL , 0};
				LFSR_init(&reg_lfsr_4 , reg_4_test_state , TOY_poly4 , 7);
				LFSR_getStream(&reg_lfsr_4 , reg_4_test_stream , stream_length);	//get stream
			
				exit = -1; j = 0;
				int diff = 0;
				
				do{
					if(plain_stream[j] == 0 && reg_5_test_stream[j] == 1 && reg_4_test_stream[j] != 1)	
						//if plain[j] == 0 , reg5[j] == 1 , reg4[j] must be 1
						exit = 1;
					else
						if(plain_stream[j] != reg_4_test_stream[j])
							diff ++;
						j++;
				}while(exit != 1 && j < stream_length);
				
				if(j == stream_length && diff >= (stream_length / 2))	//if end
				{
					printf("Reg 4 test state  : \t ");
					printArray(reg_4_test_state , 7);
					int reg_3_state_index = 1;					
					do
					{	
						for(z = 0; z < 4; z++)
							reg_3_test_state[z] = (reg_3_state_index & (1 << z)) ? 1 : 0;	//to state			
					
						LFSR reg_lfsr_3 = {NULL , NULL , 0};
						LFSR_init(&reg_lfsr_3 , reg_3_test_state , TOY_poly3 , 4);
						LFSR_getStream(&reg_lfsr_3 , reg_3_test_stream , stream_length);	//get stream

						exit = -1; j = 0;
						
						do{
							if(plain_stream[j] == 1 && reg_4_test_stream[j] == 1 && reg_3_test_stream[j] != 1)	
							exit = 1;
						else
							j++;
						}while(exit != 1 && j < stream_length);	
						
						if(j == stream_length)	//if end
						{
							
							printf("Reg 3 test state  : \t\t ");
							printArray(reg_3_test_state , 4);
							int reg_2_state_index = 1;
							do
							{	
								for(z = 0; z < 3; z++)
									reg_2_test_state[z] = (reg_2_state_index & (1 << z)) ? 1 : 0;	//to state			

								LFSR reg_lfsr_2 = {NULL , NULL , 0};
								LFSR_init(&reg_lfsr_2 , reg_2_test_state , TOY_poly2 , 3);
								LFSR_getStream(&reg_lfsr_2 , reg_2_test_stream , stream_length);	//get stream
								exit = -1; j = 0;
						
								do{
									if(plain_stream[j] == 1 && reg_4_test_stream[j] == 1 && reg_2_test_stream[j] != 1)	
										exit = 1;
									else
										j++;
								}while(exit != 1 && j < stream_length);	
							
								if(j == stream_length)	//if end
								{				
									printf("Reg 2 test state  : \t\t\t ");
									printArray(reg_2_test_state , 3);
									int reg_1_state_index = 1;
									do
									{	
										for(z = 0; z < 2; z++)
											reg_1_test_state[z] = (reg_1_state_index & (1 << z)) ? 1 : 0;	//to state			
										
										LFSR reg_lfsr_1 = {NULL , NULL , 0};
										LFSR_init(&reg_lfsr_1 , reg_1_test_state , TOY_poly1 , 2);
										LFSR_getStream(&reg_lfsr_1 , reg_1_test_stream , stream_length);	//get stream_length

										bit test_stream[stream_length];
										
										int p;
										for(p = 0; p < stream_length; p++)
											test_stream[p] = (reg_1_test_stream[p] & reg_2_test_stream[p] & reg_3_test_stream[p] & 
											reg_4_test_stream[p] & reg_5_test_stream[p]) ^ 
											(reg_4_test_stream[p] & reg_5_test_stream[p]) ^ reg_5_test_stream[p];

										if(memcmp(plain_stream , test_stream , stream_length * sizeof(bit)) == 0)
										{
											printf("Reg 1 test state  : \t\t\t\t ");
											printArray(reg_1_test_state , 2);
											find = 1;
										}	
										LFSR_delete(&reg_lfsr_1);
										reg_1_state_index++;
									}while(reg_1_state_index < 4 && find == 0);	
								}
								LFSR_delete(&reg_lfsr_2);
								reg_2_state_index++;
							}while(reg_2_state_index < 8 && find == 0);			
						}
						LFSR_delete(&reg_lfsr_3);
						reg_3_state_index++;
					}while(reg_3_state_index < 16 && find == 0);
				}			
				LFSR_delete(&reg_lfsr_4);
				reg_4_state_index++;
			}while(reg_4_state_index < 128 && find == 0);				
		}			
		LFSR_delete(&reg_lfsr_5);
		reg_5_state_index++;
	}while(reg_5_state_index < 256 && find == 0);
	
	
	
	TOY found;
	
	TOY_init_with_reg(&found , reg_1_test_state , reg_2_test_state , reg_3_test_state , reg_4_test_state , reg_5_test_state);
	TOY_print_intial_state(&found);
	
	bit newStream[stream_length];
	TOY_getStream(&found , newStream , stream_length);
	
	TOY_getStream(&toy ,   plain_stream , stream_length);
	TOY_getStream(&found , newStream , stream_length);
	
	printf("Toy stream  bit[%d - %d] : " , stream_length , stream_length * 2);
	printArray(plain_stream , stream_length);
	printf("My stream   bit[%d - %d] : " , stream_length , stream_length * 2);
	printArray(newStream , stream_length);

	int num_equal = 0 , i;
	for(i = 0; i < stream_length; i++)
		num_equal += (plain_stream[i] == newStream[i]) ? 1 : 0;
	
	printf("Number of equal bit %d / %d \n" , num_equal , stream_length);
	
	
}