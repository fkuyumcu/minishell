void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    (*input)++;  // Açılış çift tırnağını atla
    char buffer[1024];  // Geçici buffer (maksimum uzunluk ayarla)
    int i = 0;

    while (**input && **input != '"') {
        if (**input == '\\' && (*(*input + 1) == '"' || *(*input + 1) == '\\')) {
            (*input)++;  // Kaçış karakteri varsa onu atla
        }
        else if (**input == '$') {  // Environment variable varsa çözümle
            (*input)++;
            char var_name[256] = {0};  
            int j = 0;

            while (**input && (isalnum(**input) || **input == '_')) {  
                var_name[j++] = **input;
                (*input)++;
            }
            var_name[j] = '\0';

            char *env_value = getenv(var_name);  // Environment değişkenini oku
            if (env_value) {
                strncpy(&buffer[i], env_value, 1024 - i - 1);
                i += strlen(env_value);
            }
        }
        else {
            buffer[i++] = **input;
            (*input)++;
        }

        if (i >= 1023) {  // Buffer overflow kontrolü
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
    tokens[*count].value = strdup(buffer);  // Hafıza tahsisi yap
    (*count)++;
}
