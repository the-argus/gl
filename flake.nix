#{ pkgs ? import <nixpkgs> {} }:
{
  description = "openGL practice project";

  inputs = {
    #nixpkgs.url = "nixpkgs/nixos-21.11";
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; };
    in
    {
        #defaultPackage.${system} = 
        #stdenv.mkDerivation {
        #    name = "glportals";
        #    src = self;
        #    buildPhase = "python build.py";
        #    # installPhase = "...
        #};
        devShell.${system} =
            pkgs.mkShell {
                shellHook = ''
alias build=python build.py
# get access to neovim while in devshell
alias nvim=${pkgs.neovim}/bin/nvim
                '';
                nativeBuildInputs = with pkgs; [
                    gcc
                    python3Minimal
                    bear

                    # libraries
                    glfw-wayland
                    xorg.libXrandr
                    xorg.libXi
                    xorg.libX11
                ];
            };
    };
}
