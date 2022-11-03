{
  description = "openGL practice project";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-22.05";
  };

  outputs = {
    self,
    nixpkgs,
    ...
  } @ inputs: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    llvm = pkgs.llvmPackages_latest;
  in {
    devShell.${system} =
      pkgs.mkShell.override {
        stdenv = pkgs.clangStdenv;
      } rec {
        packages = with pkgs; [
          python3Minimal
          bear
          clang-tools

          # debugger
          llvm.lldb
          gdb

          # libraries
          assimp
          glfw3
          xorg.libXrandr
          xorg.libXi
          xorg.libX11
        ];
      };
  };
}
