#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Função para calcular MDC usando Algoritmo de Euclides
int calcularMDC(int a, int b) {
    printf("Calculando MDC(%d, %d):\n", a, b);
    
    int temp;
    while (b != 0) {
        int resto = a % b;
        printf("%d = %d * %d + %d\n", a, b, a/b, resto);
        temp = b;
        b = resto;
        a = temp;
    }
    
    printf("MDC = %d\n", a);
    return a;
}

// Função para valor absoluto
int absoluto(int x) {
    return (x < 0) ? -x : x;
}

// Método ρ de Pollard para fatoração
int pollardRho(int n) {
    printf("\n=== INICIANDO METODO ρ DE POLLARD PARA N = %d ===\n", n);
    
    if (n == 1) return n;
    if (n % 2 == 0) return 2;
    
    int x = 2; // semente
    int y = 2;
    int d = 1;
    int iteracao = 1;
    
    while (d == 1) {
        printf("\nIteracao %d:\n", iteracao);
        // g(x) = (x² + 1) mod n
        x = (x * x + 1) % n;
        y = (y * y + 1) % n;
        y = (y * y + 1) % n;
        
        int diff = absoluto(x - y);
        printf("x = %d, y = %d, |x - y| = %d\n", x, y, diff);
        
        d = calcularMDC(diff, n);
        printf("MDC(|x - y|, n) = %d\n", d);
        
        iteracao++;
        
        if (d == n) {
            printf("Fator nao trivial nao encontrado. Reiniciando...\n");
            x = rand() % (n - 2) + 2;
            y = x;
            d = 1;
        }
    }
    
    printf("Fator encontrado: %d\n", d);
    return d;
}

// Algoritmo Estendido de Euclides para encontrar inverso modular
int inversoModular(int a, int m) {
    printf("\n=== CALCULANDO INVERSO MODULAR DE %d mod %d ===\n", a, m);
    
    int m0 = m;
    int y = 0, x = 1;
    
    if (m == 1) return 0;
    
    printf("Passos do Algoritmo Estendido de Euclides:\n");
    
    while (a > 1) {
        int q = a / m;
        int t = m;
        
        printf("%d = %d * %d + %d\n", a, m, q, a % m);
        
        m = a % m;
        a = t;
        t = y;
        
        y = x - q * y;
        x = t;
    }
    
    if (x < 0) x += m0;
    
    printf("Inverso modular: %d\n", x);
    return x;
}

// Função de Euler para calcular φ(n)
int totientEuler(int p, int q) {
    return (p - 1) * (q - 1);
}

// Função para verificar se um número é primo (simplificada)
int ehPrimo(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Função para exponenciação modular com seleção automática do teorema
int exponenciacaoModular(int base, int expoente, int modulo, const char* contexto) {
    printf("\n=== EXPONENCIACAO MODULAR: %d^%d mod %d (%s) ===\n", 
           base, expoente, modulo, contexto);
    
    // Verificar condições para aplicar diferentes teoremas
    if (ehPrimo(modulo)) {
        printf("Aplicando Pequeno Teorema de Fermat (n eh primo)\n");
        // a^(p-1) ≡ 1 mod p
        int exp_reduzido = expoente % (modulo - 1);
        printf("Expoente reduzido: %d mod %d = %d\n", 
               expoente, modulo - 1, exp_reduzido);
        expoente = exp_reduzido;
    } else if (calcularMDC(base, modulo) == 1) {
        printf("Aplicando Teorema de Euler (MDC(M, n) = 1)\n");
        int phi = 0;
        // Para simplificar, vamos assumir que sabemos os fatores de modulo
        // Na prática, precisaríamos fatorar modulo para calcular φ(n)
        printf("Calculando φ(%d) seria necessario fatorar n\n", modulo);
    } else {
        printf("Aplicando Teorema da Divisao Euclidiana\n");
        printf("Expoente sera processado diretamente\n");
    }
    
    // Implementação da exponenciação modular
    long long resultado = 1;
    long long base_temp = base % modulo;
    int exp_temp = expoente;
    
    printf("Calculo passo a passo:\n");
    
    while (exp_temp > 0) {
        if (exp_temp % 2 == 1) {
            long long resultado_antigo = resultado;
            resultado = (resultado * base_temp) % modulo;
            printf("%lld * %lld mod %d = %lld\n", 
                   resultado_antigo, base_temp, modulo, resultado);
        }
        
        long long base_antiga = base_temp;
        base_temp = (base_temp * base_temp) % modulo;
        printf("Base: %lld^2 mod %d = %lld\n", base_antiga, modulo, base_temp);
        
        exp_temp /= 2;
        printf("Expoente restante: %d\n", exp_temp);
    }
    
    printf("Resultado final: %lld\n", resultado);
    return (int)resultado;
}

// Função para converter caractere para código numérico
int charParaCodigo(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 11 + (c - 'A');
    } else if (c >= 'a' && c <= 'z') {
        return 11 + (c - 'a');
    } else if (c == ' ') {
        return 0;
    }
    return -1; // Caractere inválido
}

// Função para converter código numérico para caractere
char codigoParaChar(int codigo) {
    if (codigo == 0) return ' ';
    if (codigo >= 11 && codigo <= 36) {
        return 'A' + (codigo - 11);
    }
    return '?'; // Código inválido
}

// Função para pré-codificar mensagem
int* preCodificarMensagem(const char* mensagem, int* tamanho) {
    int len = strlen(mensagem);
    *tamanho = len;
    int* codigos = (int*)malloc(len * sizeof(int));
    
    printf("\n=== PRE-CODIFICACAO DA MENSAGEM ===\n");
    printf("Mensagem original: %s\n", mensagem);
    printf("Tabela de conversao:\n");
    printf("A=11, B=12, ..., Z=36, ESPACO=00\n");
    
    for (int i = 0; i < len; i++) {
        codigos[i] = charParaCodigo(mensagem[i]);
        printf("%c -> %02d\n", mensagem[i], codigos[i]);
    }
    
    return codigos;
}

// Função para codificar mensagem com RSA
int* codificarRSA(const int* mensagemCodificada, int tamanho, int e, int n, int* tamanhoCodificado) {
    *tamanhoCodificado = tamanho;
    int* codificado = (int*)malloc(tamanho * sizeof(int));
    
    printf("\n=== CODIFICACAO RSA ===\n");
    printf("Usando chave publica: (n=%d, e=%d)\n", n, e);
    
    for (int i = 0; i < tamanho; i++) {
        printf("\nBloco %d: M = %d\n", i+1, mensagemCodificada[i]);
        codificado[i] = exponenciacaoModular(mensagemCodificada[i], e, n, "Codificacao");
        printf("Texto cifrado C = %d\n", codificado[i]);
    }
    
    return codificado;
}

// Função para decodificar mensagem com RSA
int* decodificarRSA(const int* mensagemCodificada, int tamanho, int d, int n, int* tamanhoDecodificado) {
    *tamanhoDecodificado = tamanho;
    int* decodificado = (int*)malloc(tamanho * sizeof(int));
    
    printf("\n=== DECODIFICACAO RSA ===\n");
    printf("Usando chave privada: (n=%d, d=%d)\n", n, d);
    
    for (int i = 0; i < tamanho; i++) {
        printf("\nBloco %d: C = %d\n", i+1, mensagemCodificada[i]);
        decodificado[i] = exponenciacaoModular(mensagemCodificada[i], d, n, "Decodificacao");
        printf("Texto decifrado M = %d\n", decodificado[i]);
    }
    
    return decodificado;
}

// Função para converter códigos decodificados de volta para texto
char* decodificarParaTexto(const int* codigos, int tamanho) {
    char* texto = (char*)malloc((tamanho + 1) * sizeof(char));
    
    printf("\n=== CONVERSAO PARA TEXTO ===\n");
    
    for (int i = 0; i < tamanho; i++) {
        texto[i] = codigoParaChar(codigos[i]);
        printf("%02d -> %c\n", codigos[i], texto[i]);
    }
    texto[tamanho] = '\0';
    
    return texto;
}

int main() {
    printf("=== SISTEMA RSA COMPLETO ===\n");
    
    // Etapa 1: Fatoração com Método ρ de Pollard
    printf("\n*** ETAPA 1: FATORACAO COM METODO ρ DE POLLARD ***\n");
    
    int N1, N2;
    printf("Digite o primeiro numero composto N1 (100-9999): ");
    scanf("%d", &N1);
    printf("Digite o segundo numero composto N2 (100-9999): ");
    scanf("%d", &N2);
    
    printf("\nAVISO: Cada Ni deve ser produto de primos distintos para eficiencia do metodo.\n");
    
    int p = pollardRho(N1);
    int q = pollardRho(N2);
    
    printf("\n=== FATORES ENCONTRADOS ===\n");
    printf("p = %d (fator de N1=%d)\n", p, N1);
    printf("q = %d (fator de N2=%d)\n", q, N2);
    
    // Etapa 2: Geração das Chaves RSA
    printf("\n*** ETAPA 2: GERACAO DAS CHAVES RSA ***\n");
    
    int n = p * q;
    int z = totientEuler(p, q);
    
    printf("n = p * q = %d * %d = %d\n", p, q, n);
    printf("φ(n) = (p-1) * (q-1) = %d * %d = %d\n", p-1, q-1, z);
    
    // Encontrar E (expoente público)
    int E = 2;
    while (E < z) {
        if (calcularMDC(E, z) == 1) {
            break;
        }
        E++;
    }
    
    printf("Expoente publico E = %d (MDC(E, φ(n)) = 1)\n", E);
    
    // Calcular D (expoente privado)
    int D = inversoModular(E, z);
    
    printf("\n=== CHAVES RSA ===\n");
    printf("Chave Publica: (n=%d, E=%d)\n", n, E);
    printf("Chave Privada: (n=%d, D=%d)\n", n, D);
    
    // Etapa 3: Codificação e Decodificação
    printf("\n*** ETAPA 3: CODIFICACAO E DECODIFICACAO ***\n");
    
    char mensagem[100];
    printf("Digite a mensagem para codificar (apenas letras maiusculas e espacos): ");
    getchar(); // Limpar buffer
    fgets(mensagem, sizeof(mensagem), stdin);
    mensagem[strcspn(mensagem, "\n")] = 0; // Remover nova linha
    
    // Pré-codificação
    int tamanhoMensagem;
    int* mensagemPreCodificada = preCodificarMensagem(mensagem, &tamanhoMensagem);
    
    // Codificação RSA
    int tamanhoCodificado;
    int* mensagemCodificada = codificarRSA(mensagemPreCodificada, tamanhoMensagem, E, n, &tamanhoCodificado);
    
    // Decodificação RSA
    int tamanhoDecodificado;
    int* mensagemDecodificada = decodificarRSA(mensagemCodificada, tamanhoCodificado, D, n, &tamanhoDecodificado);
    
    // Conversão para texto
    char* textoFinal = decodificarParaTexto(mensagemDecodificada, tamanhoDecodificado);
    
    printf("\n=== RESULTADOS FINAIS ===\n");
    printf("Mensagem original: %s\n", mensagem);
    printf("Mensagem decodificada: %s\n", textoFinal);
    
    if (strcmp(mensagem, textoFinal) == 0) {
        printf("SUCESSO: Mensagem decodificada e identica a original!\n");
    } else {
        printf("ERRO: Mensagem decodificada difere da original!\n");
    }
    
    // Liberar memória
    free(mensagemPreCodificada);
    free(mensagemCodificada);
    free(mensagemDecodificada);
    free(textoFinal);
    
    return 0;
}