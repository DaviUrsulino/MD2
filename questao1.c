#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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

// Função para verificar se um número é primo
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

// Função para verificar se um número é composto (não primo e maior que 1)
int ehComposto(int n) {
    return (n > 1 && !ehPrimo(n));
}

// Função para verificar se um número tem 3 ou 4 dígitos
int temTresOuQuatroDigitos(int n) {
    return (n >= 100 && n <= 9999);
}

// Método ρ de Pollard para fatoração com semente fixa x0=2
int pollardRho(int n) {
    printf("\n------------INICIANDO METODO ρ DE POLLARD PARA N = %d------------\n", n);
    printf("Semente fixa: x0 = 2\n");
    printf("Funcao de iteracao: g(x) = (x^2 + 1) mod n\n");
    
    // Verificar fatores pequenos primeiro
    if (n % 2 == 0) {
        printf("Fator encontrado (divisao por 2): 2\n");
        return 2;
    }
    if (n % 3 == 0) {
        printf("Fator encontrado (divisao por 3): 3\n");
        return 3;
    }
    
    int x = 2; // SEMENTE FIXA x0=2
    int y = 2; // SEMENTE FIXA x0=2
    int d = 1;
    int iteracao = 1;
    int max_iteracoes = 1000;
    
    while (d == 1 && iteracao <= max_iteracoes) {
        printf("\n--- Iteracao %d ---\n", iteracao);
        
        // g(x) = (x² + 1) mod n
        int x_antigo = x;
        int y_antigo = y;
        
        x = (x * x + 1) % n;
        y = (y * y + 1) % n;
        y = (y * y + 1) % n;
        
        printf("x%d = (%d^2 + 1) mod %d = %d\n", iteracao, x_antigo, n, x);
        printf("y%d = g(g(%d)) = %d\n", iteracao, y_antigo, y);
        
        int diff = absoluto(x - y);
        printf("|x%d - y%d| = |%d - %d| = %d\n", iteracao, iteracao, x, y, diff);
        
        d = calcularMDC(diff, n);
        
        iteracao++;
        
        if (d == n) {
            printf("Fator nao trivial nao encontrado (d == n).\n");
            // Com semente fixa, não reiniciamos com valores aleatórios
            // Continuamos com a mesma sequência determinística
            d = 1;
        }
    }
    
    if (iteracao > max_iteracoes) {
        printf("AVISO: Limite de iteracoes atingido.\n");
        // Método de força bruta como fallback
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                printf("Fator encontrado (forca bruta): %d\n", i);
                return i;
            }
        }
        printf("Nao foi possivel fatorar %d.\n", n);
        return -1;
    }
    
    printf("Fator primo encontrado: %d\n", d);
    return d;
}

// Algoritmo Estendido de Euclides para encontrar inverso modular
int inversoModular(int a, int m) {
    printf("\n------------CALCULANDO INVERSO MODULAR DE %d mod %d------------\n", a, m);
    
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

// Função para exponenciação modular com verificação de teoremas
int exponenciacaoModular(int base, int expoente, int modulo, const char* contexto) {
    printf("\n------------EXPONENCIACAO MODULAR: %d^%d mod %d (%s)------------\n", 
           base, expoente, modulo, contexto);
    
    // Verificar qual teorema aplicar
    if (ehPrimo(modulo)) {
        printf("APLICANDO PEQUENO TEOREMA DE FERMAT (n eh primo)\n");
        printf("Como %d eh primo, %d^(%d-1) ≡ 1 mod %d\n", modulo, base, modulo, modulo);
        int exp_reduzido = expoente % (modulo - 1);
        printf("Expoente reduzido: %d mod %d = %d\n", expoente, modulo-1, exp_reduzido);
        expoente = exp_reduzido;
    } else if (calcularMDC(base, modulo) == 1) {
        printf("APLICANDO TEOREMA DE EULER (MDC(M, n) = 1)\n");
        // Para φ(n) precisaríamos dos fatores de modulo
        printf("φ(%d) necessitaria fatoracao de n\n", modulo);
    } else {
        printf("APLICANDO TEOREMA DA DIVISAO EUCLIDIANA\n");
        printf("Expoente sera processado diretamente\n");
    }
    
    // Implementação da exponenciação modular binária
    long long resultado = 1;
    long long base_temp = base % modulo;
    int exp_temp = expoente;
    
    printf("\nCalculo passo a passo (exponenciacao binaria):\n");
    printf("Resultado = 1, Base = %lld, Expoente = %d\n", base_temp, exp_temp);
    
    while (exp_temp > 0) {
        if (exp_temp % 2 == 1) {
            long long resultado_antigo = resultado;
            resultado = (resultado * base_temp) % modulo;
            printf("Expoente impar: Resultado = %lld * %lld mod %d = %lld\n", 
                   resultado_antigo, base_temp, modulo, resultado);
        } else {
            printf("Expoente par: Apenas eleva base ao quadrado\n");
        }
        
        long long base_antiga = base_temp;
        base_temp = (base_temp * base_temp) % modulo;
        printf("Base = %lld^2 mod %d = %lld\n", base_antiga, modulo, base_temp);
        
        exp_temp /= 2;
        printf("Expoente = %d / 2 = %d\n\n", exp_temp * 2, exp_temp);
    }
    
    printf("Resultado final: %lld\n", resultado);
    return (int)resultado;
}

// Função para converter caractere para código numérico
int charParaCodigo(char c) {
    c = toupper(c);
    if (c >= 'A' && c <= 'Z') {
        return 11 + (c - 'A');
    } else if (c == ' ') {
        return 0;
    }
    return -1;
}

// Função para converter código numérico para caractere
char codigoParaChar(int codigo) {
    if (codigo == 0) return ' ';
    if (codigo >= 11 && codigo <= 36) {
        return 'A' + (codigo - 11);
    }
    return '?';
}

// Função para validar mensagem
int mensagemValida(const char* mensagem) {
    for (int i = 0; mensagem[i] != '\0'; i++) {
        if (!isalpha(mensagem[i]) && mensagem[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Função para pré-codificar mensagem
int* preCodificarMensagem(const char* mensagem, int* tamanho) {
    int len = strlen(mensagem);
    *tamanho = len;
    int* codigos = (int*)malloc(len * sizeof(int));
    
    printf("\n------------PRE-CODIFICACAO DA MENSAGEM------------\n");
    printf("Mensagem original: '%s'\n", mensagem);
    printf("Tabela de conversao: A=11, B=12, ..., Z=36, ESPACO=00\n");
    printf("Conversao:\n");
    
    for (int i = 0; i < len; i++) {
        codigos[i] = charParaCodigo(mensagem[i]);
        printf("  %c -> %02d\n", toupper(mensagem[i]), codigos[i]);
    }
    
    return codigos;
}

// Função para codificar mensagem com RSA
int* codificarRSA(const int* mensagemCodificada, int tamanho, int e, int n, int* tamanhoCodificado) {
    *tamanhoCodificado = tamanho;
    int* codificado = (int*)malloc(tamanho * sizeof(int));
    
    printf("\n------------CODIFICACAO RSA------------\n");
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
    
    printf("\n------------DECODIFICACAO RSA------------\n");
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
    
    printf("\n------------CONVERSAO PARA TEXTO------------\n");
    
    for (int i = 0; i < tamanho; i++) {
        texto[i] = codigoParaChar(codigos[i]);
        printf("%02d -> '%c'\n", codigos[i], texto[i]);
    }
    texto[tamanho] = '\0';
    
    return texto;
}

// Função para solicitar e validar número composto
int solicitarNumeroComposto(const char* nome) {
    int numero;
    int tentativas = 0;
    
    while (tentativas < 3) {
        printf("Digite o %s (100-9999, deve ser composto): ", nome);
        
        if (scanf("%d", &numero) != 1) {
            printf("ERRO: Digite um numero valido.\n");
            while (getchar() != '\n');
            tentativas++;
            continue;
        }
        
        if (!temTresOuQuatroDigitos(numero)) {
            printf("ERRO: O numero deve ter 3 ou 4 digitos (100-9999).\n");
            tentativas++;
            continue;
        }
        
        if (!ehComposto(numero)) {
            printf("ERRO: %d nao e composto. Exemplos: 323, 437, 899\n", numero);
            tentativas++;
            continue;
        }
        
        return numero;
    }
    
    printf("Usando valor padrao 323.\n");
    return 323;
}

// Função para solicitar e validar mensagem
void solicitarMensagem(char* mensagem, int tamanhoMax) {
    int tentativas = 0;
    
    while (tentativas < 3) {
        printf("Digite a mensagem (apenas letras e espacos): ");
        getchar();
        fgets(mensagem, tamanhoMax, stdin);
        mensagem[strcspn(mensagem, "\n")] = 0;
        
        if (strlen(mensagem) == 0) {
            printf("ERRO: Mensagem vazia.\n");
            tentativas++;
            continue;
        }
        
        if (!mensagemValida(mensagem)) {
            printf("ERRO: Use apenas letras e espacos.\n");
            tentativas++;
            continue;
        }
        
        return;
    }
    
    printf("Usando mensagem padrao 'RSA'.\n");
    strcpy(mensagem, "RSA");
}

int main() {
    printf("------------SISTEMA RSA COMPLETO------------\n");
    printf("METODO ρ DE POLLARD COM SEMENTE FIXA x0 = 2\n\n");
    
    // Etapa 1: Fatoração
    printf("------------ETAPA 1: FATORACAO COM METODO ρ DE POLLARD------------\n");
    
    int N1 = solicitarNumeroComposto("primeiro numero composto N1");
    int N2 = solicitarNumeroComposto("segundo numero composto N2");
    
    if (N1 == N2) {
        printf("AVISO: N1 e N2 iguais! Ajustando N2 para 437.\n");
        N2 = 437;
    }
    
    printf("\nNumeros selecionados: N1 = %d, N2 = %d\n", N1, N2);
    
    int p = pollardRho(N1);
    int q = pollardRho(N2);
    
    if (p == -1 || q == -1) {
        printf("ERRO na fatoracao. Usando valores padroes p=17, q=19.\n");
        p = 17;
        q = 19;
    }
    
    if (p == q) {
        printf("AVISO: p e q iguais! Ajustando q=19.\n");
        q = 19;
    }
    
    printf("\n------------FATORES ENCONTRADOS------------\n");
    printf("p = %d (fator de N1=%d)\n", p, N1);
    printf("q = %d (fator de N2=%d)\n", q, N2);
    
    // Etapa 2: Geração das Chaves RSA
    printf("\n------------ETAPA 2: GERACAO DAS CHAVES RSA------------\n");
    
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
    
    printf("\n------------CHAVES RSA------------\n");
    printf("Chave Publica: (n=%d, E=%d)\n", n, E);
    printf("Chave Privada: (n=%d, D=%d)\n", n, D);
    
    // Etapa 3: Codificação e Decodificação
    printf("\n------------ETAPA 3: CODIFICACAO E DECODIFICACAO------------\n");
    
    char mensagem[100];
    solicitarMensagem(mensagem, sizeof(mensagem));
    
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
    
    printf("\n------------RESULTADO------------\n");
    printf("Mensagem original: '%s'\n", mensagem);
    printf("Mensagem decodificada: '%s'\n", textoFinal);
    
    if (strcmp(mensagem, textoFinal) == 0) {
        printf("SUCESSO: Mensagem decodificada identica a original!\n");
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