# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2
TARGET = offarchwiki
SRC = offarchwiki.c
PREFIX = /usr/local
WIKI_DIR = $(PREFIX)/share/doc/offarchwiki/html

# GitHub repo for Arch Wiki HTML docs
WIKI_REPO = https://github.com/lahwaacz/arch-wiki-docs.git

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: all
	@echo "Installing binary to $(PREFIX)/bin/$(TARGET)..."
	install -Dm755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

	@echo "Cloning or updating wiki HTML files..."
	mkdir -p $(DESTDIR)$(WIKI_DIR)
	rm -rf /tmp/arch-wiki-docs
	git clone --depth=1 $(WIKI_REPO) /tmp/arch-wiki-docs
	cp -r /tmp/arch-wiki-docs/html/en $(DESTDIR)$(WIKI_DIR)
	rm -rf /tmp/arch-wiki-docs

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	rm -rf $(DESTDIR)$(WIKI_DIR)

clean:
	rm -f $(TARGET)

