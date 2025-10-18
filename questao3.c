#include <stdio.h>
#include <math.h>

int main() {
    int N;
    printf("Pedro Augusto Macedo Del Castilo - 241025354\n");
    printf("Davi Ursulino de Oliveira - 241012202\n\n");
    // Validação da entrada
    printf("Digite o valor de N (1-100000): ");
    if (scanf("%d", &N) != 1 || N < 1 || N > 105) {
        printf("ERRO: N deve estar entre 1 e 100000.\n");
        return 1;
    }
    
    printf("\n=== CÁLCULO DA RAZÃO DE EFICIÊNCIA PARA N = %d ===\n", N);
    
    // Caso especial N = 1
    if (N == 1) {
        printf("\n1. FATORAÇÃO PRIMA:\n");
        printf("   N = 1 (não possui fatores primos)\n");
        
        printf("\n2. CÁLCULO DE τ(N) (número de divisores):\n");
        printf("   τ(1) = 1\n");
        
        printf("\n3. CÁLCULO DE σ(N) (soma dos divisores):\n");
        printf("   σ(1) = 1\n");
        
        printf("\n4. RAZÃO DE EFICIÊNCIA:\n");
        printf("   Razão = σ(N) / τ(N) = 1 / 1 = 1.00\n");
        return 0;
    }
    
    // Fatoração prima
    printf("\n1. FATORAÇÃO PRIMA:\n");
    int temp = N;
    int fatores[100], expoentes[100];
    int idx = 0;
    
    // FATORAÇÃO POR TRIAL DIVISION (DIVISÃO POR TENTATIVA)
    printf("→ Método: Trial Division (divisão por tentativa)\n");
    printf("→ Justificativa: Para N ≤ 10^5, Trial Division é suficiente e eficiente\n");
    printf("→ Algoritmos avançados como Pollard's Rho não são necessários\n");
    printf("→ Verificando divisores de 2 até √N = %d\n", (int)sqrt(N));
    
    // Fatoração por trial division
    for (int i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            fatores[idx] = i;
            expoentes[idx] = 0;
            printf("→ Encontrado fator primo: %d\n", i);
            while (temp % i == 0) {
                expoentes[idx]++;
                temp /= i;
                printf("  Dividindo: expoente = %d, resto = %d\n", expoentes[idx], temp);
            }
            printf("   Fator primo: %d, Expoente: %d\n", fatores[idx], expoentes[idx]);
            idx++;
        }
    }
    
    // Último fator primo (se existir)
    if (temp > 1) {
        fatores[idx] = temp;
        expoentes[idx] = 1;
        printf("→ Último fator primo: %d\n", temp);
        printf("   Fator primo: %d, Expoente: %d\n", fatores[idx], expoentes[idx]);
        idx++;
    }
    
    printf("→ Fatoração completa: %d = ", N);
    for (int i = 0; i < idx; i++) {
        printf("%d^%d", fatores[i], expoentes[i]);
        if (i < idx - 1) printf(" × ");
    }
    printf("\n");
    
    // Cálculo de τ(N) - número de divisores
    printf("\n2. CÁLCULO DE τ(N) (número de divisores):\n");
    int tau = 1;
    printf("   Fórmula: τ(N) = ");
    for (int i = 0; i < idx; i++) {
        tau *= (expoentes[i] + 1);
        printf("(a%d + 1)", i+1);
        if (i < idx - 1) printf(" × ");
    }
    printf("\n   τ(N) = ");
    for (int i = 0; i < idx; i++) {
        printf("(%d + 1)", expoentes[i]);
        if (i < idx - 1) printf(" × ");
    }
    printf(" = %d\n", tau);
    
    // Cálculo de σ(N) - soma dos divisores
    printf("\n3. CÁLCULO DE σ(N) (soma dos divisores):\n");
    int sigma = 1;
    
    for (int i = 0; i < idx; i++) {
        // Calcula a soma das potências do fator primo
        int soma_potencia = 0;
        int potencia = 1;
        
        printf("   Para fator %d^%d: σ = ", fatores[i], expoentes[i]);
        printf("1");
        soma_potencia = 1; // Começa com 1 (p^0)
        
        // Calcula e exibe as potências
        int valor_atual = fatores[i];
        for (int j = 1; j <= expoentes[i]; j++) {
            printf(" + %d", valor_atual);
            soma_potencia += valor_atual;
            valor_atual *= fatores[i];
        }
        printf(" = %d\n", soma_potencia);
        
        sigma *= soma_potencia;
    }
    
    printf("   σ(N) = ");
    for (int i = 0; i < idx; i++) {
        // Calcula o valor individual de cada termo para exibição
        int termo = 0;
        int potencia = 1;
        for (int j = 0; j <= expoentes[i]; j++) {
            termo += potencia;
            potencia *= fatores[i];
        }
        printf("%d", termo);
        if (i < idx - 1) printf(" × ");
    }
    printf(" = %d\n", sigma);
    
    // Razão de Eficiência
    printf("\n4. RAZÃO DE EFICIÊNCIA:\n");
    double razao = (double)sigma / tau;
    printf("   Razão = σ(N) / τ(N) = %d / %d = %.2f\n", sigma, tau, razao);
    
    return 0;
}