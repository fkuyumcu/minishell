/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/24 15:16:14 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ast_node_t *create_node(token_type type, char **args)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    node->token = type;
    node->args = args;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int get_precedence(token_t token)
{
    int type;
    
    type = token.t_type;
    if(type == PIPE)
        return (1);
    else if(type == REDIRECT_IN || type == REDIRECT_OUT)
        return (2);
    else if(type == HEREDOC_IN || HEREDOC_OUT)
        return (3);
    return (0); 
}

char **edit_command_args(token_t *tokens, int *pos, int size)
{
    int start;
    int count; 
    char **args;
    int i;

    start = 0;
    *pos = 0;
    count = 0;
    i = -1;

    while(*pos < size && tokens[*pos].t_type == WORD)
    {
        (*pos)++;
        count ++;
    }
    args = malloc(sizeof(char *) * (count + 1));
    if(!args)
        return (NULL);
    while(++i < count)
        args[i] = tokens[start + i].value;
    args[count] = NULL;
    return (args);
}

/* ast_node_t *parse_expression(token_t tokens[], int *pos, int size, int min_prec)
{
    ast_node_t *left;

    if (tokens[*pos].t_type == HEREDOC_IN || tokens[*pos].t_type == HEREDOC_OUT ||
        tokens[*pos].t_type == REDIRECT_IN || tokens[*pos].t_type == REDIRECT_OUT) {
        
        token_type op_type = tokens[*pos].t_type;
        (*pos)++;  // Operatörü geç
        
        // Hemen ardından bir dosya adı (WORD) gelmelidir
        if (tokens[*pos].t_type != WORD) {
            printf("Hata: Beklenmeyen token!\n");
            return NULL;
        }

        // Dosya adını içeren node'u oluştur
        char **args = malloc(2 * sizeof(char *));
        args[0] = strdup(tokens[*pos].value);
        args[1] = NULL;
        (*pos)++;

        // Yönlendirme operatörünü içeren düğümü oluştur
        left = create_ast_node(NULL, op_type);
        left->right = create_ast_node(args, WORD);
    }
    // Eğer kelimeyle başlıyorsa, bunu bir komut olarak al
    else if (tokens[*pos].t_type == WORD) {
        char **args = malloc(2 * sizeof(char *));
        args[0] = strdup(tokens[*pos].value);
        args[1] = NULL;
        left = create_ast_node(args, WORD);
        (*pos)++;
    } else {
        return NULL;  // Geçersiz ifade
    }


} */






