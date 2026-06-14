# Build

Requirements:

* clang
* lld
* make
* RecompModTool-compatible mod packaging tool

Build the ELF:

```sh
make clean
make
```

Package the mod:

```sh
./tools/linux/RecompModTool mod.toml mod_out
```

The packaged `.nrm` file will be written to the output folder.
