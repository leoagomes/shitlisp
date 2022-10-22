#include <stdio.h>
#include <stdlib.h>
#include <linenoise.h>
#include <string.h>

#include "sshitlisp.h"

#define PROMPT "shitlisp> "
#define HISTORY_FILE ".shitlisp_history"

int main(int argc, char *argv[]) {
    printf("welcome to shitlisp\n");

    linenoiseSetMultiLine(1);
    linenoiseHistoryLoad(HISTORY_FILE);

    struct state* state = bootstrap_state();

    char* line;
    while ((line = linenoise(PROMPT)) != NULL) {
        if (!strncmp(line, "\\quit", 5) || !strncmp(line, "\\q", 2)) {
            printf("bye\n");
            free(line);
            break;
        }

        if (line[0] != '\0') {
            // TODO: process line
            printf("=> %s\n", line);
            linenoiseHistoryAdd(line);
            linenoiseHistorySave(HISTORY_FILE);
        }
        free(line);
    }

    destroy_state(state);
    return 0;
}
