# Task 8
## How to Use
If you want to change the value of `K`, you can edit it in `tree.h`.
Use bracket sequence for structure.

### Compile
```bash
$ cd "./Task 8/2019201423黄星童"
$ make
```

### Zip
```bash
$ ./treezip zip input.bin zipped.bin
```
`input.bin` is the name of input file and `zipped.bin` is the name of zipped file.

### Unzip
```bash
$ ./treezip unzip zipped.bin unzipped.bin
```
`unzipped.bin` is the name of unzipped file.

### Check
```bash
$ make std
$ ./std input.bin std.bin
$ diff std.bin unzipped.bin
```

### Clean
```bash
$ make clean
```
It will delete all the `.bin` files and the files created by `make` and `make std`.