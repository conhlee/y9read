#include "files.h"

#include <stdlib.h>
#include <stdio.h>

const char* FileBasePath = "";

FileHandle FileCreateHandle(const char* path) {
    if (path == NULL)
        panic("FileCreateHandle: path is NULL");

    FileHandle hndl = {0};

    char fpath[512];
    if (path[0] != '/')
        snprintf(fpath, sizeof(fpath), "%s%s", FileBasePath, path);
    else
        snprintf(fpath, sizeof(fpath), "%s", path);

    FILE* fp = fopen(fpath, "rb");
    if (fp == NULL)
        panic("FileCreateHandle: fopen failed (path : %s)", fpath);

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        panic("FileCreateHandle: fseek failed (path : %s)", fpath);
    }

    hndl.size = ftell(fp);
    if (hndl.size == -1L) {
        fclose(fp);
        panic("FileCreateHandle: ftell failed (path : %s)", fpath);
    }

    rewind(fp);

    hndl.data_void = malloc(hndl.size);
    if (hndl.data_void == NULL) {
        fclose(fp);
        panic("FileCreateHandle: malloc failed");
    }

    u64 bytesCopied = fread(hndl.data_void, 1, hndl.size, fp);
    if (bytesCopied < hndl.size && ferror(fp)) {
        fclose(fp);
        free(hndl.data_void);
        panic("FileCreateHandle: fread failed (path : %s)", fpath);
    }

    fclose(fp);
    return hndl;
}
void FileDestroyHandle(FileHandle handle) {
    if (handle.data_void)
        free(handle.data_void);
    handle.data_void = 0;
    handle.size = 0;
}
