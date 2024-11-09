#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <iostream>
#include <list>
#include <utility>
#include <limits>

class Knight {
public:
    static constexpr size_t MAX_GOLD = std::numeric_limits<size_t>::max();

    Knight(size_t gold, size_t weapon_class, size_t armor_class) : gold_(gold), weapon_class_(weapon_class), _armor_class(armor_class)  {};

    Knight(const Knight&) = default;
    Knight(Knight&&) noexcept = default;
    Knight& operator=(const Knight&) = default;
    Knight& operator=(Knight&&) noexcept = default;

    size_t getGold() const noexcept{
        return gold_;
    }
    
    size_t getWeaponClass() const noexcept{
        return weapon_class_;
    }

    size_t getArmorClass() const noexcept{
        return _armor_class;
    }
    
    void addGold(size_t amount){
        gold_ = std::min(gold_ + amount, MAX_GOLD);
    };

    void removeAllGold(){
        gold_ = 0;
    }

    void changeWeapon(size_t new_class){
        weapon_class_ = new_class;
    }
    
    void removeWeapon(){
        weapon_class_ = 0;
    }

    void changeArmor(size_t new_class){
        _armor_class = new_class;
    }

    void removeArmor(){
        _armor_class = 0;
    }

    Knight& operator+=(const Knight& other);

    Knight operator+(const Knight& other) const{
        return Knight(*this) += other; 
    }
    
    auto operator<=>(const Knight&) const = default;
    bool operator==(const Knight& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Knight& knight){
        os << "(" << knight.getGold() <<
            ", " << knight.getWeaponClass() <<
            ", " << knight.getArmorClass() <<
            ")" << std::endl;
        return os;
    }

private:
    size_t gold_;
    size_t weapon_class_;
    size_t _armor_class;
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
