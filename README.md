# kwin-effect-luminvert

KWin Desktop Effect to invert the luminance of window contents.

This has been developed for Plasma 6, but could probably be backported to Plasma 5.

## Installation

On Arch Linux, you can install the package `kwin-effect-luminvert-git` from the AUR.

Otherwise install manually:

```sh
cmake -B build -S . -Wno-dev
cmake --build build
sudo cmake --install build
```

## Compatibility

This has been tested on KWin 6.1.5.

It still works when HDR is enabled, however the colors get distorted.

## License

This repo is licensed under GPL-3.0-only.

This repo uses code from the invert plugin from https://github.com/KDE/kwin (imported from commit a00cad1).
Also, this repo uses CMake files from https://github.com/matinlotfali/KDE-Rounded-Corners (imported from commit 8b1ac40).
