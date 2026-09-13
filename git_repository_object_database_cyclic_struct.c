#include <stdlib.h>
#include <stdio.h>

/*
 * this eg to show how the repository struct  can point to object_database struct and vice-versa
 * in this case the primary_struct can point to the secondary_struct and the secondary_struct can point to the 
 * primary struct that owns that secondary struct variable.
 * 
 * at the fist this seems strange, like the egg and the chicken paradox, but if we make a look on how it is managed 
 * it bacame reasonable.
 */


struct primary_struct {
    int *some_rand_data;
    char *title;
    struct secondary_struct *sec_str;
};

struct secondary_struct {
    struct primary_struct *prm_struct;
    int *var;
    int* my_role_num;
};

/**
 * this is a function that create the secondary variable (something like a constructor in oop languages)
 * so the overhead of reposoitiry objedct data_base, is delegated to the function odb_new, in our case new_second
 * then when initializing the repo  we only point to the object database by calling that function that returns a ready/full database 
 */
struct secondary_struct *new_second(struct primary_struct *prime) {
    struct secondary_struct *s;
    int a = 1;
    int b = 2;
    s = (struct secondary_struct *)calloc(1, sizeof(*(s)));
    s->prm_struct = prime;
    s->var = &a;
    s->my_role_num = &b;

    return s;
}


int primeCreate(struct primary_struct *prime) {
    prime->sec_str = new_second(prime);
    return 0;
}

int main(void) {
    /* finally we create a primary variable (repository), no overhead to handle the secondary data (object_database) */
    struct primary_struct *pr;
    pr = (struct primary_struct *)calloc(1, sizeof(*(pr)));
    if(0 == primeCreate(pr)) {
        printf("this works like I would !\n");
    }else {
        printf("Nope ur a looser :/\n");
    }
    free(pr);
    return 0;
}