/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/01 15:08:19 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **collect_args(token_t tokens[], int *pos, int size)
{
    char **args = NULL;
    int arg_count = 0;

    while (*pos < size && tokens[*pos].t_type == WORD)
    {
        args = realloc(args, sizeof(char *) * (arg_count + 1));//realloc
        args[arg_count] = ft_strdup(tokens[*pos].value);
        (*pos)++;
        arg_count++;
    }

    if (args) {
        args = realloc(args, sizeof(char *) * (arg_count + 1));//null kısmı eklemek için
        args[arg_count] = NULL; 
    }

    return args;
}

ast_node_t *parse_redirection(token_t tokens[], int *pos, int size, token_type redir_type) {
    char **args;
    ast_node_t *redir_node;

    (*pos)++;
    if (*pos >= size || tokens[*pos].t_type != WORD) {
        printf("Hata: Yönlendirme sonrası dosya adı bekleniyordu\n");
        return NULL;
    }
    redir_node = create_ast_node(NULL, redir_type);
    redir_node->right = create_ast_node(args, WORD);

    return (redir_node);
}

ast_node_t *parse_primary(token_t tokens[], int *pos, int size) {

    char **args;


    if (*pos >= size)
        return (NULL);

    if (tokens[*pos].t_type == REDIRECT_IN || tokens[*pos].t_type == REDIRECT_OUT ||
        tokens[*pos].t_type == HEREDOC_IN || tokens[*pos].t_type == HEREDOC_OUT) {
        return (parse_redirection(tokens, pos, size, tokens[*pos].t_type));
    }

    if (tokens[*pos].t_type == WORD) {
        args = collect_args(tokens, pos, size);
        return (create_ast_node(args, WORD));
    }
	
    return (NULL); 
}

ast_node_t *parse_expression(token_t tokens[], int *pos, int size, int min_prec)
 {
    ast_node_t *left;
    ast_node_t *right;
    int prec;
    token_type type;

    left = parse_primary(tokens, pos, size);
    while (*pos < size)
    {
        type = tokens[*pos].t_type;
        prec = get_precedence(type);
        if (prec < min_prec) 
            break;
        (*pos)++;
        right = parse_expression(tokens, pos, size, prec + 1);
        if (!right)
        {
            printf("Hata: Geçersiz ifade\n");
            return (NULL);
        }
        ast_node_t *new_node = create_ast_node(NULL, type);
        new_node->left = left;
        new_node->right = right;
        left = new_node;
    }

    return left;
}


int get_precedence(token_type type) 
{
    //ÖNCELİKLER BURADA TANIMLANMALI
    if(type == PIPE)
        return (1);
    else if(type == REDIRECT_OUT || REDIRECT_IN)
        return (2);
    else if(type == HEREDOC_OUT || HEREDOC_IN)
        return (3);
    return (0);
    
}


ast_node_t *create_ast_node(char **args, token_type type) 
{
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) 
        printf("error");//error fonksiyonuna gönder

    node->token = type;
    node->args = args;
    node->left = NULL;
    node->right = NULL;
	int i  = 0;
    return (node);
}

void print_ast(ast_node_t *node, int depth) {
    if (!node) return;

    // Girintiyi ayarla
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Düğümün türünü yazdır
    switch (node->token) {
        case WORD:
            printf("WORD: ");
            if (node->args) {
                for (int i = 0; node->args[i]; i++) {
                    printf("%s ", node->args[i]);
                }
            }
            printf("\n");
            break;
        case PIPE:
            printf("PIPE |\n");
            break;
        case REDIRECT_IN:
            printf("REDIRECT_IN <\n");
            break;
        case REDIRECT_OUT:
            printf("REDIRECT_OUT >\n");
            break;
        case HEREDOC_IN:
            printf("HEREDOC_IN <<\n");
            break;
        case HEREDOC_OUT:
            printf("HEREDOC_OUT >>\n");
            break;
        default:
            printf("UNKNOWN NODE\n");
            break;
    }

    // Sol ve sağ çocukları yazdır
    if (node->left) {
        print_ast(node->left, depth + 1);
    }
    if (node->right) {
        print_ast(node->right, depth + 1);
    }
}
 

