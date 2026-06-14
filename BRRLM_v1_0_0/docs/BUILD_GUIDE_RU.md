# Сборка мода

## Требования

- `clang`
- `ld.lld`
- Linux-версия `RecompModTool` лежит в `tools/linux/RecompModTool`.

## Сборка ELF

```bash
make clean
make -j2
```

Ожидаемый результат:

```text
build/mod.elf
```

## Сборка NRM

```bash
./tools/linux/RecompModTool mod.toml release
```

Ожидаемый результат:

```text
release/Russian_Language_Mod_v1_0_0.nrm
```

## Проверка важной базы

Эта версия должна сохранять game payload ветки `probe39`. Перед выпуском проверять, что игровые бинарные части не изменились случайно.
