@echo off
meson builddir --buildtype=%1
ninja -C builddir