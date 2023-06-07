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
#include <libgen.h> /* Required for dirname() / basename() GNU/Linux or MinGW */

#define BUFFER_FILE_COPY (2048*1024)
unsigned char buffer_file_copy[BUFFER_FILE_COPY];

#define PATH_SEPARATOR_LINUX "/"
#define PATH_SEPARATOR_WINDOWS "\\"

/* External static lib from file Find_DLL_Dependencies.c */
extern int Find_DLL_Dependencies(const char* filename, char* dependencies[], size_t num_dependencies);
extern void Free_DLL_Dependencies(char* dependencies[], size_t num_dependencies);

#define APP_NAME "mingw-bundledlls"
#define VERSION "v0.2.4 07/06/2023 B.VERNOUX"

#define BANNER1 APP_NAME " " VERSION "\n"
#define USAGE "usage: " APP_NAME " <exe_file> [--copy] [--verbose]\n"

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

#define MAX_FILE_LENGTH 256
#define MAX_DIR_LENGTH 767
#define MAX_PATH_LENGTH 1024 // MAX_DIR_LENGTH(767) + PATH_SEPARATOR(1)+ MAX_FILE_LENGTH(256)
#define MAX_DEPS 10000
#define MAX_GATHERDEPS 1000

bool verbose = false;
bool copy_files = false;

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
	/* DirectX 3D 11 */
	"d3d11.dll", "dxgi.dll", "dwrite.dll"
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
		/* Detect path separator */
		char* PATH_SEPARATOR;
		if(strstr(path_prefixes[i], PATH_SEPARATOR_WINDOWS) != NULL) {
			PATH_SEPARATOR = PATH_SEPARATOR_WINDOWS;
		} else {
			PATH_SEPARATOR = PATH_SEPARATOR_LINUX;
		}
		snprintf(buffer, MAX_PATH_LENGTH, "%s%s%s", path_prefixes[i], PATH_SEPARATOR, filename);
		FILE* file = fopen(buffer, "rb");
		if(verbose) {
			if(file == NULL) {
				printf("findFullPath: %s (Not Found)\n", buffer);
			}
		}
		if (file) {
			fclose(file);
			return buffer;
		}
	}
	return NULL;
}

int gatherDeps(const char* path, const char* path_prefixes[], int num_prefixes, HashMap* seen, char* deps[], int depth_level) {
	int num_deps = 0;
	char* dependencies[MAX_GATHERDEPS]; // Adjust the array size as per your requirements
	size_t num_dependencies = ARRAY_LENGTH(dependencies);

	int num_dll_deps = Find_DLL_Dependencies(path, dependencies, num_dependencies);
	if(verbose) {
		printf("dep find(%d-%d deps): %s\n",depth_level, num_dll_deps, path);
	}
	if (num_dll_deps < 0) {
		printf("Error Find_DLL_Dependencies() return %d\n", num_dll_deps);
		return 0;
	} else if(num_dll_deps == 0) {
		return 0;
	}

	for (int dll_deps_idx = 0; dll_deps_idx < num_dll_deps; dll_deps_idx++) {
		char* dep = dependencies[dll_deps_idx];
		char ldep[MAX_FILE_LENGTH + 1] = { 0 };
		strncpy(ldep, dep, MAX_FILE_LENGTH);
		for (int i = 0; ldep[i]; i++) {
			ldep[i] = tolower(ldep[i]);
		}

		bool already_seen = get(seen, ldep);
		if (already_seen) {
			if(verbose) {
				printf("dep(%d-%d): %s already seen\n",depth_level, dll_deps_idx, ldep);
			}
			continue;
		} else {
			put(seen, ldep, true);
		}

		bool blacklisted = get(&blacklist, ldep);
		if (blacklisted) {
			if(verbose) {
				printf("dep(%d-%d): %s blacklisted\n",depth_level, dll_deps_idx, ldep);
			}
			continue;
		}

		char buffer[MAX_PATH_LENGTH + 1] = { 0 };
		char* dep_path = findFullPath(dep, path_prefixes, num_prefixes, buffer);
		if (dep_path) {
			if(num_deps < MAX_DEPS)
			{
				if(verbose) {
					printf("dep(%d-%d) found full path: %s\n",depth_level, dll_deps_idx, dep_path);
				}
				deps[num_deps++] = strdup(dep_path);
				depth_level++;
				int subdeps = gatherDeps(dep_path, path_prefixes, num_prefixes, seen, &deps[num_deps], depth_level);
				depth_level--;
				num_deps += subdeps;
			} else {
				printf("Error fatal num_deps(%d-%d) >= MAX_DEPS(%d) (increase MAX_DEPS)\n",depth_level, num_deps, MAX_DEPS);
			}
		} else {
			if(verbose) {
				printf("dep(%d-%d): NOT FOUND in full path: %s\n",depth_level, dll_deps_idx, dep);
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

	size_t bytesRead;
	while ((bytesRead = fread(buffer_file_copy, 1, sizeof(buffer_file_copy), sourceFile)) > 0) {
		if (fwrite(buffer_file_copy, 1, bytesRead, destFile) != bytesRead) {
			printf("Error writing to destination file: %s\n", destination);
			fclose(sourceFile);
			fclose(destFile);
			return;
		}
	}

	fclose(sourceFile);
	fclose(destFile);
}

void copyDeps(const char* dest_dir, char* deps[], int num_deps) {
	char exe_dir[MAX_PATH_LENGTH + 1] = { 0 };
	strncpy(exe_dir, dest_dir, MAX_PATH_LENGTH);

	for (int i = 0; i < num_deps; i++) {
		char* dep = deps[i];
		char target[MAX_PATH_LENGTH + 1] = { 0 };
		/* Detect path separator */
		char* PATH_SEPARATOR;
		if(strstr(exe_dir, PATH_SEPARATOR_WINDOWS) != NULL) {
			PATH_SEPARATOR = PATH_SEPARATOR_WINDOWS;
		} else {
			PATH_SEPARATOR = PATH_SEPARATOR_LINUX;
		}
		snprintf(target, MAX_PATH_LENGTH, "%s%s%s", exe_dir, PATH_SEPARATOR, basename(dep));
		/* Check if target file already exist */
		FILE* targetFile = fopen(target, "rb");
		if (targetFile != NULL) {
			fclose(targetFile);
			printf("Dependency '%s' target directory already exist, skipping copy...\n", target);
		} else {
			if (strcmp(dep, target) == 0) {
				printf("Dependency '%s' is already in the target directory, skipping copy...\n", dep);
			} else {
				printf("Copying '%s' to '%s'\n", dep, target);
				copyFile(dep, target);
			}
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

	char* exe_file = argv[1];
	if (argc >= 3 && strcmp(argv[2], "--verbose") == 0) {
		verbose = true;
	}
	if (argc >= 4 && strcmp(argv[3], "--verbose") == 0) {
		verbose = true;
	}
	if (argc >= 3 && strcmp(argv[2], "--copy") == 0) {
		copy_files = true;
	}
	if (argc >= 4 && strcmp(argv[3], "--copy") == 0) {
		copy_files = true;
	}
	if(verbose == true) {
		printf("verbose\n--copy=%d\nexe_file=\"%s\"\n", copy_files?1:0, exe_file);
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

	if(verbose) {
		printf("path_prefixes(%d):\n", num_prefixes);
		for(int i = 0; i < num_prefixes; i++) {
			printf("\"%s\"\n", path_prefixes[i]);
		}
		printf("\n");
	}

	HashMap seen;
	initHashMap(&seen, MAX_DEPS);
	initHashMap(&blacklist, MAX_DEPS);

	/* Add the input exe/dll file input as seen to avoid to include it in gatherDeps() */
	put(&seen, basename(exe_file), true);

	for (int i = 0; i < ARRAY_LENGTH(blacklist_entries); i++) {
		put(&blacklist, blacklist_entries[i], true);
	}

	char* deps[MAX_DEPS + 1];
	int num_deps = gatherDeps(exe_file, path_prefixes, num_prefixes, &seen, deps, 0);
	if (num_deps > 0) {
		printf("Dependencies(%d):\n", num_deps);
		if (copy_files) { // Copy all dependencies
			char dest_dir[MAX_DIR_LENGTH + 1] = { 0 };
			snprintf(dest_dir, MAX_DIR_LENGTH, "%s", dirname(exe_file));
			copyDeps(dest_dir, deps, num_deps);
		} else { // Display all dependencies
			for (int i = 0; i < num_deps; i++) {
				printf("%s\n", deps[i]);
			}
		}

		freeDeps(deps, num_deps);
	} else {
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

