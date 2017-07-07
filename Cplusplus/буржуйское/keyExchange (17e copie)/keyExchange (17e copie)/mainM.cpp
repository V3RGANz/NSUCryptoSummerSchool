#include "Matrix.cpp"

#include <iostream>
#include <time.h>
#include <math.h>

#define SIZE_MAX 20 
#define VALUE_MAX 100
#define COEF_MAX 20

using namespace std;

// return a value in [-max;max]
int random(int max) {
    int res = rand()%(max+1);
    if (rand()%2 == 0) // select a sign for the value
        return res;
    return -res;
}

// find the solution
int solve(Matrix A, Matrix W, Matrix K, int coef_range) {
    int n = A.getN();
    
    // find all power of A that we need
    Matrix A_pow[n];
    
    int i;
    A_pow[0] = Matrix(n);
    A_pow[1] = A;
    for(i=2; i<n; i++) {
        A_pow[i] = A*A_pow[i-1];
    }
    
    // initialize factors to min value
    int factors[n];
    for(i=0; i<n; i++) {
        factors[i] = -coef_range;
    }
    
    bool solution_ok = false;
    i = 0;
    long nb_solutions = pow(1+coef_range*2, n);
    int factor_changed = 0;
    Matrix Key("Key", n, n);
    while ((!solution_ok) && (i<nb_solutions)) {
        
        Key.zeros();
        for(int k=0; k<n; k++) {
            Key = Key + factors[k]*A_pow[k];
        }
        Key = Key*W;
        if (Key == K) solution_ok = true;
        
        if (!solution_ok) {
        
            factor_changed = 0;
            while(factors[factor_changed] == coef_range) {
                factor_changed++;
            }
            
            factors[factor_changed]++;
            
            for(int k=0; k<factor_changed; k++) {
                factors[k] = -coef_range;
            }
            
            
            
            cout << i << " : ";
            for(int k=0; k<n; k++) {
                cout << factors[k] << ", ";
            }
            cout << endl;
            
            i++;
        }
    }
}

int main() {
    
    // initialize random seed to choose random Matrix and numbers
    srand(time(NULL));
	
	/*Matrix M1("M1", 4, 4);
    M1.random(10);
    cout << M1 << endl;
    Matrix M2("M2", 4, 4);
    M2.random(10);
    cout << M2 << endl;
	
    try {
        Matrix M3("M3", 4, 4);
        M3 = M1.mult(M2);
        Matrix M4("M4", 4, 4);
        M4 = M1*M2;
        cout << M3 << endl;
        cout << M4 << endl;
    } catch(IncompatibleMatricesException e) {cout << e.what() << endl;}
    */
    
    /*Matrix M1("M1", 4, 4);
    //M1.setElement(0, 0, 1);
    Matrix M2("M2", 4, 4);
    cout << M1 << endl;
    cout << M2 << endl;
    try {
        cout << (M1 != M2) << endl;
    } catch(IncompatibleMatricesException e) {cout << e.what() << endl;}
    */
    
    /*Matrix M1("M1", 5, 5);
    M1.random(1000);
    cout << M1 << endl;
    Matrix M2("M2", 5, 5);
    M2.random(1000);
    cout << M2 << endl;
    
    Matrix M3("M3", 3, 3);
    M3 = M1-2*M2;
    cout << M3 << endl;
    cout << "done !" << endl;
    */
    
    /*Matrix M1(25);
    cout << "M1 : " << endl << M1 << endl;*/
    
    /*Matrix M1(3);
    cout << M1 << endl;
    Matrix M2("A", 3, 3);
    cout << M2 << endl;
    */
    
    /*Matrix M1("M1", 3, 3);
    M1.random(1000);
    cout << M1;
    M1.modulo(5);
    cout << M1;
    */
    
    /*
    // select size of the key
    
    cout << "Enter number of rows : ";
    int n=0, m=0;
    while((n<1) || (n>SIZE_MAX)) {
        cin >> n;
        if((n<1) || (n>SIZE_MAX))
            cout << "Please select a number between 1 and " << SIZE_MAX << " : ";
    }
    
    cout << "Enter number of columns : ";
    while((m<1) || (m>SIZE_MAX)) {
        cin >> m;
        if((m<1) || (m>SIZE_MAX))
            cout << "Please select a number between 1 and " << SIZE_MAX << " : ";
    }
    
    // select matrices A, B and W
    Matrix A("A", n, n);
    A.random(VALUE_MAX);
    Matrix B("B", m, m);
    B.random(VALUE_MAX);
    Matrix W("W", n, m);
    W.random(VALUE_MAX);
    
    cout << A << B << W;
    
    // Alice chooses polynomials
    int f0a, f1a, h0a, h1a;
    f0a = random(COEF_MAX);
    f1a = random(COEF_MAX);
    h0a = random(COEF_MAX);
    h1a = random(COEF_MAX);
    
    // Bob chooses polynomials
    int f0b, f1b, h0b, h1b;
    f0b = random(COEF_MAX);
    f1b = random(COEF_MAX);
    h0b = random(COEF_MAX);
    h1b = random(COEF_MAX);
    
    
    // create identity matrices
    Matrix In(n);
    Matrix Im(m);
    
    // Alice computes F1(A) and H1(B)
    Matrix F1A("F1(A)", n, n);
    F1A = f1a*A + f0a*In;
    cout << F1A;
    F1A.modulo(10);
    Matrix H1B("H1(B)", m, m);
    H1B = h1a*B + h0a*Im;
    H1B.modulo(10);
    
    cout << endl << F1A << H1B;
    
    // Bob computes F2(A) and H2(B)
    Matrix F2A("F2(A)", n, n);
    F2A = f1b*A + f0b*In;
    cout << F2A;
    F2A.modulo(10);
    Matrix H2B("H2(B)", m, m);
    H2B = h1b*B + h0b*Im;
    H2B.modulo(10);
    
    cout << endl << F2A << H2B;
    
    // they compute W1 and W2
    Matrix W1("W1", n, m);
    W1 = F1A*W*H1B;
    cout << W1;
    W1.modulo(10);
    Matrix W2("W2", n, m);
    W2 = F2A*W*H2B;
    W2.modulo(10);
    
    cout << endl << W1 << W2;
    
    // Alice computes KA
    Matrix KA("KA", n, m);
    KA = F1A*W2*H1B;
    KA.modulo(10);
    // Bob computes KB
    Matrix KB("KB", n, m);
    KB = F2A*W1*H2B;
    KB.modulo(10);
    
    cout << endl << KA << KB << endl;
    
    if (KA == KB) cout << "Alice and Bob get the same key !" << endl << endl;
    else cout << "Error, Alice and Bob don't get the same key..." << endl << endl;
    */
    
   
    /*int mod = 100;
    
    // select a non-invertible matrix W
    Matrix W("W", 3, 3);
    W.rank_1(VALUE_MAX);
    //cout << W;
    
    // select a Matrix A
    Matrix A("A", 3, 3);
    A.setElement(0, 0, 5);A.setElement(0, 1, -2);A.setElement(0, 2, 0);
    A.setElement(1, 0, 1);A.setElement(1, 1, 3);A.setElement(1, 2, 2);
    A.setElement(2, 0, 4);A.setElement(2, 1, 1);A.setElement(2, 2, 7);
    //cout << A;
    
    // Alice chooses f(A)
    Matrix f_A("f(A)", 3, 3);
    f_A = 2*A*A + 3*A;
    
    // Bob chooses g(A)
    Matrix g_A("g(A)", 3, 3);
    g_A = -1*A*A + 2*A;
    
    // f'(A) = lambda²(f(A))
    Matrix fp_A("f'(A)", 3, 3);
    fp_A.setElement(0, 0, 2977);fp_A.setElement(0, 1, 3086);fp_A.setElement(0, 2, -1532);
    fp_A.setElement(1, 0, 4607);fp_A.setElement(1, 1, 8383);fp_A.setElement(1, 2, -4618);
    fp_A.setElement(2, 0, -2725);fp_A.setElement(2, 1, -755);fp_A.setElement(2, 2, 2999);
    
    // g'(A) = lambda²(g(A))
    Matrix gp_A("g'(A)", 3, 3);
    gp_A.setElement(0, 0, 207);gp_A.setElement(0, 1, 264);gp_A.setElement(0, 2, -180);
    gp_A.setElement(1, 0, 492);gp_A.setElement(1, 1, 645);gp_A.setElement(1, 2, -444);
    gp_A.setElement(2, 0, -123);gp_A.setElement(2, 1, -138);gp_A.setElement(2, 2, 111);
    
    // Alice sends WA = f'(A)*W
    Matrix WA("WA", 3, 3);
    WA = fp_A*W;
    
    // Bob sends WB = g'(A)*W
    Matrix WB("WB", 3, 3);
    WB = gp_A*W;
    
    // Alice gets K = f'(A)*g'(A)*W
    Matrix KA("KA", 3, 3);
    KA = fp_A*WB;
    cout << KA;
    
    // Bob gets K = g'(A)*f'(A)*W
    Matrix KB("KB", 3, 3);
    KB = gp_A*WA;
    cout << KB;
    
    if (KA == KB) cout << "Both Alice and Bob have the same key !" << endl;
    else cout << "Alice and Bob don't have the same key..." << endl;
    */
    
    
    /*
    // Check complexity
    
    int m = 5;
    Matrix A("A", m, m);
    A.random(10);
    cout << A; 
    
    Matrix I(m);
    
    Matrix f_A("f(A)", m, m);
    f_A = 2*A*A*A*A+1*A*A*A-3*A*A+1*A+2*I;
    cout << f_A;
    
    int nb_operations = m*m*m+m*m*(m-1);
    cout << nb_operations << endl;
    cout << A.nb_op << endl;
    */
    
    /*int size = 6;
    Matrix A("A", size, size);
    A.setElement(0,0,1);A.setElement(0,1,1);A.setElement(0,2,3);A.setElement(0,3,1);A.setElement(0,4,1);A.setElement(0,5,3);
    A.setElement(1,0,-2);A.setElement(1,1,0);A.setElement(1,2,-4);A.setElement(1,3,1);A.setElement(1,4,1);A.setElement(1,5,3);
    A.setElement(2,0,0);A.setElement(2,1,5);A.setElement(2,2,1);A.setElement(2,0,0);A.setElement(2,1,5);A.setElement(2,2,1);
    A.setElement(3,0,1);A.setElement(3,1,1);A.setElement(3,2,3);A.setElement(3,3,1);A.setElement(3,4,1);A.setElement(3,5,3);
    A.setElement(4,0,-2);A.setElement(4,1,0);A.setElement(4,2,-4);A.setElement(4,3,1);A.setElement(4,4,1);A.setElement(4,5,3);
    A.setElement(5,0,0);A.setElement(5,1,5);A.setElement(5,2,1);A.setElement(5,0,0);A.setElement(5,1,5);A.setElement(5,2,1);
    
    Matrix W("W", size, size);
    W.random(100);
    
    Matrix I(size);
    
    Matrix K("K", size, size);
    K = (15*A*A + 2*A + 1*I)*W;
    
    
    cout << A << W << K;
    
    solve(A, W, K, COEF_MAX);*/
    
    Matrix A("A", 4, 4);
    A.setElement(0,0,6);A.setElement(0,1,2);A.setElement(0,2,3);A.setElement(0,3,3);
    A.setElement(1,0,2);A.setElement(1,1,1);A.setElement(1,2,6);A.setElement(1,3,7);
    A.setElement(2,0,3);A.setElement(2,1,4);A.setElement(2,2,7);A.setElement(2,3,4);
    A.setElement(3,0,9);A.setElement(3,1,5);A.setElement(3,2,2);A.setElement(3,3,9);
    cout << A;
    
    Matrix B("B", 4, 4);
    B.setElement(0,0,1);B.setElement(0,1,7);B.setElement(0,2,1);B.setElement(0,3,2);
    B.setElement(1,0,5);B.setElement(1,1,4);B.setElement(1,2,3);B.setElement(1,3,2);
    B.setElement(2,0,9);B.setElement(2,1,2);B.setElement(2,2,2);B.setElement(2,3,8);
    B.setElement(3,0,9);B.setElement(3,1,6);B.setElement(3,2,8);B.setElement(3,3,4);
    cout << B;
    
    Matrix W("W", 4, 4);
    W.setElement(0,0,1);W.setElement(0,1,2);W.setElement(0,2,3);W.setElement(0,3,2);
    W.setElement(1,0,3);W.setElement(1,1,2);W.setElement(1,2,5);W.setElement(1,3,2);
    W.setElement(2,0,1);W.setElement(2,1,1);W.setElement(2,2,2);W.setElement(2,3,1);
    W.setElement(3,0,2);W.setElement(3,1,2);W.setElement(3,2,4);W.setElement(3,3,2);
    cout << W;
    
    Matrix I(4);
    
    Matrix S1("S1", 4, 4);
    S1 = A*A*A + 3*A*A + 2*A + 5*I;
    S1.modulo(10);
    cout << S1;
    
    Matrix T1("T1", 4, 4);
    T1 = 2*B*B*B -5*B*B + -3*B + -2*I;
    T1.modulo(10);
    cout << T1;
    
    Matrix S2("S2", 4, 4);
    S2 = -1*A*A*A + 2*A*A + 4*A + 1*I;
    S2.modulo(10);
    cout << S2;
    
    Matrix T2("T2", 4, 4);
    T2 = -2*B*B*B + 3*B*B + 5*B + 2*I;
    T2.modulo(10);
    cout << T2;
    
    // change of basis
    
    Matrix S1p("S1'", 4, 4);
    S1p.setElement(0,0,9);S1p.setElement(0,1,9);S1p.setElement(0,2,0);S1p.setElement(0,3,0);
    S1p.setElement(1,0,0);S1p.setElement(1,1,0);S1p.setElement(1,2,0);S1p.setElement(1,3,0);
    S1p.setElement(2,0,3);S1p.setElement(2,1,3);S1p.setElement(2,2,0);S1p.setElement(2,3,0);
    S1p.setElement(3,0,6);S1p.setElement(3,1,6);S1p.setElement(3,2,0);S1p.setElement(3,3,0);
    cout << S1p;
    
    Matrix T1p("T1'", 4, 4);
    T1p.setElement(0,0,1);T1p.setElement(0,1,1);T1p.setElement(0,2,6);T1p.setElement(0,3,2);
    T1p.setElement(1,0,6);T1p.setElement(1,1,8);T1p.setElement(1,2,2);T1p.setElement(1,3,8);
    T1p.setElement(2,0,8);T1p.setElement(2,1,0);T1p.setElement(2,2,8);T1p.setElement(2,3,6);
    T1p.setElement(3,0,0);T1p.setElement(3,1,8);T1p.setElement(3,2,8);T1p.setElement(3,3,2);
    cout << T1p;
    
    Matrix S2p("S2'", 4, 4);
    S2p.setElement(0,0,8);S2p.setElement(0,1,3);S2p.setElement(0,2,7);S2p.setElement(0,3,1);
    S2p.setElement(1,0,1);S2p.setElement(1,1,8);S2p.setElement(1,2,5);S2p.setElement(1,3,6);
    S2p.setElement(2,0,1);S2p.setElement(2,1,3);S2p.setElement(2,2,9);S2p.setElement(2,3,7);
    S2p.setElement(3,0,1);S2p.setElement(3,1,0);S2p.setElement(3,2,5);S2p.setElement(3,3,7);
    cout << S2p;
    
    Matrix T2p("T2'", 4, 4);
    T2p.setElement(0,0,3);T2p.setElement(0,1,5);T2p.setElement(0,2,0);T2p.setElement(0,3,4);
    T2p.setElement(1,0,0);T2p.setElement(1,1,0);T2p.setElement(1,2,0);T2p.setElement(1,3,0);
    T2p.setElement(2,0,4);T2p.setElement(2,1,0);T2p.setElement(2,2,0);T2p.setElement(2,3,2);
    T2p.setElement(3,0,8);T2p.setElement(3,1,0);T2p.setElement(3,2,2);T2p.setElement(3,3,4);
    cout << T2p;
    
    Matrix KA("KA", 4, 4);
    KA = S2p*T1p;
    KA.modulo(10);
    cout << KA;
    
    Matrix KB("KB", 4, 4);
    KB = T1p*S2p;
    KB.modulo(10);
    cout << KB;
    
    // lien : https://math.stackexchange.com/questions/1604461/action-of-a-matrix-on-the-exterior-algebra
}
