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

## Example Usage
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
