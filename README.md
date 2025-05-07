# offarchwiki

**offarchwiki** is a lightweight command-line tool for accessing the Arch Linux Wiki offline via local HTML files.

## 📦 Features

- Offline access to the full Arch Wiki
- Opens pages in GUI or terminal browsers
- Supports keyword search and page listing
- Lightweight and portable

## 🚀 Installation

### Requirements

- `gcc`, `make`, `git`
- A browser (GUI or terminal)

### Install Steps

- git clone https://github.com/bluesboynix/offarchwiki.git
- cd offarchwiki
- make
- make install

## 📖 Usage
#### offarchwiki [options] [keyword]

| Option         | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| `-b <browser>` | Use a specific browser (e.g., `firefox`, `w3m`, `lynx`)        |
| `-c`           | Use the first available terminal browser (`w3m`, `lynx`, etc.) |
| `-l <pattern>` | List matching wiki pages                                       |
| `-h`           | Show help                                                      |
| `-v`           | Show version info                                              |

## Example

#### Open pacman page with default browser:
$ offarchwiki pacman

#### Open with terminal browser:
$ offarchwiki -c systemd

#### Use Firefox:
$ offarchwiki -b firefox makepkg

#### List pages containing the word "boot":
$ offarchwiki -l boot

#### Show the main page:
$ offarchwiki

## 🔧 Uninstall
$ sudo make uninstall
