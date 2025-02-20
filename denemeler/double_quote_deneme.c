#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *clean_quotes(const char *input) {
    char buffer[1024];
    int buf_index = 0;
    int i = 0;
    int len = strlen(input);
    char last_quote = '\0'; // Önceki quote türünü saklar

    while (i < len) {
        if (input[i] == '\'' || input[i] == '\"') {
            char quote = input[i];
            i++; // Açılış tırnağını atla
            int start = i;

            // Kapanış tırnağını bulana kadar ilerle
            while (i < len && input[i] != quote) {
                i++;
            }
            if (i >= len) {
                fprintf(stderr, "Error: Unclosed quote\n");
                return NULL;
            }

            // İçeriği kopyala
            for (int j = start; j < i; j++) {
                buffer[buf_index++] = input[j];
            }
            i++; // Kapanış tırnağını atla

            // Eğer aynı tipten bitişik tırnak varsa devam et
            while (i < len && (input[i] == '\'' || input[i] == '\"') && input[i] == quote) {
                i++; // Açılış tırnağını atla
                start = i;
                while (i < len && input[i] != quote) {
                    i++;
                }
                if (i >= len) {
                    fprintf(stderr, "Error: Unclosed quote\n");
                    return NULL;
                }
                for (int j = start; j < i; j++) {
                    buffer[buf_index++] = input[j];
                }
                i++; // Kapanış tırnağını atla
            }

        } else {
            // Normal karakterse direkt kopyala
            buffer[buf_index++] = input[i++];
        }
    }

    buffer[buf_index] = '\0';
    return strdup(buffer);
}

//#ifdef TEST_CLEAN_QUOTES
int main() {
    const char *test1 = "\"ls\"\"-l\"";         // Beklenen: ls-l
    const char *test2 = "\"file\"'.txt'";       // Beklenen: file.txt
    const char *test3 = "\"echo\" 'abc'";       // Beklenen: echo ve abc (ayrı token olmalı)
    const char *test4 = "\"hello";              // Hata: kapanmayan tırnak
    const char *test5 = "\"this\"\"is\"\"fine\""; // Beklenen: thisisfine

    char *res;

    res = clean_quotes(test1);
    if (res) {
        printf("Test1: %s\n", res);
        free(res);
    }

    res = clean_quotes(test2);
    if (res) {
        printf("Test2: %s\n", res);
        free(res);
    }

    res = clean_quotes(test3);
    if (res) {
        printf("Test3: %s\n", res);
        free(res);
    }

    res = clean_quotes(test4);
    if (res) {
        printf("Test4: %s\n", res);
        free(res);
    }

    res = clean_quotes(test5);
    if (res) {
        printf("Test5: %s\n", res);
        free(res);
    }

    return 0;
}
