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

    Knight() = delete;
    constexpr Knight(size_t gold, size_t weapon_class, size_t armour_class) : gold_(gold), weapon_class_(weapon_class), armour_class_(armour_class) {};
    Knight(const Knight&) = default;
    Knight(Knight&&) noexcept = default;
    ~Knight() = default;

    Knight& operator=(const Knight&) = default;
    Knight& operator=(Knight&&) noexcept = default;

    // accessors
    constexpr size_t get_gold() const noexcept { return gold_; }
    
    constexpr size_t get_weapon_class() const noexcept { return weapon_class_; }

    constexpr size_t get_armour_class() const noexcept { return armour_class_; }

    // mutators
    size_t take_gold(size_t g) 
    {   
        size_t prev = gold_;
        gold_ = (g >= MAX_GOLD - gold_) ? MAX_GOLD : gold_ + g; 
        return prev;
    }
    size_t give_gold() { size_t prev = gold_; gold_ = 0; return prev; }

    size_t change_weapon(size_t w) { size_t prev = weapon_class_; weapon_class_ = w; return prev; }
    size_t give_up_weapon() { size_t prev = weapon_class_; weapon_class_ = 0; return prev; }

    size_t change_armour(size_t a) { size_t prev = armour_class_; armour_class_ = a; return prev; } 
    size_t take_off_armour() { size_t prev = armour_class_; armour_class_ = 0; return prev; }

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
    size_t gold_;
    size_t weapon_class_;
    size_t armour_class_;
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

constexpr std::pair<size_t, size_t> max_diff_classes(const std::list<Knight>& knights);

#endif // KNIGHTS_H
