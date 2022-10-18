#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    printf("The Unix version 6 was released in %d\n", getyear());
    return 0;
}