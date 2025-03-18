# Vertigo

An open-source clone of the *Massive Development* video game [Archimedean Dynasty](https://en.wikipedia.org/wiki/Archimedean_Dynasty), also known as [Schleichfahrt](https://de.wikipedia.org/wiki/Schleichfahrt_%28Computerspiel%29).

This implementation requires the game assets from an original version of the game (either English or German).

If you use Nix with flakes enabled, you can build and start Vertigo by running:
```
nix run github:pnordhus/vertigo
```

# Compiling

```
mkdir build
cd build
cmake ..
make
```
