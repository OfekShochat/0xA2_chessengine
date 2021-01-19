@echo off
meson build --buildtype=%1
ninja -C build