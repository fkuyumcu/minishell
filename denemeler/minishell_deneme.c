/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_deneme.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:54:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/13 19:06:54 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <readline/readline.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 100

// minishell ana struct
typedef struct minishell 
{
	int	single_quote_flag;
	int double_quote_flag;
}
minikshell;

// Token türleri
typedef enum {
    TOKEN_WORD,       // Komut veya argüman
    TOKEN_PIPE,       // "|"
    TOKEN_REDIRECT_IN,  // "<"
    TOKEN_REDIRECT_OUT, // ">"
    TOKEN_AMPERSAND,  // "&"
    TOKEN_END         // NULL göstergesi
} TokenType;

// Token yapısı
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int tokenize(const char *input, Token tokens[]) {
    int pos = 0, token_count = 0;
    while (input[pos] != '\0') {
        // Boşlukları atla
        while (isspace(input[pos])) pos++;

        if (input[pos] == '\0') break;

        if (input[pos] == '|') {
            tokens[token_count++] = (Token){TOKEN_PIPE, "|"};
            pos++;
        } else if (input[pos] == '<') {
            tokens[token_count++] = (Token){TOKEN_REDIRECT_IN, "<"};
            pos++;
        } else if (input[pos] == '>') {
            tokens[token_count++] = (Token){TOKEN_REDIRECT_OUT, ">"};
            pos++;
        } else if (input[pos] == '&') {
            tokens[token_count++] = (Token){TOKEN_AMPERSAND, "&"};
            pos++;
        } else {
            // Normal kelimeyi oku
            int start = pos;
            while (input[pos] != '\0' && !isspace(input[pos]) && input[pos] != '|' &&
                   input[pos] != '<' && input[pos] != '>' && input[pos] != '&') {
                pos++;
            }
            int length = pos - start;
            if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, input + start, length);
            tokens[token_count].value[length] = '\0';
            tokens[token_count].type = TOKEN_WORD;
            token_count++;
        }
    }

    // Sonlandırıcı token
    tokens[token_count] = (Token){TOKEN_END, ""};
    return token_count;
}

// Tokenleri yazdıran fonksiyon
void print_tokens(Token tokens[]) {
    int i = 0;
    while (tokens[i].type != TOKEN_END) {
        printf("Token: %-15s Type: %d\n", tokens[i].value, tokens[i].type);
        i++;
    }
}

// tırnakları flagleme işlemi
void quote_checker(minikshell *a, char *str)
{
    int i = 0;
    a->double_quote_flag = 0;
    a->single_quote_flag = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            if (str[i] == '"' && a->single_quote_flag % 2 == 0)
            {
                a->double_quote_flag++;
            }
            else if (str[i] == '\'' && a->double_quote_flag % 2 == 0)
            {
                a->single_quote_flag++;
            }
        }
        i++;
    }
    if (a->double_quote_flag % 2 != 0 || a->single_quote_flag % 2 != 0)
        write(1, "error\n", 6);
}
// ctrl-c işlemini yapan fonksiyon
void sigint_handler(int signum)
{
	rl_on_new_line();         
    rl_replace_line("", 0);   
    rl_redisplay();           
    printf("\nminishell> "); 
}

// Ana fonksiyon
int main() {
    char input[256];
    /* char *input;    readline ı eşitleyeceğimiz için pointer kullanılıyor array değil*/

    while (1) {
       /* input = readline("minishell> ");
        if (!input)
			exit(1); */
	quote_checker(&a,input);
        printf("minishell> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // Satır sonu karakterini kaldır
        input[strcspn(input, "\n")] = 0;

        Token tokens[MAX_TOKENS];
        int count = tokenize(input, tokens);

        print_tokens(tokens);
    }

    return 0;
}
