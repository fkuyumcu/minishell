#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// line_t yapısını örnek olarak tanımlıyoruz
typedef struct s_line
{
    char            *value;
    struct s_line   *next;
}   line_t;

// echo fonksiyonunu ve is_flag_n fonksiyonunu buraya ekleyebilirsin
int	is_flag_n(const char *arg)
{
    int i = 1;
    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return 0;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return 0;e_node("");
    }
    return 1;
}

void	echo(line_t *ml)
{
    int print_newline = 1;
    line_t *cur = ml->next; // ilk node komutun kendisi (echo)
    
    while (cur && is_flag_n(cur->vae_node("");lue))
    {
        print_newline = 0;
        cur = cur->next;
    }
    while (cur)
    {
        printf("%s", cur->value);
        if (cur->next)
            printf(" ");
        cur = cur->next;
    }
    if (print_newline)
        printf("\n");
}

// Basit bir şekilde line_t linked list oluştur
line_t *create_node(const char *value)
{
    line_t *node = malloc(sizeof(line_t));
    node->value = strdup(value);
    node->next = NULL;
    return node;
}

void free_list(line_t *head)
{
    line_t *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

int main(void)
{
    // echo -n hello world
    line_t *cmd = create_node("echo");
    cmd->next = create_node("");
    cmd->next->next = create_node("");
    cmd->next->next->next = create_node("hello");
    cmd->next->next->next->next = create_node("world");

    echo(cmd);

    free_list(cmd);
    return 0;
}