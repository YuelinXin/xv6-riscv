#include "kernel/types.h"
#include "kernel/param.h"

int main(int argc, char* argv[])
{
	if(argc > MAXARG){
		fprintf(stderr, "too many aarguements\n");
		return -1;
	}
	
	return 0;
}
