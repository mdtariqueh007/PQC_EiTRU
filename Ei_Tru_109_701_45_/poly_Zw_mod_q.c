#include "poly.h"
#include<stdbool.h>

Term poly_div_rat(Term A, Term q){ // q is of form (c + 0 w) where c is prime
    
    Term rem;
    Term REM;
    
   
    // int x = (A.a%(q.a) + q.a)%(q.a);
    // int y = (A.b%(q.a) + q.a)%(q.a);

    // printf("\nMod result  :%d\n",uint32_mod_uint14(1000,701));

    int x = uint32_mod_uint14(A.a,q.a);
    int y = uint32_mod_uint14(A.b,q.a);

    if(q.a==2){
        if(x==1 && y==1){
            rem.a = -1;
            rem.b = -1;
        }
        else{
            rem.a = x;
            rem.b = y;
        }

        return rem;
    }

    int X = 2 * x;
    int Y = 2 * y;

    // if((x+y)>(q.a) && X>y && Y>=x){
    //     REM.a = x - q.a;
    //     REM.b = y - q.a;
    // }
    // else if((X-y)>q.a && Y<x){
    //     REM.a = x - q.a;
    //     REM.b = y;
    // }
    // else if((Y-x)>=q.a && X<=y){
    //     REM.a = x;
    //     REM.b = y - q.a;
    // }
    // else{
    //     REM.a = x;
    //     REM.b = y;
    // } 

    // REM.a = (REM.a%(q.a) + q.a)%q.a;
    // REM.b = (REM.b%(q.a) + q.a)%q.a;

    // printf("REM : (%d + %dw)\n",REM.a,REM.b);

    bool t1 = (x+y)>(q.a) && X>y && Y>=x;
    bool t2 = (X-y)>q.a && Y<x;
    bool t3 = (Y-x)>=q.a && X<=y;

    rem.a = x - (int)((t1 || t2) && !(t3))*q.a;
    rem.b = y - (int)((t1 || t3) && !(t2))*q.a;

    rem.a += q.a;
    rem.b += q.a;

    rem.a = uint32_mod_uint14(rem.a,q.a);
    rem.b = uint32_mod_uint14(rem.b,q.a);

    printf("rem: (%d + %dw)\n",rem.a,rem.b);
    return rem;

}

void poly_Zw_mod_q(const Term* a, Term q, Term* r){
    
    for(int i = 0;i<N;i++){
        r[i] = poly_div_rat(a[i],q);
    }
}
void ZwCnC3_mod_q(const Term* a, Term q, Term* r){
    
    for(int i = 0;i<N3;i++){
        r[i] = poly_div_rat(a[i],q);
    }
}
