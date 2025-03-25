/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:57:33 by yalp              #+#    #+#             */
/*   Updated: 2025/03/25 16:35:20 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void handle_heredoc(char *delimiter, ast_node_t *node, minishell_t *minishell, char *out)
{
    int pipefd[2];
    pid_t pid;
    char *line = NULL;
    size_t len = 0;
    char *cmd_path;
    char **command;
    int fdd;

    // Komutu belirle: node->left veya node->right'ı kontrol et
    if (node->left && node->left->args)
        command = node->left->args; // cat << eof durumu
    else if (node->right && node->right->args)
        command = node->right->args + 1; // << eof cat durumu (ilk argümanı atla)
    else
    {
        perror("Invalid heredoc syntax");
        return;
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe error");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    if (pid == 0) // Çocuk işlem (execve çağrılacak)
    {
        close(pipefd[1]); // Yazma ucunu kapat
        dup2(pipefd[0], STDIN_FILENO); // Pipe'i giriş olarak kullan
        close(pipefd[0]);

        // Komut bulunamazsa hata mesajı ver
        cmd_path = find(command[0], minishell->envp, minishell);
        if (!cmd_path)
        {
            perror("command not found");
            exit(127);
        }

        // Komutu çalıştır
        execve(cmd_path, command, minishell->envp);
        free(cmd_path);
        perror("execve error");
        exit(1);
    }

    // Ebeveyn işlem (kullanıcıdan giriş alacak)
    close(pipefd[0]); // Okuma ucunu kapat
    line = NULL;
    len = 0;
    char *all_input = NULL;
    size_t total_length = 0;

    while (1)
    {
        ft_putstr_fd("> ", 0);
        if (getline(&line, &len, stdin) == -1)
            break;
        line[strcspn(line, "\n")] = '\0'; // Satır sonundaki newline kaldırıldı

        if (strcmp(line, delimiter) == 0)
            break; // EOF ile çıkış

        size_t line_length = strlen(line);
        all_input = realloc(all_input, total_length + line_length + 2); // +2: newline + null karakter
        if (!all_input)
        {
            perror("Memory allocation error");
            exit(1);
        }
        strcpy(all_input + total_length, line);
        total_length += line_length;
        all_input[total_length] = '\n';
        total_length++;
    }

    // Kullanıcıdan alınan heredoc girişini dosyaya veya pipe'a yaz
    if (all_input)
    {
        if (!out)
            write(pipefd[1], all_input, total_length);
        else
        {
            fdd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fdd == -1)
            {
                perror("file open error");
                exit(1);
            }
            write(fdd, all_input, total_length);
            close(fdd);
        }
        free(all_input);
    }
    free(line);
    close(pipefd[1]); // Yazma ucunu kapat
    wait(NULL);
}
