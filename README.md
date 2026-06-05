# Roguelite Dungeon — POO Project

A 2D top-down pixel art roguelite dungeon crawler built with **Godot 4.6** and **GDExtension (C++)** for the Object-Oriented Programming course at FMI UniBuc, Year 1.

---

## Overview

The player explores a dungeon, fights enemies in waves, collects health potions, and tries to survive as long as possible. The entire game logic is written in C++ using Godot's GDExtension system — no GDScript for gameplay logic.

---

## Features

- **Wave-based combat** — enemies spawn in increasing waves around the player
- **4-directional movement and animations** — run, attack, roll
- **Enemy AI** — idle, wander, chase, attack states with detection radius
- **Health system** — hearts UI, invincibility frames, blink animation on hit
- **Potion drops** — 1/5 chance on enemy death, instant heal on pickup
- **HUD** — wave counter, score, enemies remaining, wave announcement animation
- **C++ terminal demo** — standalone program demonstrating all OOP requirements

---

## Tech Stack

| Component | Technology |
|-----------|-----------|
| Engine | Godot 4.6.1 |
| Language | C++17 |
| Binding | GDExtension (godot-cpp) |
| Build | SCons |
| Demo | MSVC / CLion + CMake |

---

## Project Structure

```
testtemplate/
├── src/
│   ├── core/              # Pure C++ — no Godot dependency
│   │   ├── types.h        # Enums + custom exceptions
│   │   ├── stats.h/.cpp   # Stats class with operator overloads
│   │   ├── entity.h/.cpp  # Abstract base class
│   │   ├── player.h/.cpp  # Player class
│   │   ├── enemy.h/.cpp   # Enemy class
│   │   ├── item.h         # Abstract Item class
│   │   ├── consumable.h/.cpp  # Consumable : Item
│   │   ├── inventory.h    # std::list<Item*>
│   │   ├── game_manager.h/.cpp  # Singleton, std::stack
│   │   └── object_pool.h  # Template class ObjectPool<T>
│   ├── player_node.h/.cpp    # GDE CharacterBody2D wrapper
│   ├── enemy_node.h/.cpp     # GDE CharacterBody2D wrapper
│   ├── hud_node.h/.cpp       # GDE Control, HUD updates
│   ├── spawner_node.h/.cpp   # GDE wave spawner
│   ├── potion_node.h/.cpp    # GDE Area2D potion pickup
│   └── register_types.cpp    # GDE class registration
├── demo/
│   ├── main.cpp           # Standalone C++ terminal demo
│   └── CMakeLists.txt     # Build config for CLion
├── project/               # Godot project files
│   ├── scenes/
│   └── resources/
└── godot-cpp/             # GDExtension bindings (submodule)
```

---

## OOP Requirements Covered

### Tema 1
- 3+ classes with composition and aggregation
- Constructors, copy constructors, `operator=`, destructors
- `operator<<`, `operator>>`, `operator+`, `operator==` on `Stats`
- 4 complex public methods: `attackEnemy`, `pickupItem`, `levelUp`, `simulateCombat`
- Read and display N objects

### Tema 2
- Abstract classes: `Entity` (3 pure virtuals), `Item` (2 pure virtuals)
- Inheritance: `Player : Entity`, `Enemy : Entity`, `Consumable : Item`
- Parent constructor called in initializer list
- `const` on all getters
- Static members: `entityCount`, `spawnedCount`
- Custom exceptions: `InvalidTargetException`, `ItemNotFoundException`
- `try/catch` blocks in demo
- Explicit upcast and downcast with `dynamic_cast`
- `typeid` usage
- `std::list<Item*>` in `Inventory`
- `std::stack<GameState>` in `GameManager`
- 4 `enum class` types

### Tema 3
- `GameManager` singleton (private constructor, `getInstance()`)
- `ObjectPool<T>` template class
- `dynamic_cast` in production code (`Player::pickupItem`)
- Virtual functions with `override`

---

## Building

### Game (Godot + GDExtension)

```bat
cd testtemplate
scons platform=windows
```

Then open `project/` in Godot 4.6.

### Terminal Demo (CLion)

Open the `demo/` folder in CLion — it auto-detects `CMakeLists.txt`.
Press **Shift+F10** to run.

Or manually with MSVC Developer Command Prompt:

```bat
cd demo
cl /EHsc /std:c++17 /I.. main.cpp ..\src\core\stats.cpp ..\src\core\characters.cpp ..\src\core\player.cpp ..\src\core\consumable.cpp ..\src\core\entity.cpp ..\src\core\game_manager.cpp /Fe:demo.exe
demo.exe
```

---

## Controls

| Key | Action |
|-----|--------|
| WASD | Move |
| K | Attack |
| Space | Roll |

---

## Author

Cucule — FMI UniBuc, Year 1, POO Course, 2025
