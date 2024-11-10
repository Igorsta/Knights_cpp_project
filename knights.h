#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <limits>
class Knight {
public:
    using order = std::strong_ordering;
    static constexpr size_t MAX_GOLD = std::numeric_limits<size_t>::max();

    constexpr Knight(size_t gold, size_t weapon_class, size_t armor_class) :
        _gold(gold), _weapon_class(weapon_class), _armour_class(armor_class) {};
    
    ~Knight() = default;
    Knight(const Knight&) = default;
    Knight(Knight&&) noexcept = default;
    Knight& operator=(const Knight&) = default;
    Knight& operator=(Knight&&) noexcept = default;

    inline constexpr size_t take_gold() noexcept { return set_to_0(_gold); }
    inline constexpr size_t give_up_weapon() noexcept { return set_to_0(_weapon_class); }
    inline constexpr size_t take_off_armour() noexcept { return set_to_0(_armour_class); }
    
    inline constexpr size_t get_gold() const noexcept { return _gold; }
    inline constexpr size_t get_weapon_class() const noexcept { return _weapon_class; }
    inline constexpr size_t get_armour_class() const noexcept { return _armour_class; }

    inline constexpr void take_gold(const size_t amount) noexcept { _gold = safe_add(_gold, amount); }
    inline constexpr void change_weapon(size_t new_class) noexcept { _weapon_class = new_class; }    
    inline constexpr void change_armour(size_t new_class) noexcept { _armour_class = new_class; }

    Knight& operator+=(Knight& other) {
        _gold = safe_add(_gold, set_to_0(other._gold));

        compare(_weapon_class, other._weapon_class);
        compare(_armour_class, other._armour_class);

        return *this;
    }

    constexpr Knight operator+(const Knight& other) const {
        return Knight(
            safe_add(_gold, other._gold),
            std::max(_weapon_class, other._weapon_class),
            std::max(_armour_class, other._armour_class)
        );
    }
    
    order operator<=>(const Knight& other) const {\
        bool this_beats_other = (_weapon_class > other._armour_class) && (_armour_class >= other._weapon_class);
        bool other_beats_this = (other._weapon_class > _armour_class) && (other._armour_class >= _weapon_class);

        if (this_beats_other && !other_beats_this)
            return order::greater;
        
        if (other_beats_this && !this_beats_other)
            return order::less;
        
        if (_armour_class != other._armour_class)
            return  _armour_class < other._armour_class ? 
                    order::less :
                    order::greater;
        
        return  _weapon_class < other._weapon_class ? order::less :
                _weapon_class > other._weapon_class ? order::greater :
                order::equal;
    }
    
    constexpr bool operator==(const Knight& other) const {
        return (_weapon_class <= other._armour_class && other._weapon_class <= _armour_class) ||
                (_weapon_class == other._weapon_class && _armour_class == other._weapon_class);
    };

    friend std::ostream& operator<<(std::ostream& os, const Knight& knight) {
        os << "(" << knight._gold <<
            ", " << knight._weapon_class <<
            ", " << knight._armour_class <<
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

constexpr const Knight TRAINEE_KNIGHT = Knight(0, 0, 1);

class Tournament {
public:
    Tournament(const std::vector<Knight>& contestants) : 
        _contestants(contestants.empty() ? (std::vector<Knight>){TRAINEE_KNIGHT} : contestants), 
        _eliminated((std::vector<Knight>){}) {};
    
    Tournament(const Tournament& other) : 
        _contestants(other._contestants),
        _eliminated () {};
    Tournament(Tournament&&) noexcept = default;
    Tournament& operator=(const Tournament&) = default;
    Tournament& operator=(Tournament&&) noexcept = default;


    void operator-=(const Knight& knight) {

    }

    std::vector<Knight>::const_iterator play();
    
    void operator+=(const Knight& knight) { _contestants.push_back(knight); }
    std::vector<Knight>::const_iterator no_winner() const noexcept{ return _contestants.end(); }

    size_t size() const noexcept{ return _contestants.size(); }

    friend std::ostream& operator<<(std::ostream& os, const Knight& Tournament) {
        return os;
    }

private:
    std::vector<Knight> _contestants;
    std::vector<Knight> _eliminated;
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