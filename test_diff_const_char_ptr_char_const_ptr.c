#include <stdio.h>

/*
 * the difference btween [ const char * ] and [ char * const ]
 */
void foo(void)
{
    char c = 'A';
    char cc = 'B';

    /*
     * ccp is a pointer to a const char variable 
     * so the value pointed can't be modified but the pointer can be.
     * so dereferencing the pointer and update the write the value is forbiden
     * so in order to change the value we need to change the pointer value it self by pointing to an other valid address
     */
    const char *ccp = &c;
    // *ccp = 'V'; //is forbiden because the pointer is pointing to a const char data
    printf("%c\n", *ccp); // but it can be readed !
    ccp = &cc;
    printf("%c\n", *ccp); // but it can be readed !

    /*
     * cpc is a const pointer to a char variable 
     * so the value pointed can be modified but the pointer can't be.
     * so dereferencing the pointer and update the write the value is allowed
     * but change the pointer value it self is not denied
     */
    char * const cpc = &c;
    *cpc = 'H';
    printf("%c\n", *cpc);
    // cpc = &cc; // this is not allowed because the pointer is constant

}

void main(void)
{
    foo();
}