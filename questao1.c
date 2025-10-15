#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int mdc(int a, int b){
    if(b == 0){
        return a;
    } else {
        return mdc(b, a % b);
    }
}

int funcao(int x, int N){
    return (x * x + 1) % N;
}

int pollard(int x, int y, int N, int i){
    if(i > 10000) return 1;
    int num = mdc(abs(x - y), N);
    if(num > 1 && num < N){
        return num;
    }

    x = funcao(x, N);
    y = funcao(funcao(y, N), N);
    return pollard(x, y, N, i + 1);
}

int main(){
    int n1, n2;
    printf("Escreva dois dígitos que tenham 3 ou 4 dígitos quem sejam o produto de primos distintos\n");
    scanf("%d%d", &n1, &n2);
    if(n1 < 100 || n1 > 9999 || n2 < 100 || n2 > 9999){
        printf("Os números não estão entre 100 e 9999\n");
        return 0;
    }
    int x0 = 2;
    int p = pollard(x0, x0, n1, 0);
    int q = pollard(x0, x0, n2, 0);

    printf("Valores das chaves p: %d e q: %d\n", p, q);

    return 0;
}