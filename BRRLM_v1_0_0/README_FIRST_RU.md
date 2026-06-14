# Russian Language Mod v1.0.0 — source notes

Current game payload: `probe39`.

This source tree contains the current public release source for the Russian Language Mod for Banjo: Recompiled.

## Accepted final fix chain

- `probe34` — Brentilda broom fact repair: asset `0x109D`.
- `probe37` — post-Furnace-Fun character parade credits route: suppress vanilla lower text and print Russian text in the parade route.
- `probe38` — stable Brentilda broom fact/question behavior across different save variants.
- `probe39` — ending jigsaw count numeric route fix.

## Do not use as base

`probe17..25`, `probe27`, `probe28`, `probe29`, `probe31`, `probe32`, `probe33`, `probe35`, `probe36`.

## Hard rules for future work

- Do not add `0x11AF..0x11C8` to `src/rus_assets_table.c`.
- Do not patch `dialogBin_get` or `dialogBin_release`.
- Do not restore `0x14E8` Furnace Fun map model replacement.
- Do not change the font without a separate task and separate testing.
- Do not change the mod version after `1.0.0` without an explicit release task.

## Required sanity checks

```text
asset_count == entries == 976
duplicate asset ids == []
0x109D exists
0x12DB..0x12ED exist
0x1407..0x1424 exist
dialogBin_get/dialogBin_release are not RECOMP_FORCE_PATCH targets
```

## Release build

```text
release/Russian_Language_Mod_v1_0_0.nrm
```

The game payload must match the accepted `probe39` build for `mod_binary.bin`, `mod_syms.bin`, and `rt64.json`.
