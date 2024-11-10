#include "knights.h"

using ord = std::strong_ordering;

const size_t Knight::MAX_GOLD; // initialize static member

Knight& Knight::operator+=(Knight& other) {
    _gold = safe_add(_gold, set_to_0(other._gold));

    compare(_weapon_class, other._weapon_class);
    compare(_armour_class, other._armour_class);
}

constexpr Knight Knight::operator+(const Knight& other) const {
    return Knight(
        safe_add(_gold, other._gold),
        std::max(_weapon_class, other._weapon_class),
        std::max(_armour_class, other._armour_class)
    );
}

// can probably be simplified
constexpr ord Knight::operator<=>(const Knight& other) const {
    bool this_weapon_stronger = _weapon_class > other._armour_class;
    bool other_weapon_stronger = other._weapon_class > _armour_class;

    if (this_weapon_stronger && !other_weapon_stronger)
        return ord::greater; 

    if (!this_weapon_stronger && other_weapon_stronger)
        return ord::less;

    if (this_weapon_stronger && other_weapon_stronger) {
        if (_armour_class != other._armour_class)
            return _armour_class <=> other._armour_class;
        return _weapon_class <=> other._weapon_class;
    }
    
    return ord::equal;
}

constexpr bool Knight::operator==(const Knight& other) const {
    return (*this <=> other) == 0;
}