{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    gcc gnumake

    SDL2 libGL libGLU
  ];
}

