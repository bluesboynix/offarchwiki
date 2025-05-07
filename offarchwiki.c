#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.5"
#define AUTHOR "bluesboynix"
#define GITHUB_REPO "https://github.com/bluesboynix/offarchwiki.git"
#define WIKI_PATH "/usr/share/doc/arch-wiki/html/en"
#define MAIN_PAGE WIKI_PATH "/Main_page.html"
#define TERMINAL_BROWSER "w3m"

// Function to show help message
void show_help() {
    printf("Usage: archwiki [options] [search_term]\n\n");
    printf("Options:\n");
    printf("  -v          Show version\n");
    printf("  -h          Show this help message\n");
    printf("  -b <browser>  Specify a browser (e.g., firefox, chrome, etc.)\n");
    printf("  -c          Use the default terminal browser (e.g., w3m, lynx)\n");
    printf("  -l          List possible matches for search term\n\n");
    printf("Author: %s\n", AUTHOR);
    printf("GitHub Repo: %s\n", GITHUB_REPO);
}

// Function to check if the given command exists
int command_exists(const char *command) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "command -v %s > /dev/null 2>&1", command);
    return system(buffer) == 0;
}

// Function to handle the version option
void show_version() {
    printf("Version: %s\n", VERSION);
    printf("Author: %s\n", AUTHOR);
    printf("GitHub Repo: %s\n", GITHUB_REPO);
}

// Function to find wiki files based on search term
void find_wiki_files(const char *search_term) {
    char command[1024];
    snprintf(command, sizeof(command), "find %s -type f -iname '*%s*.html'", WIKI_PATH, search_term);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error: Could not search for files.\n");
        return;
    }

    char file_path[1024];
    int found = 0;
    while (fgets(file_path, sizeof(file_path), fp) != NULL) {
        printf("%s", file_path);  // Print each found file
        found = 1;
    }

    fclose(fp);
    if (!found) {
        printf("No matching pages found for '%s'.\n", search_term);
    }
}

// Function to search for the first matching wiki file
char* find_first_match(const char *search_term) {
    static char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/*%s*.html", WIKI_PATH, search_term);
    FILE *fp = popen(file_path, "r");
    if (fp == NULL) {
        return NULL;
    }
    
    if (fgets(file_path, sizeof(file_path), fp)) {
        fclose(fp);
        return file_path;
    }

    fclose(fp);
    return NULL;
}

int main(int argc, char *argv[]) {
    char *browser = "xdg-open";  // Default browser
    char *search_term = NULL;
    int list_matches = 0;
    int i;

    // Parse command line arguments
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            show_version();
            return 0;
        } else if (strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        } else if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 < argc) {
                if (command_exists(argv[i + 1])) {
                    browser = argv[i + 1];
                    i++;
                } else {
                    printf("Warning: Browser '%s' not found. Falling back to default.\n", argv[i + 1]);
                }
            } else {
                printf("Error: '-b' requires a browser name.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-c") == 0) {
            browser = TERMINAL_BROWSER;
        } else if (strcmp(argv[i], "-l") == 0) {
            list_matches = 1;
        } else {
            if (search_term == NULL) {
                search_term = argv[i];
            } else {
                printf("Error: unexpected extra argument '%s'\n", argv[i]);
                return 1;
            }
        }
    }

    // If -l is specified, list possible matches
    if (list_matches) {
        if (search_term == NULL) {
            printf("Error: '-l' option requires a search term.\n");
            return 1;
        }
        find_wiki_files(search_term);
        return 0;
    }

    // Determine the file to open
    char *file_to_open = (search_term == NULL) ? MAIN_PAGE : find_first_match(search_term);
    
    if (file_to_open == NULL) {
        printf("No matching page found for '%s'.\n", search_term);
        return 1;
    }

    // Construct the command to open the browser
    char command[1024];
    if (strcmp(browser, "w3m") == 0 || strcmp(browser, "lynx") == 0) {
        snprintf(command, sizeof(command), "%s %s", browser, file_to_open);
    } else {
        snprintf(command, sizeof(command), "%s %s > /dev/null 2>&1 &", browser, file_to_open);
    }

    // Execute the command
    system(command);

    return 0;
}


