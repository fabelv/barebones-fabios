{
  description = "Cross-compilation environment for i686-elf kernel development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = [
            # Toolchain used for compiling
            pkgs.pkgsCross.i686-embedded.buildPackages.gcc
            pkgs.pkgsCross.i686-embedded.buildPackages.gcc.cc
            pkgs.pkgsCross.i686-embedded.newlib
            pkgs.gtest.dev
            pkgs.nasm

            # utility
            pkgs.libisoburn # for creating iso images
            pkgs.qemu # for emulating and virtualizing
            pkgs.cmake

            # only used for lsp purposes not compiling
            pkgs.bear
            pkgs.clang-tools
            pkgs.asm-lsp
          ];

          shellHook = ''
            export GCC="${pkgs.pkgsCross.i686-embedded.buildPackages.gcc}"
            export GCC_ROOT="${pkgs.pkgsCross.i686-embedded.buildPackages.gcc.cc}"
            export NEW_LIB="${pkgs.pkgsCross.i686-embedded.newlib}"
          '';
        };
      });
}


