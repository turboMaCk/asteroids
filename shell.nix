{ pkgs ? import <nixpkgs> {} }:
with pkgs;
mkShell {
  buildInputs = [
    gcc
    gdb
    pkg-config
    SDL2
    SDL2_image
    SDL2_ttf
  ];
}
