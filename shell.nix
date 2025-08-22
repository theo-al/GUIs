{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    gcc

    SDL2 SDL2_image
    gnumake pkg-config

    #man man-pages man-pages-posix
  ];
}

