#include "interaction.h"

void startInteraction(Files *files, char *baseDir, char *entryFileName) {
    
    printf("-- MODO INTERAÇÃO --\n");
    char buffer[128];
    char command[16];
 
    do {
        fgets(buffer, 100, stdin);
        sscanf(buffer, "%15s", command);

        if (strcmp(command, "q") == 0) {
            char qryFileName[64];
            sscanf(buffer + 2, "%63[^\n]", qryFileName);

            if (!Files_OpenQueryFiles(files, baseDir, entryFileName, qryFileName))
                continue;
            
            processAndGenerateQuery(files);
        } else if (strcmp(command, "dmprbt") == 0) {
            char t = '\0', arq[64] = "";
            sscanf(buffer + 7, "%c %63[^\n]", &t, arq);

            if (t == '\0') {
                printf("Forneça um tipo!\n");
                continue;
            }

            if (arq[0] == '\0') {
                printf("Forneça um arquivo!\n");
                continue;
            }

            Query_Dmprbt(Files_GetOutputDir(files), t, arq);
            printf("Árvore escrita no arquivo %s!\n", arq);

        } else if (strcmp(command, "nav") == 0) {
            char t;
            sscanf(buffer + 4, "%c", &t);

            // TODO: nav
        }
    } while (strcmp(command, "sai") != 0);
}