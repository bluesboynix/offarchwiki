// offarchwiki - Arch Wiki offline CLI tool
// Author: bluesboynix
// Repo: https://github.com/bluesboynix/offarchwiki.git
// License: N/A

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#define VERSION "0.1"
#define WIKI_PATH "/usr/local/share/doc/offarchwiki/html/en"

void show_help(const char *progname) {
    printf("Usage: %s [options] [keyword]\n", progname);
    printf("Options:\n");
    printf("  -b <browser>   Use a specific browser (e.g., firefox, w3m)\n");
    printf("  -c             Use first available terminal browser (w3m, lynx)\n");
    printf("  -l <pattern>   List wiki pages matching the pattern\n");
    printf("  -h             Show this help message\n");
    printf("  -v             Show version information\n");
}

void show_version() {
    printf("offarchwiki version %s\n", VERSION);
}

char* to_lower(char *str) {
    for (char *p = str; *p; ++p)
        *p = tolower((unsigned char)*p);
    return str;
}

void list_matches(const char *pattern) {
    DIR *dir = opendir(WIKI_PATH);
    if (!dir) {
        perror("Failed to open wiki directory");
        return;
    }

    struct dirent *entry;
    char lower_pattern[256];
    strncpy(lower_pattern, pattern, sizeof(lower_pattern));
    to_lower(lower_pattern);

    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;
        char lower_name[256];
        strncpy(lower_name, name, sizeof(lower_name));
        to_lower(lower_name);

        if (strstr(lower_name, lower_pattern)) {
            printf("%s\n", name);
        }
    }
    closedir(dir);
}

char* find_browser() {
    char *browsers[] = { "w3m", "lynx", NULL };
    for (int i = 0; browsers[i]; i++) {
        if (system((char[]){ "which ", browsers[i], " > /dev/null 2>&1", 0 }) == 0) {
            return browsers[i];
        }
    }
    return NULL;
}

void open_page(const char *browser, const char *keyword) {
    char file_path[512];
    if (keyword == NULL) {
        snprintf(file_path, sizeof(file_path), "%s/Main_page.html", WIKI_PATH);
    } else {
        snprintf(file_path, sizeof(file_path), "%s/%s.html", WIKI_PATH, keyword);
    }

    if (access(file_path, F_OK) != 0) {
        fprintf(stderr, "Page not found: %s\n", file_path);
        return;
    }

    char command[1024];
    if (strcmp(browser, "w3m") == 0 || strcmp(browser, "lynx") == 0) {
        snprintf(command, sizeof(command), "%s %s", browser, file_path);
        system(command);
    } else {
        snprintf(command, sizeof(command), "%s %s >/dev/null 2>&1 &", browser, file_path);
        system(command);
    }
}

int main(int argc, char *argv[]) {
    char *browser = NULL;
    char *pattern = NULL;
    int use_terminal_browser = 0;
    int opt;

    while ((opt = getopt(argc, argv, "b:l:chv")) != -1) {
        switch (opt) {
            case 'b':
                browser = optarg;
                break;
            case 'c':
                use_terminal_browser = 1;
                break;
            case 'l':
                list_matches(optarg);
                return 0;
            case 'h':
                show_help(argv[0]);
                return 0;
            case 'v':
                show_version();
                return 0;
            default:
                show_help(argv[0]);
                return 1;
        }
    }

    if (!browser && use_terminal_browser) {
        browser = find_browser();
        if (!browser) {
            fprintf(stderr, "No terminal browser found (w3m, lynx).\n");
            return 1;
        }
    }

    if (!browser) {
        browser = "xdg-open";
    }

    const char *keyword = argv[optind];  // Remaining argument
    open_page(browser, keyword);

    return 0;
}
