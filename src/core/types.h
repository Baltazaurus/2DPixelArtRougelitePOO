#pragma once
#include <stdexcept>
#include <string>

///// ////////////////////////////////////////////////////ENUM
enum class EntityType { PLAYER, ENEMY, NPC };
enum class DamageType { PHYSICAL, MAGICAL };
enum class ItemType { CONSUMABLE, WEAPON };
enum class GameState { MAIN_MENU, PLAYING, WAVE_COMPLETE, GAME_OVER };



//////////////////////////////////////////////////////////////////////////exceptii
class InvalidTargetException : public std::runtime_error {
public:
    explicit InvalidTargetException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    explicit ItemNotFoundException(const std::string& msg)
        : std::runtime_error(msg) {}
};
