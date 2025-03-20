/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:18:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/20 14:29:59 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

void execute_pipe(ast_node_t *node, minishell_t *ms)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;

    if (pipe(fd) == -1)
    {
        perror("pipe");//perror yazıldı
        return;
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        return;
    }

    if (pid1 == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); 
        close(fd[1]); 

        execute_ast(node->left, ms);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        return;
    }

    if (pid2 == 0) 
    {
        close(fd[1]); 
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]); 

        execute_ast(node->right, ms);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


void execute_redir_in(ast_node_t *node, minishell_t *ms)
{
    pid_t pid;
    int fd;

    if (!node || !node->right || !node->right->args[0] || !node->left)
        return;

    fd = open(node->right->args[0], O_RDONLY);
    if (fd == -1)
    {
        printf("Cannot Find File\n");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        printf("Fork Error\n");
        close(fd);
        return;
    }

    if (pid == 0)
    {
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            printf("Dup2 Error");
            close(fd);
            exit(1);
        }
        close(fd);  
        execute_word(node->left, ms);
        free_tree(node);  
        exit(0);
    }
    else
    {
        close(fd);
        waitpid(pid, NULL, 0);
    }
}

void execute_redir_out(ast_node_t *node, minishell_t *ms)
{
    pid_t pid;
    int fd;

    if (!node || !node->right || !node->right->args[0] || !node->left)
        return;
    if (fd == -1)
    {
        perror("open");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(fd);
        return;
    }
    if (pid == 0)  // Child process
    {
        if (node->left->token == HEREDOC_IN)
        {
            execute_heredoc(node->left, ms, node->right->args[0]);
            free_tree(node);
            exit(0);
        }
        else
        {
            fd = open(node->right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
            execute_ast(node->left, ms); // Bu çağrı execute_heredoc'u da içerebilir
            close(fd);
            free_tree(node);
            exit(0);    
        }
    }
    close(fd);
    waitpid(pid, NULL, 0);
}




void execute_heredoc(ast_node_t *node, minishell_t *ms, char *out)
{
    
    handle_heredoc(node->right->args[0], node->left, ms, out);
}


void execute_heredoc_out(ast_node_t *node, minishell_t *ms)
{


}


void	execute(minishell_t *ms)
{

execute_ast(ms->ast, ms);


}


void execute_ast(ast_node_t *node, minishell_t *ms)
{
    if(!node)
        return ;
    if(node->token == PIPE)
        execute_pipe(node, ms);
    else if(node->token == REDIRECT_IN)
        execute_redir_in(node, ms);
    else if(node->token == REDIRECT_OUT)
    execute_redir_out(node, ms);
    else if(node->token == HEREDOC_IN)
        execute_heredoc(node,ms, NULL);
    else if(node->token == HEREDOC_OUT)
        execute_heredoc_out(node, ms);
    else if(node->token == WORD)
        execute_word(node, ms);
    else
        exit(1);//error
}
