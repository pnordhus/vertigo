{
  description = "Vertigo";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
    supportedSystems = [ "x86_64-linux" "i686-linux" ];
    forAllSystems = f: nixpkgs.lib.genAttrs supportedSystems (system: f system);
    nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlay ]; });
  in {
    overlay = final: prev: {
      vertigo = with final; stdenv.mkDerivation rec {
        name = "vertigo";

        src = ./.;

        nativeBuildInputs = [
          cmake
          glm
          openal
          SDL2
          qt5.wrapQtAppsHook
        ];

        buildInputs = [
          qt5.qtbase
        ];
      };
    };

    packages = forAllSystems (system: {
      inherit (nixpkgsFor.${system}) vertigo;
    });

    defaultPackage = forAllSystems (system: self.packages.${system}.vertigo);
  };
}
