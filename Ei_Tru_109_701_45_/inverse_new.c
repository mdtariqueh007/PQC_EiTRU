// #include "poly_r.h"
// #include "poly.h"

// #include <stdio.h>

// #define endl printf("\n----------------------------------------------\n");

#include "inverse.h"



Term zero = {0,0};

int deg(Term *a,int len){
    int n = len;

    

    for(int i = n - 1;i>=0;i--){
        if(isSame(a[i],zero)==0){
            return i;
        }
    }
}

int is_degree_one(Term* a,int len){
    for(int i = 1;i<len;i++){
        if(isSame(a[i],zero)==0){
            return 0;
        }
    }

    if(isSame(a[0],zero)==0){
        return 1;
    }

    return 0;

}


void division_by_x(Term* a, int len){
    for(int i = 0;i<len-1;i++){
        a[i] = a[i+1];
    }

    a[len-1] = zero;
    
}

void multiply_by_x(Term* a, int len){
    for(int i = len-1;i>0;i--){
        a[i] = a[i-1];
    }

    a[0] = zero;
}

Term unit(Term u){
    Term res;
    if(u.a==1 && u.b==0){
        res.a = 1;
        res.b = 0;
    }
    if(u.a==-1 && u.b==0){
        res.a = -1;
        res.b = 0;
    }
    if(u.a==0 && u.b==1){
        res.a = -1;
        res.b = -1;
    }
    if(u.a==0 && u.b==-1){
        res.a = 1;
        res.b = 1;
    }
    if(u.a==1 && u.b==1){
        res.a = 0;
        res.b = -1;
    }
    if(u.a==-1 && u.b==-1){
        res.a = 0;
        res.b = 1;
    }
    
    return res;
}

Term minus(Term A, Term B){
    Term res;

    res.a = A.a - B.a;
    res.b = A.b - B.b;

    return res;
}

void minus_Zw(Term* a, Term *b, Term mod, Term* res,int len){
    for(int i = 0;i<len;i++){
        res[i] = poly_div_rat(minus(a[i],b[i]),mod);
    }
}

int round1(int c,int d){
    int c1 = c%d;
    if(c1>d/2){
        c1 = c1 - d;
    }
    int res = (c-c1)/d;

    return res;
}

int norm_Zw(Term element){
    int a = element.a;
    int b = element.b;

    int d = a*a + b*b - a*b;

    return d;
}


void red_q_etru(Term element,Term q, Term *res){
    int m = element.a;
    int n = element.b;
    int a = q.a;
    int b = q.b;

    int Q_ = norm_Zw(q);

    int d = 2 * Q_;

    int s = m*(2*a-b)+n*(2*b-a);
    int t = n*a-m*b;

    int re = s/d;
    int im = t/d;

    int x0 = round1(s,d);
    int x1 = round1(t,d);

    Term r1 = {x0+x1,2*x1};

    int s1 = s+Q_;
    int t1 = t-Q_;

    int y0 = round1(s1,d);
    int y1 = round1(t1,d);

    Term r2 = {y0+y1,2*y1+1};

    Term b1 = minus(element,multiply_terms(q,r1));
    Term b2 = minus(element,multiply_terms(q,r2));

    int d1 = norm_Zw(b1);
    int d2 = norm_Zw(b2);

    if(d1<d2){
        res[0] = r1;
        res[1] = b1;
    }
    else if(d1>d2){
        res[0] = r2;
        res[1] = b2;
    }
    else if(x0<y0){
        res[0] = r1;
        res[1] = b1;
    }
    else{
        res[0] = r2;
        res[1] = b2;
    }

}


long long powerMod(long long x, long long n, long long mod)
{
    long long res = 1;
    while (n > 0)
    {
        if (n & 1)
        {
            res = (res * x) % mod;
        }
        x = (x * x) % mod;
        n = n >> 1;
    }

    return res;
}

Term inverse_zq(int num,int q){
    long long inv = powerMod(num,q-2,q);

    Term INV = {inv,0};

    // Term res;
    Term q_ = {q,0};

    // printf("\n(%d + %d w)\n",INV.a,INV.b);

    Term res;

    res = poly_div_rat(INV,q_);

    // printf("\n(%d + %d w)\n",res->a,res->b);
    return res;
}

Term inverse_zw(Term A,Term q){
    int a = A.a;
    int b = A.b;

    int temp = a * a + b * b - a * b;

    Term r;

    r = inverse_zq(temp,q.a);

    Term res_temp;

    res_temp.a = a - b;
    res_temp.b = -b;

    res_temp.a *= r.a;
    res_temp.b *= r.a;

    Term res;

    res = poly_div_rat(res_temp,q);

    // printf("\nInverse in zw:\n");

    // printf("\n(%d + %d w)\n",res.a,res.b);

    return res;



}

//////////// New Almost Inverse Algorithm ////////////////////////

static int int16_nonzero_mask(Term X)
{

    int16 x = X.a;
  uint16 u = x; /* 0, else 1...65535 */
  uint32 v = u; /* 0, else 1...65535 */
  v = -v; /* 0, else 2^32-65535...2^32-1 */
  v >>= 31; /* 0, else 1 */


    int16 y = X.b;
    uint16 u_ = y; /* 0, else 1...65535 */
  uint32 v_ = u_; /* 0, else 1...65535 */
  v_ = -v_; /* 0, else 2^32-65535...2^32-1 */
  v_ >>= 31; /* 0, else 1 */

  if(v==0 && v_==0){
    return 0;
  }
  else{
    return -1;
  }
    

//   return -v; /* 0, else -1 */
}


/* return -1 if x<0; otherwise return 0 */
static int int16_negative_mask(int16 x)
{
  uint16 u = x;
  u >>= 15;
  return -(int) u;
  /* alternative with gcc -fwrapv: */
  /* x>>15 compiles to CPU's arithmetic right shift */
}


int newInverseFunction(Term *out,Term *in)
{ 
  Term f[N+1],g[N+1],v[N+1],r[N+1];
  int i,loop,delta;
  int swap,t;
  Term f0,g0;
  Term scale;

  int sign_a, sign_b,t_a, t_b;
  Term q_ = {Q,0};

  for (i = 0;i < N+1;++i){ 
    v[i].a = 0;
    v[i].b = 0;

    r[i].a = 0;
    r[i].b = 0;
  }
 
//   r[0] = Fq_recip(3);

    r[0].a = 1;
    r[0].b = 0;


  for (i = 0;i < N;++i){ 
    f[i].a = 0;
    f[i].b = 0;

    // g[i].a = 0;
    // g[i].b = 0;
  }
  f[0].a = 1; 
  f[0].b = 0;
  f[N-1].a = -1; 
  f[N-1].b = 0;
  f[N].a = -1;
  f[N].b = 0;
  for (i = 0;i < N;++i){ 
    g[N-1-i] = in[i];
  }
  g[N].a = 0;
  g[N].b = 0;

  delta = 1;

  for (loop = 0;loop < 2*N-1;++loop) {
    for (i = N;i > 0;--i){
        v[i] = v[i-1];
    }
    v[0].a = 0;
    v[0].b = 0;


    
    swap = int16_negative_mask(-delta) & int16_nonzero_mask(g[0]);
    delta ^= swap&(delta^-delta);
    delta += 1;

    for (i = 0;i < N+1;++i) {
        t_a = swap & (f[i].a ^ g[i].a); f[i].a ^= t_a; g[i].a ^= t_a;
        t_b = swap & (f[i].b ^ g[i].b); f[i].b ^= t_b; g[i].b ^= t_b;
        t_a = swap & (v[i].a ^ r[i].a); v[i].a ^= t_a; r[i].a ^= t_a;
        t_b = swap & (v[i].b ^ r[i].b); v[i].b ^= t_b; r[i].b ^= t_b;
    }

    f0 = f[0];
    g0 = g[0];
    
    for (i = 0;i < N+1;++i){
        // Term temp;
        // temp.a = f0*g[i].a - g0*f[i].a;
        // temp.b = f0*g[i].b - g0*f[i].b;
        Term temp1 = multiply_terms(f0,g[i]);
        Term temp2 = multiply_terms(g0,f[i]);

        Term temp;

        temp.a = temp1.a - temp2.a;
        temp.b = temp1.b - temp2.b;
        g[i] = poly_div_rat(temp,q_); // use red_q
    }
    for (i = 0;i < N+1;++i){
        // Term temp;
        // temp.a = f0*r[i].a - g0*v[i].a;
        // temp.b = f0*r[i].b - g0*v[i].b;
        Term temp1 = multiply_terms(f0,r[i]);
        Term temp2 = multiply_terms(g0,v[i]);

        Term temp;
        temp.a = temp1.a - temp2.a;
        temp.b = temp1.b - temp.b;
        r[i] = poly_div_rat(temp,q_); // use red_q
    }

    for (i = 0;i < N;++i) g[i] = g[i+1];
    g[N].a = 0;
    g[N].b = 0;
  }

  scale = inverse_zw(f[0],q_);//use find_inverse_new
  for (i = 0;i < N;++i){
    Term temp = multiply_terms(scale,v[N-i-1]);
    out[i] = poly_div_rat(temp,q_); // use red_q
  }

  Term DELTA = {delta,0};

  return int16_nonzero_mask(DELTA);
}

bool checkInverse(Term* out, Term* in){
    Term mul_res[N];
    Term q_ = {Q,0};
    poly_Zw_mul_mod_q(out,in,q_,mul_res);

    printf("\nmul: \n");

    for(int i = 0;i<N;i++){
        printf("(%d + %dw) ",mul_res[i].a,mul_res[i].b);
    }

    printf("\n");

    if(mul_res[0].a!=1 || mul_res[0].b!=0){
        return false;
    }

    for(int i = 1;i<N;i++){
        if(mul_res[i].a!=0 || mul_res[i].b!=0){
            return false;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////

void egcd_etru(Term A,Term q,Term* r){
    Term one = {1,0};

    Term r2 = A;
    Term r1 = q;

    if(isSame(A,zero) && isSame(q,zero)){
        r[0] = zero;
        r[1] = zero;
        r[2] = zero;
        return;
    }

    Term u0 = zero;
    Term u1 = zero;
    Term u2 = one;

    while(isSame(r1,zero)==0){
        Term res[2];
        red_q_etru(r2,r1,res);

        Term q0 = res[0];
        Term r0 = res[1];

        r2 = r1;
        r1 = r0;
        u0 = minus(u2,multiply_terms(q0,u1));

        u2 = u1;
        u1 = u0;
    }

    Term u = u2;
    Term v;

    if(isSame(q,zero)){
        v = zero;
    }
    else{
        Term res[2];
        red_q_etru(minus(r2,multiply_terms(u,A)),q,res);
        v = res[0];
    }

    Term res[2];

    red_q_etru(r2,q,res);
    
    Term d = res[1];

    Term inv_d = unit(d);

    red_q_etru(multiply_terms(u,inv_d),q,res);

    u = res[1];

    r[0] = u;
    r[1] = v;
    r[2] = d;
}

Term find_inverse(Term A,Term mod){
    Term res[2];

    egcd_etru(A,mod,res);

    // printf("find_inverse : (%d + %d w) (%d + %d w) + (%d + %d w)\n",res[0].a,res[0].b,res[1].a,res[1].b, res[2].a,res[2].b);
    // printf("find_inverse : (%d + %d w)\n",res[0].a,res[0].b);

    return res[0];
}

void multiply_by_constant(Term constant, Term* a,Term *res,int len,Term q){
    for(int i = 0;i<len;i++){
        res[i] = poly_div_rat(multiply_terms(a[i],constant),q);
    }
}


void reverse(int i,int j,Term* a){
    while(i<=j){
        Term temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        i++;
        j--;
    }
}

void shiftRight_k(Term *arr, int k,int len){
    
    k = (k%len + len)%len;
    reverse(0,len-1,arr);
    reverse(0,k-1,arr);
    reverse(k,len-1,arr);

    // if(isSame(arr[len-1],zero)==0){
    //     arr[0] = arr[0] + arr[len-1];
    //     arr[len-1] = zero;
    // }
}

void swapArrays(Term* a,Term* b,int len){
    for(int i = 0;i<len;i++){
        Term temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}



int Almost_inverse_ZpwCn(Term *a, int mod,Term* a_inv,int n){
    Term q = {mod,0};

    int allZero = 1;

    for(int i = 0;i<n;i++){
        if(a[i].a!=0 || a[i].b!=0){
            allZero = 0;
            break;
        }
    }

    if(allZero){
        printf("Inverse do not exists\n");

        return 0;
    }

    int upper_bound = n + deg(a,n);

    // printf("%d\n",upper_bound);
    // printf("%d\n",deg(a,N3));



    int counter = 0;

    Term f[n+1];

    for(int i = 0;i<n;i++){
        f[i] = a[i];
    }

    f[n] = zero;

    // poly_Zw_print(f,N3+1);

    int k = 0;

    Term b[n+1];

    b[0].a = 1;
    b[0].b = 0;

    for(int i = 1;i<=n;i++){
        b[i] = zero;
    }

    // poly_Zw_print(b,N3+1);

    Term c[n+1];

    for(int i = 0;i<=n;i++){
        c[i] = zero;
    }

    // poly_Zw_print(c,N3+1);

    Term g[n+1];

    for(int i = 0;i<=n;i++){
        g[i] = zero;
    }

    g[0].a = -1; g[0].b = 0;
    g[n].a = 1 ; g[n].b = 0;



    // poly_Zw_print(g,N3+1);

    int flag = 1;

    

    while(flag){

        // printf("while : %d\n",isSame(poly_div_rat(f[0],q),zero));
        // c_++;
        // printf("Before loop\n");
        // printf("Result of check: %d\n",isSame(poly_div_rat(f[0],q),zero));
        while(isSame(poly_div_rat(f[0],q),zero)){
            // printf("\n************************************\n");
            // printf("Inside almost inverse loop\n");
            // poly_Zw_print(f,N3+1);
            division_by_x(f,n+1);
            // printf("\n-----------------------------------------------------\n");
            // poly_Zw_print(f,N3+1);
            
            counter++;
            // printf("Counter : %d\n",counter);
            // printf("\n-----------------------------------------------------\n"); 
            
            if(counter==upper_bound){
                printf("Inverse does not exists.\n");

                return 0;
            }
            multiply_by_x(c,n+1);
            k++;
            
        }

        

        // printf("is_deg_one: %d\n",is_degree_one(f,N3+1));

        if(is_degree_one(f,n+1)){
            flag = 0;
            Term f0_inv = find_inverse(f[0],q);

            // printf("f0_inv : ( %d  + %d w)\n",f0_inv.a,f0_inv.b);
            
            multiply_by_constant(f0_inv,b,b,n+1,q);
            // printf("f0_inv * b(x):  ");
            // poly_Zw_print(b,N3+1);
            for(int i = 0;i<n;i++){
                a_inv[i] = b[i];
            }

            // printf("a_inv:          ");

            // poly_Zw_print(a_inv,N3+1);

            shiftRight_k(a_inv,n-k,n);// change the a_inv size to N3 from N3+1

            // printf("a_inv:          ");


            // poly_Zw_print(a_inv, N3);

            return 1;
        }

        // printf("%d\n",deg(f,N3+1)<deg(g,N3+1));

        if(deg(f,n+1)<deg(g,n+1)){
            swapArrays(f,g,n+1);//swap can be optimized using pointers
            swapArrays(b,c,n+1);
        }

        Term g0_inv = find_inverse(g[0],q);

        // printf("( %d + %d w) ",g[0].a,g[0].b);

        // printf("( %d + %d w)\n",g0_inv.a,g0_inv.b);

        Term u = poly_div_rat(multiply_terms(f[0],g0_inv),q);

        // printf("u :( %d + %d w)\n",u.a,u.b);

        Term ug[n+1];

        multiply_by_constant(u,g,ug,n+1,q);
        // printf("u*g(x): ");
        // poly_Zw_print(ug,N3+1);
        // // break;
        minus_Zw(f,ug,q,f,n+1);

        // printf("f(x) - u*g(x) : ");
        // poly_Zw_print(f,N3+1);

        Term uc[n+1];

        multiply_by_constant(u,c,uc,n+1,q);
        // printf("u*c(x): ");
        // poly_Zw_print(uc,N3+1);
        minus_Zw(b,uc,q,b,n+1);


        // printf("b(x) - u*c(x): ");
        // poly_Zw_print(b,N3+1);

    }
    
}

int Almost_inverse_ZpwCnC3(Term *f,int mod,Term *res){

    int t = T;
    int t2 = t*t;
    Term q = {mod,0};

    group_ring F;

    F.a = (Term*)malloc(N*sizeof(Term));
    F.b = (Term*)malloc(N*sizeof(Term));
    F.c = (Term*)malloc(N*sizeof(Term));

    poly_to_CnC3(f,&F);

    Term u[N];
    Term v[N];
    Term w[N];

    for(int i = 0;i<N;i++){
        u[i] = F.a[i];
        v[i] = F.b[i];
        w[i] = F.c[i]; 
    }

    // printf("u: ");poly_Zw_print(u,N);
    // printf("v: ");poly_Zw_print(v,N);
    // printf("w: ");poly_Zw_print(w,N);

    Term ut[N];
    Term vt[N];
    Term wt[N];

    poly_Zw_shift_t(u,t,ut);
    poly_Zw_shift_t(v,t,vt);
    poly_Zw_shift_t(w,t,wt);

    // printf("ut: ");poly_Zw_print(ut,N);
    // printf("vt: ");poly_Zw_print(vt,N);
    // printf("wt: ");poly_Zw_print(wt,N);

    Term ut2[N];
    Term vt2[N];
    Term wt2[N];

    poly_Zw_shift_t(u,t2,ut2);
    poly_Zw_shift_t(v,t2,vt2);
    poly_Zw_shift_t(w,t2,wt2);

    // printf("ut2: ");poly_Zw_print(ut2,N);
    // printf("vt2: ");poly_Zw_print(vt2,N);
    // printf("wt2: ");poly_Zw_print(wt2,N);

    Term utut2[N];
    Term wwt2[N];
    Term vvt[N];
    Term vtwt2[N];
    Term vut2[N];
    Term wut[N];

    poly_Zw_mul_mod_q(ut,ut2,q,utut2);
    poly_Zw_mul_mod_q(w,wt2,q,wwt2);
    poly_Zw_mul_mod_q(v,vt,q,vvt);
    poly_Zw_mul_mod_q(vt,wt2,q,vtwt2);
    poly_Zw_mul_mod_q(v,ut2,q,vut2);
    poly_Zw_mul_mod_q(w,ut,q,wut);

    // printf("utut2: ");poly_Zw_print(utut2,N);
    // printf("wwt2: ");poly_Zw_print(wwt2,N);
    // printf("vvt: ");poly_Zw_print(vvt,N);
    // printf("vtwt2: ");poly_Zw_print(vtwt2,N);
    // printf("vut2: ");poly_Zw_print(vut2,N);
    // printf("wut: ");poly_Zw_print(wut,N);
    // endl

    Term U[N];
    Term V[N];
    Term W[N];

    poly_Zw_mul_mod_q(u,utut2,q,U);
    poly_Zw_mul_mod_q(vvt,vt2,q,V);
    poly_Zw_mul_mod_q(wwt2,wt,q,W);

    Term UVW[N];
    Term VWU[N];
    Term WUV[N];

    poly_Zw_mul_mod_q(u,vtwt2,q,UVW);
    poly_Zw_mul_mod_q(vut2,wt,q,VWU);
    poly_Zw_mul_mod_q(wut,vt2,q,WUV);

    // printf("UVW: ");poly_Zw_print(UVW,N);
    // printf("VWU: ");poly_Zw_print(VWU,N);
    // printf("WUV: ");poly_Zw_print(WUV,N);

    Term first[N];
    Term second[N];

    poly_Zw_add3(U,V,W,first);//
    poly_Zw_mod_q(first,q,first);

    poly_Zw_add3(UVW,VWU,WUV,second);
    poly_Zw_mod_q(second,q,second);

    // printf("first: ");poly_Zw_print(first,N);
    // printf("second: ");poly_Zw_print(second,N);
    // endl
   

    Term final[N];

    minus_Zw(first,second,q,final,N);

    
    // printf("final : ");poly_Zw_print(final,N);
    // endl

    Term one[N];
    Term two[N];
    Term three[N];

    minus_Zw(utut2,vtwt2,q,one,N);
    minus_Zw(wwt2,vut2,q,two,N);
    minus_Zw(vvt,wut,q,three,N);

    Term final_inv[N];
    int found = 0;

    

    // found = Almost_inverse_ZpwCn(final,q.a,final_inv,N); /// inverse function is being called here

    found = newInverseFunction(final_inv,final);

    // printf("final_inv: ");poly_Zw_print(final_inv,N);
    // endl

    

    // if(found==0){
    //     printf("Inverse does not exists in CnC3\n");
    //     return 0;
    // }

    Term a[N];
    Term b[N];
    Term c[N];

    poly_Zw_mul_mod_q(final_inv,one,q,a);
    poly_Zw_mul_mod_q(final_inv,two,q,b);
    poly_Zw_mul_mod_q(final_inv,three,q,c);

    // printf("a: ");poly_Zw_print(a,N);
    // printf("b: ");poly_Zw_print(b,N);
    // printf("c: ");poly_Zw_print(c,N);
    // endl

    for(int i = 0;i<N;i++){
        res[i] = a[i];
    }
    for(int i = N;i<2*N;i++){
        res[i] = b[i-N];
    }
    for(int i = 2*N;i<3*N;i++){
        res[i] = c[i-2*N];
    }
    
    // printf("inverse: ");poly_Zw_print(res,N3);
    

    return 1;





    







}
