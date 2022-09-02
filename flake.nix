{
  description = "openGL practice project";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-22.05";
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      llvm = pkgs.llvmPackages_latest;
    in
    {
      devShell.${system} = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } rec {
        nativeBuildInputs = with pkgs; [
          python3Minimal
          bear
          clang
          clang-tools

          # debugger
          llvm.lldb
          gdb

          # lsp and std lib
          llvm.libstdcxxClang

          # libraries
          assimp
          llvm.libcxx
          llvm.libllvm
          glfw3
          xorg.libXrandr
          xorg.libXi
          xorg.libX11
        ];
      };
    };
}
