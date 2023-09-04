#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 100
#define MAX_FILE_SIZE 1024

// Structure to represent a file version
typedef struct {
    char data[MAX_FILE_SIZE];
    int version;
} FileVersion;

// Structure to represent a managed file
typedef struct {
    char name[MAX_FILENAME_LENGTH];
    FileVersion* versions;
    int numVersions;
} ManagedFile;

// Function to create a new managed file
ManagedFile* createManagedFile(const char* name) {
    ManagedFile* file = (ManagedFile*)malloc(sizeof(ManagedFile));
    if (file) {
        strcpy(file->name, name);
        file->versions = NULL;
        file->numVersions = 0;
    }
    return file;
}

// Function to add a new version to a managed file
void addFileVersion(ManagedFile* file, const char* data) {
    if (file) {
        FileVersion newVersion;
        strcpy(newVersion.data, data);
        newVersion.version = file->numVersions + 1;

        file->versions = (FileVersion*)realloc(file->versions, (file->numVersions + 1) * sizeof(FileVersion));
        if (file->versions) {
            file->versions[file->numVersions] = newVersion;
            file->numVersions++;
        }
    }
}

// Function to display all versions of a managed file
void displayFileVersions(const ManagedFile* file) {
    if (file) {
        printf("File: %s\n", file->name);
        for (int i = 0; i < file->numVersions; i++) {
            printf("Version %d:\n%s\n", file->versions[i].version, file->versions[i].data);
        }
    }
}

// Function to compare two versions of a managed file
int compareFileVersions(const ManagedFile* file, int version1, int version2) {
    if (file && version1 >= 1 && version2 >= 1 && version1 <= file->numVersions && version2 <= file->numVersions) {
        if (version1 == version2) {
            return 0; // Versions are identical
        } else if (version1 < version2) {
            return -1; // Version 1 is older
        } else {
            return 1; // Version 2 is older
        }
    }
    return -2; // Invalid input
}

// Function to merge two versions of a managed file
void mergeFileVersions(ManagedFile* file, int version1, int version2) {
    if (file && version1 >= 1 && version2 >= 1 && version1 <= file->numVersions && version2 <= file->numVersions) {
        int mergedVersion = file->numVersions + 1;
        FileVersion newVersion;

        // Merge data from version1 and version2 into newVersion
        snprintf(newVersion.data, MAX_FILE_SIZE, "Merged from Version %d and Version %d", version1, version2);
        newVersion.version = mergedVersion;

        // Add the merged version to the file
        file->versions = (FileVersion*)realloc(file->versions, (file->numVersions + 1) * sizeof(FileVersion));
        if (file->versions) {
            file->versions[file->numVersions] = newVersion;
            file->numVersions++;
        }
    }
}

int main() {
    ManagedFile* exampleFile = createManagedFile("example.txt");

    addFileVersion(exampleFile, "This is version 1 of the file.");
    addFileVersion(exampleFile, "This is version 2 of the file, with some changes.");
    addFileVersion(exampleFile, "Version 3 includes more changes.");

    displayFileVersions(exampleFile);

    int result = compareFileVersions(exampleFile, 1, 2);
    if (result == 0) {
        printf("Version 1 and Version 2 are identical.\n");
    } else if (result < 0) {
        printf("Version 1 is older than Version 2.\n");
    } else {
        printf("Version 2 is older than Version 1.\n");
    }

    mergeFileVersions(exampleFile, 1, 2);

    displayFileVersions(exampleFile);

    // Clean up memory
    free(exampleFile->versions);
    free(exampleFile);

    return 0;
}
