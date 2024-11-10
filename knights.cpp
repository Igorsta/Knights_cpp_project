#include "knights.h"

using ord = std::strong_ordering;

const size_t Knight::MAX_GOLD; // initialize static member

Knight& Knight::operator+=(Knight& other) {
    gold_ = (other.gold_ >= MAX_GOLD - gold_) ? MAX_GOLD : gold_ + other.gold_;
    other.give_gold();

    if (other.weapon_class_ > weapon_class_) {
        weapon_class_ = other.weapon_class_;
        other.give_up_weapon();
    }

    if (other.armour_class_ > armour_class_) {
        armour_class_ = other.armour_class_;
        other.take_off_armour();
    }

    return *this;
}

constexpr Knight Knight::operator+(const Knight& other) const {
    return Knight(
        (other.gold_ >= MAX_GOLD - gold_) ? MAX_GOLD : gold_ + other.gold_, 
        std::max(weapon_class_, other.weapon_class_), 
        std::max(armour_class_, other.armour_class_)
    );
}

// can probably be simplified
constexpr ord Knight::operator<=>(const Knight& other) const {
    bool this_weapon_stronger = weapon_class_ > other.armour_class_;
    bool other_weapon_stronger = other.weapon_class_ > armour_class_;

    if (this_weapon_stronger && !other_weapon_stronger)
        return ord::greater; 

    if (!this_weapon_stronger && other_weapon_stronger)
        return ord::less;

    if (this_weapon_stronger && other_weapon_stronger) {
        if (armour_class_ != other.armour_class_)
            return armour_class_ <=> other.armour_class_;
        return weapon_class_ <=> other.weapon_class_;
    }
    
    return ord::equal;
}

constexpr bool Knight::operator==(const Knight& other) const {
    return (*this <=> other) == 0;
}

class Tournament {
public:
    Tournament(std::list<Knight> contestants);
    Tournament(const Tournament&) = default;
    Tournament(Tournament&&) noexcept = default;
    Tournament& operator=(const Tournament&) = default;
    Tournament& operator=(Tournament&&) noexcept = default;

    void operator+=(const Knight& knight);
    void operator-=(const Knight& knight);

    std::list<Knight>::const_iterator run();
    std::list<Knight>::const_iterator end() const noexcept;
    size_t size() const noexcept;

    void print() const;

private:
    std::list<Knight> contestants_;
    std::list<Knight> eliminated_;
};

constexpr std::pair<size_t, size_t> max_diff_classes(const std::list<Knight>& knights);