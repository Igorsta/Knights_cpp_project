#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <limits>
#include <compare>

class Knight {
public:
    using order = std::weak_ordering;
    
    static constexpr size_t MAX_GOLD = std::numeric_limits<size_t>::max();

    //constructors
    constexpr Knight(size_t gold, size_t weapon_class, size_t armor_class) :
        _gold(gold), _weapon_class(weapon_class), _armour_class(armor_class) {};
    
    ~Knight() = default;
    Knight(const Knight&) = default;
    Knight(Knight&&) noexcept = default;
    Knight& operator=(const Knight&) = default;
    Knight& operator=(Knight&&) noexcept = default;

    //destructive accessors
    inline constexpr size_t give_gold() noexcept { return reset(_gold); }
    inline constexpr size_t give_up_weapon() noexcept { return reset(_weapon_class); }
    inline constexpr size_t take_off_armour() noexcept { return reset(_armour_class); }

    //accessors    
    inline constexpr size_t get_gold() const noexcept { return _gold; }
    inline constexpr size_t get_weapon_class() const noexcept { return _weapon_class; }
    inline constexpr size_t get_armour_class() const noexcept { return _armour_class; }

    //setters
    inline constexpr void take_gold(const size_t amount) noexcept { _gold = safe_add(_gold, amount); }
    inline constexpr void change_weapon(size_t new_class) noexcept { _weapon_class = new_class; }    
    inline constexpr void change_armour(size_t new_class) noexcept { _armour_class = new_class; }

    //operators
    Knight& operator+=(Knight& other) {
        _gold = safe_add(_gold, reset(other._gold));

        get_if_better(_weapon_class, other._weapon_class);
        get_if_better(_armour_class, other._armour_class);

        return *this;
    }

    constexpr Knight operator+(const Knight& other) const {
        return Knight(
            safe_add(_gold, other._gold),
            std::max(_weapon_class, other._weapon_class),
            std::max(_armour_class, other._armour_class)
        );
    }
    
    constexpr auto operator<=>(const Knight& other) const {
        bool this_vulnerable   = other._weapon_class > _armour_class;
        bool other_vulnerable  = _weapon_class > other._armour_class;

        bool diff_vulnerablity = this_vulnerable ^ other_vulnerable;

        if (diff_vulnerablity)
            return (other_vulnerable ? order::greater : order::less);

        if (!this_vulnerable)
            return order::equivalent;
        
        if (_armour_class != other._armour_class)
            return  std::weak_ordering(_armour_class <=> other._armour_class);
        
        return  std::weak_ordering(_weapon_class <=> other._weapon_class);
    }
    
    constexpr bool operator==(const Knight& other) const {
        return (*this <=> other) == order::equivalent;
    };


    //output
    friend std::ostream& operator<<(std::ostream& os, const Knight& knight) {
        os << "(" << knight._gold <<
            " gold, " << knight._weapon_class <<
            " weapon class, " << knight._armour_class <<
            " armour class)\n";
        return os;
    }

private:
    constexpr inline size_t safe_add(const size_t a, const size_t b) const {
        assert(a <= MAX_GOLD);
        return a + std::min(b, MAX_GOLD - a);
    }

    constexpr inline void get_if_better(size_t& mine, size_t& other) {
        if (mine < other)
            mine = reset(other);
    }

    constexpr inline size_t reset(size_t& var) {
        size_t tmp = var;
        var = 0;
        return tmp;
    }

    size_t _gold;
    size_t _weapon_class;
    size_t _armour_class;
};

constinit const Knight TRAINEE_KNIGHT = Knight(0, 1, 1);

class Tournament {
public:
    using order = std::weak_ordering;
    
    //constructors
    Tournament(const std::initializer_list<Knight>& contestants) : 
        _contestants(contestants.size() ? contestants : std::list<Knight>({TRAINEE_KNIGHT})), 
        _eliminated((std::list<Knight>){}) {};
    
    Tournament(const Tournament& other) : 
        _contestants(other._contestants),
        _eliminated((std::list<Knight>){}) {};
    
    Tournament(Tournament&& other) :
        _contestants(other._contestants),
        _eliminated((std::list<Knight>){}) {};    
    
    Tournament& operator=(const Tournament&) = default;
    Tournament& operator=(Tournament&&) noexcept = default;

    //accessors
    std::list<Knight>::const_iterator no_winner() const noexcept{ return _contestants.end(); }
    size_t size() const noexcept{ return _contestants.size() + _eliminated.size(); }
    
    //operators
    Tournament& operator+=(const Knight& knight) { 
        _contestants.push_back(knight);
        _eliminated.clear();
        return *this;
    }

    Tournament& operator-=(const Knight& knight) {
        _eliminated.clear();

        auto filter = [knight](const Knight& k) {
            return  k.get_gold() == knight.get_gold() &&
                    k.get_armour_class() == knight.get_armour_class() &&
                    k.get_weapon_class() == knight.get_weapon_class();
        };
        
        _contestants.erase(
            std::remove_if(_contestants.begin(), _contestants.end(), filter),
            _contestants.end()
        );
        return *this;
    }

    //methods
    std::list<Knight>::const_iterator play() {
        _eliminated.clear();

        while (_contestants.size() >= 2) {
            Knight first = get_front();
            Knight second = get_front();
            
            auto comparison = first <=> second;

            reward(first, second, comparison);

            judge(second, order::less, comparison);
            judge(first, order::greater, comparison);
        }
        
        return (_contestants.empty() ? _contestants.end() : _contestants.begin());
    }
    
    //output
    friend std::ostream& operator<<(std::ostream& os, const Tournament& t) {
        for (const Knight& k : t._contestants)
            os << "+ " << k;

        for (const Knight& k : t._eliminated)
            os << "- " << k;

        os << "=" << std::endl;

        return os;
    }

private:
    inline Knight get_front(){
        Knight ans = _contestants.front();
        _contestants.pop_front();
        return ans; 
    }

    inline void reward(Knight& first, Knight& second, const order& res){
        if(res == order::greater)
            first += second;
        
        if(res == order::less)
            second += first;
    }

    inline void judge(const Knight& knight, const order& desired, const order& verdict){
        if (verdict == desired)
            _contestants.push_back(knight);
        else
            _eliminated.push_front(knight);
    }

    std::list<Knight> _contestants;
    std::list<Knight> _eliminated;
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
