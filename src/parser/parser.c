/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/20 18:18:03 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static void do_something2(char *str, char q, int **in_q)
{
    if (*str == q)
        **in_q = 0;
}

static void do_something(int **a, int **b)
{
    (**a)++;
    **b = 1;
}

int count_word(char *str, int *count, int *in_word, int *in_quotes)
{
    char quote_char;

    quote_char = '\0';
    while (*str)
    {
        if (*in_quotes)
            do_something2(str, quote_char, &in_quotes);
        else
        {
            if (*str == '"' || *str == '\'')
            {
                *in_quotes = 1;
                quote_char = *str;
                if (!*in_word)
                    do_something(&count, &in_word);
            }
            else if (*str == ' ' || *str == '\t' || *str == '\n')
                *in_word = 0;
            else if (!*in_word)
                do_something(&count, &in_word);
        }
        str++;
    }
    return (*count);
}


void parser(char *buf)
{
     minishell_t minishell;
    int c;
    int in_word;
    int in_quotes;
    int allocation;
    env_t *env_list;
    c = 0;
    in_word = 0;
    in_quotes = 0;

    allocation = sizeof(token_t)* count_word(buf, &c, &in_word, &in_quotes);
    minishell.allocation = allocation;
    minishell.env_list = env_list;
    token_t tokens[allocation];
    lex_analize(buf, tokens, &minishell);
    printf("%s", tokens[0].value);
    printf("%s", tokens[1].value);
    printf("%s", tokens[2].value);
    printf("%s", tokens[3].value);
    printf("%s", tokens[4].value);
    printf("%s", tokens[5].value);
    printf("%s", tokens[6].value);
    printf("%s", tokens[7].value);
    printf("%s", tokens[8].value);


}