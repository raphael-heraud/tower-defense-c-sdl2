# Tower Defense - C & SDL2

Jeu de stratégie de type Tower Defense développé en langage C avec la bibliothèque graphique **SDL2**. 

L'ordinateur place aléatoirement et automatiquement des vagues d'ennemis terrestres et aériens dans le but de détruire la tour du Roi. 

Différentes tours sont placées stratégiquement pour défendre le Roi. Le chemin suivi par les ennemis de la Horde est retracé aléatoiremnt à chaque début de partie.

---

## Installation et compilation

### 1. Prérequis

Avoir un compilateur C (`gcc`), `make` et la bibliothèque **SDL2** selon le système.

* **Linux (Ubuntu / Debian) :**
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev
```

* **Windows (via MSYS2 / MinGW64) :**
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 make
```

### 2. Cloner et compiler

```bash
git clone https://github.com/raphael-heraud/tower-defense-c-sdl2
cd tower-defense-c-sdl2
make
```

### 3. Lancer le jeu

```bash
./towerdefense
```

Pour supprimer les fichiers objets (`.o`) et l'exécutable :

```bash
make clean
```

---

## Architecture du projet

```text
tower-defense-c-sdl2/
├── assets/          # Sprites des unités, tours et décors (.bmp)
├── include/         # Fichiers d'en-tête (.h)
├── src/             # Code source C 
└── Makefile         # Script de compilation
```