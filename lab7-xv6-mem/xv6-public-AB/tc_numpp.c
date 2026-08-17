#include "user.h"

int main()
{
    printf(1, "Virtual Address size: %d\n", numvp());
    printf(1, "Physical Address size: %d\n", numpp());
    exit();
}