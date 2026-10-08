# 🔍 Analisador Léxico em C (Portugol / Pseudocódigo)

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Compilers](https://img.shields.io/badge/Domain-Compilers%20%26%20Lexers-purple.svg)
![Status](https://img.shields.io/badge/Status-Conclu%C3%ADdo-brightgreen.svg)

## 📌 Visão Geral
Este projeto consiste na implementação de um **Analisador Léxico (Scanner / Tokenizer)** desenvolvido em **C**, projetado para reconhecer e categorizar tokens de uma linguagem de programação baseada em **Portugol / pseudocódigo estruturado**. 

O componente representa a primeira fase fundamental da arquitetura de um compilador, sendo responsável por ler o fluxo de caracteres de entrada (via arquivo ou `stdin`), filtrar delimitadores/espaços em branco e convertê-los em uma sequência estruturada de tokens tipados com rastreamento preciso de localização (linha e coluna).

---

## 🚀 Funcionalidades Principais
- **Classificação Completa de Tokens**:
  - `TOKEN_KEYWORD`: Palavras reservadas da linguagem (`programa`, `inicio`, `fim`, `se`, `senao`, `enquanto`, `para`, `escreva`, `leia`, etc.).
  - `TOKEN_IDENTIFIER`: Variáveis, procedimentos e identificadores definidos pelo usuário.
  - `TOKEN_INTEGER` & `TOKEN_FLOAT`: Constantes numéricas inteiras e de ponto flutuante.
  - `TOKEN_STRING`: Literais de texto delimitados por aspas.
  - `TOKEN_OPERATOR`: Operadores aritméticos, relacionais e lógicos (`+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<=`, `>=`, etc.).
  - `TOKEN_DELIMITER`: Pontuação e delimitadores estruturais (`;`, `,`, `(`, `)`, `{`, `}`, `[`, `]`).
  - `TOKEN_ERROR`: Tratamento e captura de caracteres inválidos.
- **Estrutura de Dados Dinâmica**: Implementação de `TokenList` como vetor redimensionável em memória (dynamic array) para alocação eficiente dos tokens.
- **Rastreamento de Posição**: Informa linha e coluna exatas de cada token, facilitando diagnósticos sintáticos e semânticos.
- **Flexibilidade de Entrada**: Suporte a execução passando arquivo por parâmetro ou via pipe/entrada padrão (`stdin`).
- **Gerenciamento de Memória**: Desalocação completa de memória dinâmica (`malloc`/`free`) garantindo zero vazamento (memory leak).

---

## 🛠️ Tecnologias e Ferramentas
- **Linguagem**: C (Padrão C99 / C11)
- **Compilador**: GCC / Clang / MSVC
- **Bibliotecas Standard**: `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<ctype.h>`

---

## 📂 Estrutura do Repositório
```plaintext
analisador-lexico/
└── lexer.c       # Implementação completa do tokenizer, estruturas e rotinas de teste
```

---

## ⚙️ Como Executar o Projeto Localmente

### Pré-requisitos
- Compilador C (GCC, Clang ou MinGW no Windows).

### Compilação
Abra o terminal no diretório do projeto e compile:
```bash
gcc -Wall -Wextra -std=c99 lexer.c -o lexer
```

### Execução

1. **Executando com um arquivo fonte:**
```bash
./lexer codigo_exemplo.txt
```

2. **Executando interativamente:**
```bash
./lexer
```

### Exemplo de Saída
```plaintext
Line   | Col    | Type         | Lexeme
-------|--------|--------------|-----------------
1      | 1      | KEYWORD      | programa
1      | 10     | KEYWORD      | inicio
1      | 17     | KEYWORD      | se
1      | 20     | IDENTIFIER   | x
1      | 22     | OPERATOR     | >
1      | 24     | INTEGER      | 10
1      | 27     | KEYWORD      | entao
...
```

---

## 👨‍💻 Autor
Desenvolvido por **Luca Samuel dos Santos** ([@LucaS4nt0s](https://github.com/LucaS4nt0s)).
