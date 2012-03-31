/*************************************************************************************************
 * BandRich Inc.
 * Copyright (c) 2010
 * All rights reserved
 *************************************************************************************************
 * Author: BandRich Inc.
 *
 *
 *
 ************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

unsigned char input_buffer[16*20];

int main(int argc, char *argv[])
{
	long i;
	size_t read_len = 0;
	FILE *f_input = NULL;
	FILE *f_output = stdout;
	
	/* show command help if the argument missing  */
	/* argv[1] : array name
	*  argv[2] : input file
	*  argv[3] : output file
	*/
	if ( argc < 3 )
	{
		printf("usage: bin2c <array name> <input file> <output file>\n");
		return 1;
	}
	else if ( argc >= 4 )
	{
		f_output = fopen(argv[3],"wb"); /* output file name */
		if (f_output==NULL) 
		{
			printf("Open %s error!",argv[3]); 
			f_output = stdout;
		}
	}

	f_input = fopen(argv[2],"rb"); /* input file name */
	if (f_input==NULL) 
	{
		if ( f_output != stdout ) fclose( f_output );

		printf("Open %s error!",argv[2]); 
		return 1;
	}

	fprintf(f_output,"char %s[]=\n{", argv[1] ); /* array name */

	while ( 1 ) 
	{
		read_len = fread (input_buffer,1,sizeof(input_buffer),f_input);
		for ( i = 0 ; i < read_len ; i++ )
		{
			if ( (i%16) == 0 )
			{
				fprintf(f_output,"\n");
			}

			fprintf(f_output,"0X%02X,", input_buffer[i]);
		}
		
		if ( read_len < sizeof(input_buffer) )
		{
			break;	
		}
	}
	fprintf(f_output,"\n};\n\n");

	fclose( f_input );

	if ( f_output != stdout ) fclose( f_output );
}

