// Gabriel Santos de Andrade   RA 815407
// Leonardo Prado Silva        RA 813169
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long x, y;
} Ponto;

const Ponto O = {0, 0};

int ponto_igual_O(Ponto P) {
    return (P.x == O.x && P.y == O.y);
}

long long mod(long long a, long long p) {
    long long resto = a % p;
    // Mantém operações entre 0 e p-1
    return (resto < 0) ? resto + p : resto; // <- único exemplo de linguagem funcional em C (Zé que disse)
}

long long fermat(long long a, long long p) {
    long long resto = 1, expoente = p - 2;
    a = mod(a, p); // a^1 -> a^(2^1)
    while (expoente > 0) {
        if (expoente % 2 == 1) // se o bit menos significativo é 1
            resto = mod(resto * a, p); // multiplica o resultado atual por a^(2^i)
        a = mod(a * a, p); // a^(2^i) * a^(2^i) = a^(2^(i+1))
        expoente /= 2;
    }
    return resto;
}

Ponto soma(Ponto P, Ponto Q, long long a, long long p) {
    if (ponto_igual_O(P)) // se P é O retorna Q
        return Q;
    if (ponto_igual_O(Q)) // se Q é O retorna P
        return P;
    if (P.x == Q.x && (P.y + Q.y) % p == 0) // se P == -Q retorna O (reta entre os pontos é vertical)
        return O;

    /* 
    se P == Q e Q.y != 0:
        m = (3*x1^2 + a) / (2*y1) mod p === m = (3*x1^2 + a) * (2*y1)^(-1) mod p
    se P != Q:
        se x1 == x2: 
            retorna O
        m = (y2 - y1) / (x2 - x1) mod p === m = (y2 - y1) * (x2 - x1)^(-1) mod p
    */

    long long lambda;
    if (P.x == Q.x && P.y == Q.y && Q.y != 0) 
        lambda = mod((3 * mod(P.x * P.x, p) + a) * fermat(2 * P.y, p), p);
    else {
        if (P.x == Q.x) 
            return O;
        lambda = mod((mod(Q.y - P.y, p)) * fermat(mod(Q.x - P.x, p), p), p);
    }

    long long x3 = mod(mod(lambda * lambda, p) - P.x - Q.x, p);
    long long y3 = mod(lambda * mod(P.x - x3, p) - P.y, p);
    return (Ponto){x3, y3};
}

Ponto multiplicacao_escalar(Ponto P, long long k, long long a, long long p) {
    Ponto R = O;
    Ponto Q = P;

    while (k > 0) {
        if (k % 2 == 1) // se o bit menos significativo é 1
            R = soma(R, Q, a, p); // adiciona Q ao resultado
        Q = soma(Q, Q, a, p); // dobra Q
        k /= 2;
    }
    return R;
}

int main() {
    long long n, a, p, x, y;
    while (scanf("%lld", &n) == 1 && n != 0) {
        scanf("%lld %lld %lld %lld", &a, &p, &x, &y);
        Ponto G = {mod(x,p), mod(y,p)};
        Ponto R = multiplicacao_escalar(G, n, a, p);
        printf("%lld %lld\n", R.x, R.y);
    }
    return 0;
}

/*
Exemplo de entrada          Saída para exemplo de entrada
2                           
3 13 2 10                   12 2

3
3 13 2 10                   9 6

9
3 13 2 10                   0 0

5
10 13 3 6                   10 0

6
10 13 3 6                   1 9

10
10 13 3 6                   0 0

0
*/