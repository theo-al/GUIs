{ pkgs ? import <nixpkgs> {} }:

let
  manpages = true;
in
pkgs.mkShell {
  packages = with pkgs; [
    gcc

    SDL2 SDL2_gfx SDL2_image
    gnumake pkg-config
  ] ++ (
    if manpages then [
      man man-pages man-pages-posix
      gnumake.man pkg-config.man
    ] else []
  );

  shellHook = with pkgs; (if manpages then ''
    export MANPATH="${pkg-config.man}/share/man:${MANPATH:-}"
    export MANPATH="${gnumake.man}/share/man:${MANPATH:-}"
  '' else "");
}
