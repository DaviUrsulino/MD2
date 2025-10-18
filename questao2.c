#include <stdio.h>
#include <stdlib.h>

// Função para calcular MDC
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função para calcular MMC
int mmc(int a, int b) {
    return (a * b) / mdc(a, b);
}

int main() {
    int N;
    
    printf("Pedro Augusto Macedo Del Castilo - 241025354\n");
    printf("Davi Ursulino de Oliveira - 241012202\n\n");
    // Validação do número de chaves
    printf("Digite o número de chaves (1-10): ");
    if (scanf("%d", &N) != 1 || N < 1 || N > 10) {
        printf("ERRO: Número de chaves deve estar entre 1 e 10.\n");
        return 1;
    }
    
    int ciclos[10];
    
    printf("Digite os %d ciclos das chaves (cada entre 2-20): ", N);
    for (int i = 0; i < N; i++) {
        if (scanf("%d", &ciclos[i]) != 1) {
            printf("ERRO: Entrada inválida.\n");
            return 1;
        }
        // Validação dos ciclos
        if (ciclos[i] < 2 || ciclos[i] > 20) {
            printf("ERRO: Ciclo %d deve estar entre 2 e 20.\n", ciclos[i]);
            return 1;
        }
    }
    
    printf("\n=== PASSO A PASSO ===\n");
    
    // Exibir ciclos
    printf("1. Ciclos das chaves: ");
    for (int i = 0; i < N; i++) {
        printf("%d", ciclos[i]);
        if (i < N - 1) printf(", ");
    }
    printf("\n");
    
    // Calcular MMC de todos os ciclos
    printf("2. Calculando MMC:\n");
    int resultado = ciclos[0];
    for (int i = 1; i < N; i++) {
        printf("   MMC(%d, %d) = ", resultado, ciclos[i]);
        resultado = mmc(resultado, ciclos[i]);
        printf("%d\n", resultado);
    }
    
    printf("3. MMC final: %d\n", resultado);
    printf("4. Verificando anos de 1 a 50...\n");
    
    // Encontrar o primeiro ano válido
    int ano_encontrado = -1;
    for (int ano = resultado; ano <= 50; ano += resultado) {
        if (ano > 0) {
            ano_encontrado = ano;
            break;
        }
    }
    
    if (ano_encontrado != -1) {
        printf("5. PRIMEIRO ANO SINCRONIZADO: %d\n", ano_encontrado);
        printf("   (Todas as chaves ativas simultaneamente)\n");
    } else {
        printf("5. RESULTADO: Nenhum ano válido encontrado no intervalo de 1 a 50.\n");
        printf("   (MMC = %d > 50)\n", resultado);
    }
    
    return 0;
}