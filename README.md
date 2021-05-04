# Project ls Clone

This project was made for the c module at the IUT Nancy-Charlemagne (2020-2021).
It aims to reproduce some features of the ls command of unix os.

## Installation
Use the makefile command `make` to create the executable `ls`.
```bash
make
```

## Usage
```bash
./ls [path] [-lia]
```
When no path is given, will show files list of ls program directory.

Options:

* -l    shows all information of the directory: rights, size, last date of edit, file name
* -i    display inode of the files
* -a    also display hidden files

