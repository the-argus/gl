#{ pkgs ? import <nixpkgs> {} }:
{
  description = "openGL practice project";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-22.05";
    nixpkgs-unstable.url = "nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, nixpkgs-unstable, ... }@inputs:
    let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; };
      unstable = nixpkgs-unstable.legacyPackages.${system};
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
alias build="python build.py"
                '';
                nativeBuildInputs = with pkgs; [
                    unstable.neovim
                    gcc
                    python3Minimal
                    bear

                    # libraries
                    glfw3
                    xorg.libXrandr
                    xorg.libXi
                    xorg.libX11
                ];
            };
    };
}
