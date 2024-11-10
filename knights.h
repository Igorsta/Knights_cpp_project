#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <iostream>
#include <list>
#include <utility>
#include <limits>
#include <compare>

class Knight {
public:
    static constexpr size_t MAX_GOLD = std::numeric_limits<size_t>::max();

    constexpr Knight(size_t gold, size_t weapon_class, size_t armour_class) : 
        _gold(gold), _weapon_class(weapon_class), _armour_class(armour_class) {};
    
    Knight() = delete;
    Knight(const Knight&) = default;
    Knight(Knight&&) noexcept = default;
    ~Knight() = default;

    Knight& operator=(const Knight&) = default;
    Knight& operator=(Knight&&) noexcept = default;

    // accessors
    inline constexpr size_t get_gold() const noexcept { return _gold; }
    inline constexpr size_t get_weapon_class() const noexcept { return _weapon_class; }
    inline constexpr size_t get_armour_class() const noexcept { return _armour_class; }

    // mutators
    inline constexpr void take_gold(const size_t amount) noexcept { _gold = safe_add(_gold, amount); }
    inline constexpr void change_weapon(size_t new_class) noexcept { _weapon_class = new_class; }    
    inline constexpr void change_armour(size_t new_class) noexcept { _armour_class = new_class; }

    inline constexpr size_t take_gold() noexcept { return set_to_0(_gold); }
    inline constexpr size_t give_up_weapon() noexcept { return set_to_0(_weapon_class); }
    inline constexpr size_t take_off_armour() noexcept { return set_to_0(_armour_class); }

    // operators
    Knight& operator+=(Knight& other);
    constexpr Knight operator+(const Knight& other) const;
    constexpr std::strong_ordering operator<=>(const Knight& other) const;
    constexpr bool operator==(const Knight& other) const;


    friend std::ostream& operator<<(std::ostream& os, const Knight& knight) {
        os << "(" << knight.get_gold() <<
            ", " << knight.get_weapon_class() <<
            ", " << knight.get_armour_class() <<
            ")" << std::endl;
        return os;
    }

private:
    constexpr inline size_t safe_add(const size_t a, const size_t b) const {
        return a + std::min(b, MAX_GOLD - a);
    }

    constexpr inline void compare(size_t& mine, size_t& other) {
        if (mine < other)
            mine = set_to_0(other);
    }

    constexpr inline size_t set_to_0(size_t& var) {
        size_t tmp = var;
        var = 0;
        return tmp;
    }

    size_t _gold;
    size_t _weapon_class;
    size_t _armour_class;
};

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

consteval std::pair<size_t, size_t> max_diff_classes(const std::initializer_list<Knight>& knights) {
    size_t max_diff = 0;
    std::pair<size_t, size_t> res = {0, 0};

    for (const auto& knight : knights) {
        size_t diff = // to avoid underflow
            knight.get_weapon_class() >= knight.get_armour_class() ? 
            knight.get_weapon_class() - knight.get_armour_class() : 
            knight.get_armour_class() - knight.get_weapon_class();
        
        if (diff > max_diff) {
            max_diff = diff;
            res = {knight.get_weapon_class(), knight.get_armour_class()};
        }
    }

    return res;
}

#endif // KNIGHTS_H