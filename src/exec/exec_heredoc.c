/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:57:33 by yalp              #+#    #+#             */
/*   Updated: 2025/03/17 14:43:13 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void handle_heredoc(char *delimiter, char **command)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe error");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    if (pid == 0)
    {
        close(pipefd[1]); // Yazma ucunu kapat (sadece okuma için)
        dup2(pipefd[0], STDIN_FILENO); // stdin'i pipe'dan oku
        close(pipefd[0]); // stdin yönlendirildiği için artık kapatabiliriz

        execvp(command[0], command); // Komutu çalıştır
        perror("execvp error"); // execvp başarısız olursa hata ver
        exit(1);
    }
    else
    {
        close(pipefd[0]); // Okuma ucunu kapat (sadece yazma için)

        char *line;
        char *all_input = NULL;
        size_t total_length = 0;

        while (1)
        {
            line = readline("> ");
            if (!line)
                break;

            line[strcspn(line, "\n")] = '\0'; // '\n' karakterini kaldır

            if (strcmp(line, delimiter) == 0)
            {
                free(line);
                break; // Delimiter geldi, çık
            }

            size_t line_length = strlen(line);
            all_input = realloc(all_input, total_length + line_length + 2); // Belleği genişlet
            if (!all_input)
            {
                perror("Memory allocation error");
                exit(1);
            }

            strcpy(all_input + total_length, line);
            total_length += line_length;
            all_input[total_length] = '\n'; // Satır sonuna newline ekle
            total_length++;

            free(line);
        }

        // Tüm heredoc girdisini tek seferde pipe’a yaz
        if (all_input)
        {
            write(pipefd[1], all_input, total_length);
            free(all_input);
        }

        close(pipefd[1]); // Pipe’ı kapat (child process'in okuması için)
        wait(NULL); // Çocuk sürecin bitmesini bekle
    }
}
