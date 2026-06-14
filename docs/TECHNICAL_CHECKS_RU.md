# Технические проверки

## Asset table

Проверить `src/rus_assets_table.c`:

```text
asset_count == entries == 976
duplicate asset ids == []
```

Обязательные диапазоны:

```text
0x109D
0x12DB..0x12ED
0x1407..0x1424
```

## Запрещённые опасные маршруты

Не возвращать:

```text
RECOMP_FORCE_PATCH dialogBin_get
RECOMP_FORCE_PATCH dialogBin_release
```

Не возвращать старую замену Furnace Fun map model через `0x14E8`.

## Принятая релизная база

Текущий релиз основан на `probe39`.
