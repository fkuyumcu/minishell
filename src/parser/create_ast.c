/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/25 15:23:21 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**collect_redirection_args(token_t tokens[], int *pos, minishell_t *ms)
{
	char	**args;
	int		arg_count;

	arg_count = 0;
	args = NULL;

	if (*pos < ms->size && tokens[*pos].t_type == WORD)
	{
		args = malloc(sizeof(char *) * 2);
		args[0] = ft_strdup(tokens[*pos].value, ms);
		args[1] = NULL;
		(*pos)++;
	}

	return (args);
}


char	**ft_realloc(char **args, size_t new_size, minishell_t *minishell)
{
	size_t	i;
	size_t	old_size;
	char	**tmp;

	i = -1;
	old_size = 0;
	if (args)
	{
		while (args[old_size])
			old_size++;
	}
	tmp = malloc((old_size + new_size + 1) * sizeof(char *));
	if (!tmp)
		ft_error(minishell, "Error Allocating Space");
	while (++i < old_size)
		tmp[i] = ft_strdup(args[i], minishell);
	i = 0;
	while (i < old_size)
		free(args[i++]);
	free(args);
	tmp[old_size + new_size] = NULL;
	return (tmp);
}

char	**collect_args(token_t tokens[], int *pos, minishell_t *ms)
{
	char	**args;
	int		arg_count;

	arg_count = 0;
	args = NULL;
	if (ms->size == 1 && ft_strlen(tokens[0].value) == 1)
	{
		args = malloc(sizeof(char *) * 2);
		args[0] = ft_strdup(tokens[0].value, ms);
		(*pos)++;
		args[1] = NULL;
		return (args);
	}
	while (*pos < ms->size && tokens[*pos].t_type == WORD)
	{
		args = ft_realloc(args, 1, ms);
		args[arg_count] = ft_strdup(tokens[*pos].value, ms);
		(*pos)++;
		arg_count++;
	}

	return (args);
}



ast_node_t	*parse_primary(token_t tk[], int *pos, minishell_t *ms)
{
	char	**args;

	args = NULL;
	if (*pos >= ms->size)
		return (NULL);
	if (tk[*pos].t_type == WORD)
	{
		args = collect_args(tk, pos, ms);
		return (create_ast_node(args, WORD, ms));
	}
	return (NULL);
}

ast_node_t	*parse_expression(token_t tokens[], int *pos, int min_prec, minishell_t *ms)
{
	ast_node_t	*left;
	ast_node_t	*right;
	token_type	type;
	ast_node_t	*new_node;
	int prec;

	left = parse_primary(tokens, pos, ms);
	while (*pos < ms->size)
	{
		type = tokens[*pos].t_type;
		prec = get_precedence(type);
		if (prec < min_prec) 
			break;

		(*pos)++;

		if (type == REDIRECT_OUT || type == REDIRECT_IN || type == HEREDOC_OUT || type == HEREDOC_IN)
		{
			char **args = collect_redirection_args(tokens, pos, ms);
			new_node = create_ast_node(args, type, ms);
		}
		else
		{
			right = parse_expression(tokens, pos, prec + 1, ms);
			if (!right)
			{
				printf("Hata: Geçersiz ifade\n");
				free_tree(left);
				return (NULL);
			}
			new_node = create_ast_node(NULL, type, ms);
			new_node->right = right;
		}

		new_node->left = left;
		left = new_node;
	}	
	return (left);
}


int	get_precedence(token_type type) 
{
	//ÖNCELİKLER BURADA TANIMLANMALI
	if (type == PIPE)
		return (1);
	else if (type == REDIRECT_OUT || REDIRECT_IN)
		return (2);
	else if (type == HEREDOC_OUT || HEREDOC_IN)
		return (3);
	return (0);
}

ast_node_t	*create_ast_node(char **args, token_type type, minishell_t *ms)
{
	ast_node_t *node;

	node = (ast_node_t *)malloc(sizeof(ast_node_t));
	if (!node) 
		ft_error(ms, "err");
	node->token = type;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
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
