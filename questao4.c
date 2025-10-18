#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdComPassos(int a, int b) {
    int resto;
    while (b != 0) {                    // Linha 10 - [1] b != 0
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;                      // Linha 14 - [2] resto
    }
    return a;
}

int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    int A = a, B = m;

    if (mdComPassos(a, m) != 1) {       // Linha 23 - [3] if (mdComPassos(a, m) != 1)
        printf("MDC(%d, %d) != 1, não existe inverso modular!\n", a, m);
        return -1;
    }

    while (m != 0) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;                       // Linha 36 - [4] x1 += m0
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

int powMod(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    while (exp > 0) {
        if (exp & 1)                    // Linha 45 - [5] exp & 1
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    printf("Pedro Augusto Macedo Del Castilo - 241025354\n");
    printf("Davi Ursulino de Oliveira - 241012202\n\n");
    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    int inverse = inversoModular(G, Zn); // Linha 72 - [6] inversoModular(G, Zn)
    int a = (H * inverse) % Zn;

    printf("Fazendo a multiplicação modular: %d * %d mod %d = %d\n", H, inverse, Zn, a);
    printf(" Sendo %d o inverso de %d.\n", inverse, G);

    int resultado = powMod(a, x, n1);    // Linha 78 - [7] powMod(a, x, n1)
    printf("Valor final da congruência: %d\n", resultado);

    return 0;
}