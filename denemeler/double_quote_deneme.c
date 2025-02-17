void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    (*input)++;  // İlk çift tırnağı atla
    char buffer[1024];  // Geçici bir buffer (maksimum uzunluk ayarla)
    int i = 0;

    while (**input && **input != '"') {
        if (**input == '\\' && (*(*input + 1) == '"' || *(*input + 1) == '\\')) {
            (*input)++;  // Kaçış karakteri varsa onu atla, sonraki karakteri doğrudan al
        }
        buffer[i++] = **input;
        (*input)++;

        if (i >= 1023) { // Buffer overflow kontrolü
            fprintf(stderr, "String too long!\n");
            return;
        }
    }

    if (**input == '"') {
        (*input)++;  // Kapanış çift tırnağını atla
    } else {
        fprintf(stderr, "Syntax Error: Unclosed double quote\n");
        return;
    }

    buffer[i] = '\0';  // String null-terminate et
    tokens[*count].type = TOKEN_STRING;
    tokens[*count].value = strdup(buffer); // Hafıza tahsisi yap
    (*count)++;
}
