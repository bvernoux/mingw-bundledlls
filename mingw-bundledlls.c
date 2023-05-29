/* gcc -Wall -O3 -o mingw-bundledlls mingw-bundledlls.c Find_DLL_Dependencies.c see also Makefile
 * Python3 version of mingw-bundledlls https://github.com/mpreisler/mingw-bundledlls
 *
 * Copyright (c) 2015 Martin Preisler (Python2 / Python3 version)
 * Copyright (C) 2023 Benjamin VERNOUX (C version)
 * MIT License (MIT)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <libgen.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/utime.h>
#define PATH_SEPARATOR "\\"
#else
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#define PATH_SEPARATOR "/"
#endif

/* External static lib from file Find_DLL_Dependencies.c */
extern int Find_DLL_Dependencies(const char* filename, char* dependencies[], size_t num_dependencies);
extern void Free_DLL_Dependencies(char* dependencies[], size_t num_dependencies);

#define APP_NAME "mingw-bundledlls"
#define VERSION "v0.2.0 29/05/2023 B.VERNOUX"

#define BANNER1 APP_NAME " " VERSION "\n"
#define USAGE "usage: " APP_NAME " <exe_file> [--copy]\n"

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

#define MAX_PATH_LENGTH 1024
#define MAX_DEPS 10000

const char* DEFAULT_PATH_PREFIXES[] = {
	"", "/usr/bin", "/usr/i686-w64-mingw32/sys-root/mingw/bin", "/mingw64/bin",
	"/usr/i686-w64-mingw32/sys-root/mingw/lib",
	"C:\\msys64\\mingw64\\bin"
};

// blacklist Windows system DLL (lower case)
const char* blacklist_entries[] = {
	"advapi32.dll", "kernel32.dll", "msvcrt.dll", "ole32.dll", "user32.dll",
	"ws2_32.dll", "comdlg32.dll", "gdi32.dll", "imm32.dll", "oleaut32.dll",
	"shell32.dll", "winmm.dll", "winspool.drv", "wldap32.dll",
	"ntdll.dll", "d3d9.dll", "mpr.dll", "crypt32.dll", "dnsapi.dll",
	"shlwapi.dll", "version.dll", "iphlpapi.dll", "msimg32.dll", "setupapi.dll",
	"opengl32.dll", "dwmapi.dll", "uxtheme.dll", "secur32.dll", "gdiplus.dll",
	"usp10.dll", "comctl32.dll", "wsock32.dll", "netapi32.dll", "userenv.dll",
	"avicap32.dll", "avrt.dll", "psapi.dll", "mswsock.dll", "glu32.dll",
	"bcrypt.dll", "rpcrt4.dll", "hid.dll",
	"d3d11.dll", "dxgi.dll" // DirectX 3D 11
};

typedef struct {
	char* key;
	bool value;
} Entry;

typedef struct {
	Entry* entries;
	int size;
} HashMap;

HashMap blacklist;

void initHashMap(HashMap* hashMap, int size) {
	hashMap->entries = malloc(size * sizeof(Entry));
	hashMap->size = size;
	for (int i = 0; i < size; i++) {
		hashMap->entries[i].key = NULL;
		hashMap->entries[i].value = false;
	}
}

void freeHashMap(HashMap* hashMap) {
	for (int i = 0; i < hashMap->size; i++) {
		free(hashMap->entries[i].key);
	}
	free(hashMap->entries);
}

size_t hash(const char* str) {
	/*
	 * Recommended hash function for string keys.
	 *
	 * This is an implementation of the well-documented Jenkins one-at-a-time
	 * hash function. See https://en.wikipedia.org/wiki/Jenkins_hash_function
	 */
	size_t hash = 0;
	for (; *str; ++str) {
		hash += *str;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

bool put(HashMap* hashMap, const char* key, bool value) {
	size_t index = hash(key) % hashMap->size;
	Entry* entry = &hashMap->entries[index];

	if (entry->key == NULL) {
		entry->key = strdup(key);
		entry->value = value;
		return true;
	}

	return false;
}

bool get(HashMap* hashMap, const char* key) {
	size_t index = hash(key) % hashMap->size;
	Entry* entry = &hashMap->entries[index];

	if (entry->key != NULL && strcmp(entry->key, key) == 0) {
		return entry->value;
	}

	return false;
}

char* findFullPath(const char* filename, const char* path_prefixes[], int num_prefixes, char* buffer) {
	for (int i = 0; i < num_prefixes; i++) {
		snprintf(buffer, MAX_PATH_LENGTH, "%s%s%s", path_prefixes[i], PATH_SEPARATOR, filename);
		FILE* file = fopen(buffer, "r");
		if (file) {
			fclose(file);
			return buffer;
		}
	}
	return NULL;
}

int gatherDeps(const char* path, const char* path_prefixes[], int num_prefixes, HashMap* seen, char* deps[]) {
	int num_deps = 0;

	char* dependencies[MAX_DEPS]; // Adjust the array size as per your requirements
	size_t num_dependencies = ARRAY_LENGTH(dependencies);
	int num_dll_deps = Find_DLL_Dependencies(path, dependencies, num_dependencies);
	if (num_dll_deps < 1) {
		printf("Error Find_DLL_Dependencies() return %d\n", num_dll_deps);
		return 0;
	}

	for (size_t dll_deps_idx = 0; dll_deps_idx < num_dll_deps; dll_deps_idx++) {
		char* dep = dependencies[dll_deps_idx];
		char ldep[MAX_PATH_LENGTH + 1];
		strncpy(ldep, dep, MAX_PATH_LENGTH);
		for (int i = 0; ldep[i]; i++) {
			ldep[i] = tolower(ldep[i]);
		}

		bool blacklisted = get(&blacklist, ldep);
		if (blacklisted) {
			continue;
		}

		bool already_seen = get(seen, ldep);
		if (already_seen) {
			continue;
		}

		char buffer[MAX_PATH_LENGTH + 1];
		char* dep_path = findFullPath(dep, path_prefixes, num_prefixes, buffer);
		if (dep_path) {
			if(num_deps < MAX_DEPS)
			{
				put(seen, ldep, true);
				deps[num_deps++] = strdup(dep_path);
				int subdeps = gatherDeps(dep_path, path_prefixes, num_prefixes, seen, &deps[num_deps]);
				num_deps += subdeps;
			} else {
				printf("Error fatal num_deps(%d) >= MAX_DEPS(%d) (increase MAX_DEPS)\n", num_deps, MAX_DEPS);
			}
		}
	}
	Free_DLL_Dependencies(dependencies, num_dll_deps);
	return num_deps;
}

/* Free all memory allocated in gatherDeps() char* deps[], int num_deps */
void freeDeps(char* deps[], int num_deps) {
	for (int i = 0; i < num_deps; i++) {
		free(deps[i]);
	}
}

void copyFile(const char* source, const char* destination) {
	FILE* sourceFile = fopen(source, "rb");
	if (sourceFile == NULL) {
		printf("Error opening source file: %s\n", source);
		return;
	}

	FILE* destFile = fopen(destination, "wb");
	if (destFile == NULL) {
		printf("Error creating destination file: %s\n", destination);
		fclose(sourceFile);
		return;
	}

	char buffer[8192];
	size_t bytesRead;

	while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
		if (fwrite(buffer, 1, bytesRead, destFile) != bytesRead) {
			printf("Error writing to destination file: %s\n", destination);
			fclose(sourceFile);
			fclose(destFile);
			return;
		}
	}

	fclose(sourceFile);
	fclose(destFile);

#ifdef _WIN32
	// Set the same access and modification time for the destination file as the source file
	struct _stat64 sourceStat;
	if (_stat64(source, &sourceStat) == 0) {
		_utime64(destination, (struct __utimbuf64*)&sourceStat.st_atime);
	}
#else
	// Set the same access and modification time for the destination file as the source file
	struct stat sourceStat;
	if (stat(source, &sourceStat) == 0) {
		struct utimbuf times;
		times.actime = sourceStat.st_atime;
		times.modtime = sourceStat.st_mtime;
		utime(destination, &times);
	}
#endif
}

void copyDeps(const char* exe_file, char* deps[], int num_deps) {
	char exe_dir[MAX_PATH_LENGTH + 1];
	strncpy(exe_dir, exe_file, MAX_PATH_LENGTH);

	for (int i = 0; i < num_deps; i++) {
		char* dep = deps[i];
		char target[MAX_PATH_LENGTH + 1];
		snprintf(target, MAX_PATH_LENGTH, "%s%s%s", exe_dir, PATH_SEPARATOR, basename(dep));

		if (strcmp(dep, target) == 0) {
			printf("Dependency '%s' is already in the target directory, skipping...\n", dep);
		}
		else {
			printf("Copying '%s' to '%s'\n", dep, target);
			copyFile(dep, target);
		}
	}
}

int main(int argc, char* argv[]) {

	const char** path_prefixes;
	int num_prefixes;

	printf(BANNER1);

	if (argc < 2) {
		printf(USAGE);
		return 1;
	}

	const char* env_path_prefixes = getenv("MINGW_BUNDLEDLLS_SEARCH_PATH");
	if (env_path_prefixes != NULL) {
		int count = 0;
		char* env_copy = strdup(env_path_prefixes);
		char* path = strtok(env_copy, ";");
		while (path != NULL) {
			count++;
			path = strtok(NULL, ";");
		}
		free(env_copy);

		num_prefixes = count;
		path_prefixes = (const char**)malloc(num_prefixes * sizeof(const char*));
		env_copy = strdup(env_path_prefixes);
		path = strtok(env_copy, ";");
		int i = 0;
		while (path != NULL) {
			path_prefixes[i++] = strdup(path);
			path = strtok(NULL, ";");
		}
		free(env_copy);
	}
	else {
		num_prefixes = ARRAY_LENGTH(DEFAULT_PATH_PREFIXES);
		path_prefixes = DEFAULT_PATH_PREFIXES;
	}

	char* exe_file = argv[1];

	HashMap seen;
	initHashMap(&seen, MAX_DEPS);
	initHashMap(&blacklist, MAX_DEPS);

	for (int i = 0; i < ARRAY_LENGTH(blacklist_entries); i++) {
		put(&blacklist, blacklist_entries[i], true);
	}

	char* deps[MAX_DEPS + 1];
	int num_deps = gatherDeps(exe_file, path_prefixes, num_prefixes, &seen, deps);
	if (num_deps > 0) {
		printf("Dependencies(%d):\n", num_deps);
		for (int i = 0; i < num_deps; i++) {
			printf("%s\n", deps[i]);
		}
		if (argc >= 3 && strcmp(argv[2], "--copy") == 0) {
			char destination[MAX_PATH_LENGTH + 1];
			snprintf(destination, MAX_PATH_LENGTH, "%s", dirname(exe_file));
			copyDeps(destination, deps, num_deps);
		}
		freeDeps(deps, num_deps);
	}
	else {
		printf("No dependencies found.\n");
	}

	freeHashMap(&seen);
	freeHashMap(&blacklist);

	if (env_path_prefixes != NULL) {
		for (int i = 0; i < num_prefixes; i++) {
			free((char*)path_prefixes[i]);
		}
		free((char**)path_prefixes);
	}

	return 0;
}
