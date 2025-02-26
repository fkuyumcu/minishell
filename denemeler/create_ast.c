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



/* ast_node_t *create_ast_node(char **args, token_type type);

int get_precedence(token_type type);
ast_node_t *parse_expression(token_t tokens[], int *pos, int size, int min_prec);
ast_node_t *parse_redirection(token_t tokens[], int *pos, int size, token_type redir_type);
char **collect_args(token_t tokens[], int *pos, int size);
void print_ast(ast_node_t *node, int level);


char **collect_args(token_t tokens[], int *pos, int size)
{
    char **args = NULL;
    int arg_count = 0;

    while (*pos < size && tokens[*pos].t_type == WORD) {
        args = realloc(args, sizeof(char *) * (arg_count + 1));
        args[arg_count] = ft_strdup(tokens[*pos].value);
        (*pos)++;
        arg_count++;
    }

    if (args) {
        args = realloc(args, sizeof(char *) * (arg_count + 1));
        args[arg_count] = NULL; // Son eleman NULL olmalı
    }

    return args;
}

ast_node_t *parse_redirection(token_t tokens[], int *pos, int size, token_type redir_type) 
{
    (*pos)++; // Yönlendirme operatörünü geç

    if (*pos >= size || tokens[*pos].t_type != WORD) {
        printf("Hata: Yönlendirme sonrası dosya adı bekleniyordu\n");
        return NULL;
    }

    char **args = malloc(2 * sizeof(char *));
    args[0] = ft_strdup(tokens[*pos].value);
    args[1] = NULL;
    (*pos)++;

    ast_node_t *redir_node = create_ast_node(NULL, redir_type);
    redir_node->right = create_ast_node(args, WORD); // Dosya adı sağa bağlanır
    return redir_node;
}

ast_node_t *parse_primary(token_t tokens[], int *pos, int size) {
    if (*pos >= size) return NULL;

    // Yönlendirmeleri kontrol et (<<, >>, <, >)
    if (tokens[*pos].t_type == REDIRECT_IN || tokens[*pos].t_type == REDIRECT_OUT ||
        tokens[*pos].t_type == HEREDOC_IN || tokens[*pos].t_type == HEREDOC_OUT) {
        return parse_redirection(tokens, pos, size, tokens[*pos].t_type);
    }

    // Komut ve argümanları topla (ls -l -s gibi)
    if (tokens[*pos].t_type == WORD) {
        char **args = collect_args(tokens, pos, size);
        return create_ast_node(args, WORD);
    }

    return NULL; // Geçersiz ifade
}

ast_node_t *parse_expression(token_t tokens[], int *pos, int size, int min_prec)
 {
    ast_node_t *left = parse_primary(tokens, pos, size); // İlk komutu veya yönlendirmeyi ayrıştır

    while (*pos < size) {
        token_type type = tokens[*pos].t_type;
        int prec = get_precedence(type);

        if (prec < min_prec) break; // Öncelik yeterli değilse dur

        (*pos)++; // Operatörü geç

        ast_node_t *right = parse_expression(tokens, pos, size, prec + 1); // Sağ tarafı ayrıştır
        if (!right) {
            printf("Hata: Geçersiz ifade\n");
            return NULL;
        }

        ast_node_t *new_node = create_ast_node(NULL, type); // Operatör düğümü oluştur
        new_node->left = left;
        new_node->right = right;
        left = new_node; // Yeni düğüm artık mevcut AST'nin kökü
    }

    return left;
}


int get_precedence(token_type type) 
{
    switch (type) {
        case PIPE: return 1;
        case REDIRECT_OUT: return 2;
        case REDIRECT_IN: return 2;
        case HEREDOC_IN: return 3;
        case HEREDOC_OUT: return 3;
        default: return 0; // WORD veya diğerleri için öncelik yok
    }
}


ast_node_t *create_ast_node(char **args, token_type type) 
{
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) {
        perror("malloc hatası: AST düğümü oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    node->token = type;
    node->args = args;  // args doğrudan atanır (önceden oluşturulmuş olmalı)
    node->left = NULL;
    node->right = NULL;

    return node;
}

void print_ast(ast_node_t *node, int level)
{
    if (!node) return;

    for (int i = 0; i < level; i++) printf("  ");
    printf("Type: %d, Args: ", node->token);

    if (node->args) {
        for (int i = 0; node->args[i]; i++) {
            printf("%s ", node->args[i]);
        }
    }
    printf("\n");

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}

 */
/*
	int pos = 0;
     ast_node_t *ast = parse_expression(tokens, &pos, 5, 0);

    if (ast == NULL) {
        printf("Hata: AST oluşturulamadı\n");
        return;
    }

    print_ast(ast, 0); */