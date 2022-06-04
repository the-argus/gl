{ pkgs ? import <nixpkgs> {} }:
{
  description = "openGL practice project";

  inputs = {
    #nixpkgs.url = "nixpkgs/nixos-21.11";
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      system = "${builtins.currentSystem}";
    in
    {
        devShell.${system} =
            pkgs.mkShell { 
                nativeBuildInputs = with pkgs; [
                    gcc
                    python
                    bear
                    glfw
                ];
            };
    };
}
