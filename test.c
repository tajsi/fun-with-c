#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 256
#define IS_WHITE_SPACE(c) (c == 0x20 || c == '\t' || c == '\r' || c == '\n')

char * extract_current_token(
    char **str,
    char delim,
    int trim_trailing_ws,
    int accept_quotes
    )
{
    char *start, *s = *str, *p;
    char quote_char = 0;
    int is_opened_quote = 0, cnt = 0;

    if (trim_trailing_ws) {
        while (IS_WHITE_SPACE(*s))
            s++;
    }

    start = s;

    while (*s) {
        if (accept_quotes) {
            if (!is_opened_quote && ((*s == '\'') || (*s == '\"'))) {
                is_opened_quote = 1;
                quote_char = *s;
            }
            else if (is_opened_quote && (*s == quote_char)) {
                is_opened_quote = 0;
            }
        }
        if (!is_opened_quote && (*s == delim))
            break;
        s++;
        cnt++;
    }

    if (trim_trailing_ws && cnt) {
        p = s;
        p--;
        while (cnt && IS_WHITE_SPACE(*p)) {
            cnt--;
            p--;
        }
    }

    if (*s == delim) {
        *s = '\0';
        s++;
    }

    *str = s;

    return start;
}

int tokenize(
    char **buf,
    char *str,
    char delim,
    unsigned int max_tokens,
    int trim_trailing_ws,
    int accept_quotes
    )
{
    int count;
    char *pos;

    count = 0;
    pos = str;

    while (*pos && count < max_tokens) {
        *(buf + count) = extract_current_token(&pos, delim, trim_trailing_ws, accept_quotes);
        printf("DEBUG: %p - %s\n", buf + count, *(buf + count));
        count++;
    }

    if (*pos && count >= max_tokens) {
        fprintf(stderr, "Max amount of tokens (%i) exceeded\n", max_tokens);
        return -1;
    }

    return count;
}

int main() {

    FILE *fd;
    char buf[BUFFER_SIZE], buf_copy[BUFFER_SIZE], *token_buf, **token_list;
    char *text;
    int num_tokens, token_idx;

    fd = fopen("plik.txt", "r");
    if (fd == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return 1;
    }

    token_buf = (char *) malloc(4096);
    if (token_buf == NULL) {
        fprintf(stderr, "Error allocating buffer: %s\n", strerror(errno));
        return 1;
    }

    token_list = &token_buf;

    while ((text = fgets((char *) buf, BUFFER_SIZE, fd)) != NULL) {
        printf("[%i] - %s", (int) strlen(text), text);
        strcpy(buf_copy, text);
        text = buf_copy;
        num_tokens = tokenize(token_list, text, 0x20, 64, 1, 1);
        if (num_tokens < 0)
            break;
        token_list = &token_buf;
        printf("Found %i tokens\n", num_tokens);
        for (token_idx = 0; token_idx < num_tokens; token_idx++) {
            printf("DEBUG: %p\n", token_list + token_idx);
            printf("[%02i] - %s\n", token_idx, *(token_list + token_idx));
        }
        puts("Finished");
    }
    if (errno != 0) {
        fprintf(stderr, "Error while reading lines: %s\n", strerror(errno));
        return 1;
    }

    fclose(fd);
    free(token_buf);

    return 0;
}
