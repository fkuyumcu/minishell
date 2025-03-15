/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:18:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/10 16:45:31 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

void execute_pipe(ast_node_t *node, minishell_t *ms)
{
    size_t pid;
    int fd[2];
    pipe(fd);
    pid = fork();
    if (pid == 0)
    { 
        close(fd[0]); 
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execute_ast(node->left, ms);
        exit(0);
    } 
    else if(pid > 0)
    { 
        close(fd[1]); 
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execute_ast(node->right, ms);
        waitpid(pid, NULL, 0);
    }
    else 
        exit(1);//error
}

void execute_redir(ast_node_t *node, minishell_t *ms)
{
    int fd;
    if ( node->token == REDIRECT_IN || node->token == REDIRECT_OUT)
        fd = open(node->left->args[0], O_CREAT | O_RDWR | O_TRUNC, 0777);
    else
        fd = open(node->left->args[0], O_CREAT | O_RDWR | O_APPEND, 0777);
    if (fd == -1)
    {
        perror("Error opening input file");//error
        return;//error
    }
    if(node->token == REDIRECT_OUT)
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execute_ast(node->right, ms);
    }
    else
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
        execute_ast(node->left, ms);
    }
}
void execute_heredoc(ast_node_t *node)
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
    else if(node->token == REDIRECT_IN || node->token == REDIRECT_OUT
    || node->token == HEREDOC_OUT)
        execute_redir(node, ms);
    else if(node->token == HEREDOC_IN)
        execute_heredoc(node);
    else if(node->token == WORD)
        execute_word(node, ms);
    else
        exit(1);//error
}

