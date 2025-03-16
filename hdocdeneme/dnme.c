#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void handle_heredoc(char *delimiter, char *heredoc_file)
{
    char *line;
    char *all_input = NULL;
    size_t total_length = 0;
    int fd;

    if (!heredoc_file)
        fd = STDOUT_FILENO;
    else
        fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("Error opening heredoc file");
        exit(1);
    }

    while (1)
    {
        line = readline("> ");  // readline ile kullanıcıdan input al (stdin)
        if (line == NULL)
            break;

        // Satır sonundaki newline karakterini kaldır
        line[strcspn(line, "\n")] = '\0';

        // Eğer delimiter (EOF vb.) ile karşılaştıysak çık
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        // Girilen satırı mevcut all_input'a ekle
        size_t line_length = strlen(line);
        all_input = realloc(all_input, total_length + line_length + 2);  // Yeni satırı eklemek için belleği genişlet
        if (all_input == NULL)
        {
            perror("Memory allocation error");
            exit(1);
        }

        // Mevcut inputu birleştir
        strcpy(all_input + total_length, line);
        total_length += line_length;
        all_input[total_length] = '\n';  // Her satırdan sonra newline ekle
        total_length++;

        free(line);  // Satırın belleğini serbest bırak
    }

    // Inputu dosyaya veya stdout'a yaz
    if (all_input)
    {
        write(fd, all_input, total_length);  // Tüm inputu yaz
        free(all_input);  // Belleği serbest bırak
    }

    close(fd);  // Dosyayı kapat
}

int main()
{
    char *heredoc_file = "dnme";  // Here-doc dosyasının adı (NULL olursa, stdout kullanılır)
    char *delimiter = "EOF";  // Here-doc için delimiter belirle

    // Here-document işlemini başlat
    handle_heredoc(delimiter, heredoc_file);

    return 0;
}
