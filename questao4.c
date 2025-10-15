#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdccmPassos(int a, int b) {
    int resto;
    while (b != 0) {
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;
    }
    return a;
}

int inverseModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    int A = a, B = m;

    // Verificar se existe inverso
    if (mdccmPassos(a, m) != 1) {
        printf("ERRO: MDC(%d, %d) ≠ 1, não existe inverso modular!\n", a, m);
        return -1;
    }

    // Reinicializar variáveis para o algoritmo estendido
    a = A;
    m = m0;
    x0 = 0;
    x1 = 1;

    printf("\nAplicando Algoritmo Estendido de Euclides:\n");
    while (m != 0) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
        
        printf("q=%d, x0=%d, x1=%d\n", q, x0, x1);
    }
    
    if (x1 < 0)
        x1 += m0;
    
    printf("Substituindo, temos que o inverso de %d em %d é %d.\n", A, B, x1);
    printf("Verificação: %d × %d = %d ≡ 1 (mod %d)\n\n", A, x1, A * x1, B);
    return x1;
}

int powMod(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    
    printf("Calculando %d^%d mod %d:\n", base, exp, mod);
    
    // Verificar se podemos aplicar Fermat ou Euler
    if (mod > 1) {
        // Verificar se mod é primo
        int is_prime = 1;
        for (int i = 2; i * i <= mod; i++) {
            if (mod % i == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime && mod > 1) {
            printf("→ n1 = %d é PRIMO - Aplicando Pequeno Teorema de Fermat\n", mod);
            printf("→ Pelo Teorema: a^(%d-1) ≡ 1 mod %d\n", mod, mod);
            exp = exp % (mod - 1);
            printf("→ Expoente reduzido: %d mod %d = %d\n", exp + (mod - 1), mod - 1, exp);
        } else if (mdccmPassos(base, mod) == 1) {
            // Calcular φ(mod) para Teorema de Euler
            int phi = 1;
            int temp_mod = mod;
            for (int i = 2; i * i <= temp_mod; i++) {
                if (temp_mod % i == 0) {
                    phi *= (i - 1);
                    temp_mod /= i;
                    while (temp_mod % i == 0) {
                        phi *= i;
                        temp_mod /= i;
                    }
                }
            }
            if (temp_mod > 1) phi *= (temp_mod - 1);
            
            printf("→ Aplicando Teorema de Euler: a^φ(%d) ≡ 1 mod %d (φ(%d)=%d)\n", mod, mod, mod, phi);
            exp = exp % phi;
            printf("→ Expoente reduzido: %d mod %d = %d\n", exp + phi, phi, exp);
        }
    }
    
    // Exponenciação por quadratura
    printf("→ Método: Exponenciação por Quadratura\n");
    int step = 1;
    while (exp > 0) {
        if (exp & 1) {
            long long old_res = res;
            res = (res * b) % mod;
            printf("  Passo %d: res = (%lld × %lld) mod %d = %lld\n", step++, old_res, b, mod, res);
        }
        long long old_b = b;
        b = (b * b) % mod;
        printf("  Passo %d: b = (%lld^2) mod %d = %lld\n", step++, old_b, mod, b);
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    // Entradas com validação
    printf("=== SISTEMA DE DIVISÃO MODULAR E EXPONENCIAÇÃO ===\n\n");
    
    printf("Insira H: ");
    if (scanf("%d", &H) != 1) {
        printf("ERRO: Entrada inválida para H.\n");
        return 1;
    }
    
    printf("Insira G: ");
    if (scanf("%d", &G) != 1) {
        printf("ERRO: Entrada inválida para G.\n");
        return 1;
    }
    
    printf("Insira Zn: ");
    if (scanf("%d", &Zn) != 1 || Zn <= 1) {
        printf("ERRO: Zn deve ser maior que 1.\n");
        return 1;
    }
    
    printf("Insira x: ");
    if (scanf("%d", &x) != 1) {
        printf("ERRO: Entrada inválida para x.\n");
        return 1;
    }
    
    printf("Insira n1: ");
    if (scanf("%d", &n1) != 1 || n1 <= 1) {
        printf("ERRO: n1 deve ser maior que 1.\n");
        return 1;
    }
    
    printf("\n=== PROCESSAMENTO ===\n");

    // Cálculo do inverso modular
    printf("\n1. CALCULANDO INVERSO MODULAR:\n");
    int inverse = inverseModular(G, Zn);
    if (inverse == -1) {
        printf("ERRO: Não é possível continuar sem inverso modular.\n");
        return 1;
    }

    // Divisão modular
    printf("2. DIVISÃO MODULAR:\n");
    int a = (H * inverse) % Zn;
    printf("   Operação: (%d × %d) mod %d = %d\n", H, inverse, Zn, a);
    printf("   Onde %d é o inverso de %d módulo %d\n", inverse, G, Zn);

    // Exponenciação modular
    printf("\n3. EXPONENCIAÇÃO MODULAR:\n");
    int resultado = powMod(a, x, n1);
    
    printf("\n4. RESULTADO FINAL:\n");
    printf("   Valor final da congruência: %d\n", resultado);

    return 0;
}