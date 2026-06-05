#include "consumable.h"
#include "player.h"

void Consumable::use(Player& p) {
    p.getStats().heal(healAmount);
}
