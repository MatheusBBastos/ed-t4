#include "files.h"

typedef struct files_t {
    FILE *entryFile;
    FILE *outputSVGFile;
    FILE *outputQryFile;
    FILE *queryFile;
    FILE *txtFile;
    FILE *ecFile;
    FILE *pmFile;
    char *outputDir;
    char *svgFileName;
} *FilesPtr;

Files Files_Create() {
    FilesPtr files = malloc(sizeof(struct files_t));
    return files;
}

FILE *Files_GetEntryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->entryFile;
}

FILE *Files_GetOutputSVGFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputSVGFile;
}

FILE *Files_GetOutputQryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputQryFile;
}

FILE *Files_GetQueryFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->queryFile;
}

FILE *Files_GetTxtFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->txtFile;
}

FILE *Files_GetEcFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->ecFile;
}

FILE *Files_GetPmFile(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->pmFile;
}

char *Files_GetOutputDir(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->outputDir;
}

char *Files_GetSvgFileName(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    return files->svgFileName;
}

void Files_SetEntryFile(Files filesVoid, FILE *entryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->entryFile = entryFile;
}

void Files_SetOutputSVGFile(Files filesVoid, FILE *outputSVGFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputSVGFile = outputSVGFile;
}

void Files_SetOutputQryFile(Files filesVoid, FILE *outputQryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputQryFile = outputQryFile;
}

void Files_SetQueryFile(Files filesVoid, FILE *queryFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->queryFile = queryFile;
}

void Files_SetTxtFile(Files filesVoid, FILE *txtFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->txtFile = txtFile;
}

void Files_SetEcFile(Files filesVoid, FILE *ecFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->ecFile = ecFile;
}

void Files_SetPmFile(Files filesVoid, FILE *pmFile) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->pmFile = pmFile;
}

void Files_SetOutputDir(Files filesVoid, char *outputDir) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->outputDir = outputDir;
}

void Files_SetSvgFileName(Files filesVoid, char *svgFileName) {
    FilesPtr files = (FilesPtr) filesVoid;
    files->svgFileName = svgFileName;
}

void Files_Destroy(Files filesVoid) {
    FilesPtr files = (FilesPtr) filesVoid;
    free(files);
}
