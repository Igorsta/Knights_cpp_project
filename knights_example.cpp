#include "knights.h"

#ifdef NDEBUG
  #undef NDEBUG
#endif

#include <iostream>
#include <cassert>

#define KNIGHT_PARAMS_ASSERTS(k, g, wc, ac) \
do { \
    assert(k.get_gold() == g); \
    assert(k.get_weapon_class() == wc); \
    assert(k.get_armour_class() == ac); \
} while(0) \

#define KNIGHT_PARAMS_STATIC_ASSERTS(k, g, wc, ac) \
do { \
    static_assert(k.get_gold() == g); \
    static_assert(k.get_weapon_class() == wc); \
    static_assert(k.get_armour_class() == ac); \
} while(0) \

namespace {
    using std::cout;
    using std::string;
    using std::make_pair;

    void knight_examples() {
        Knight knight(100, 21, 15);
        Knight black_knight = knight;
        Knight white_knight(knight);
        Knight little_knight = Knight(10, 20, 30);

        cout << knight << black_knight << little_knight;

        assert(knight.get_gold() == 100);
        knight.take_gold(20);
        assert(knight.get_gold() == 120);
        assert(knight.take_gold() == 120);
        assert(knight.get_gold() == 0);

        assert(knight.get_weapon_class() == 21);
        knight.change_weapon(27);
        assert(knight.get_weapon_class() == 27);
        assert(knight.give_up_weapon() == 27);
        assert(knight.get_weapon_class() == 0);

        assert(knight.get_armour_class() == 15);
        knight.change_armour(11);
        assert(knight.get_armour_class() == 11);
        assert(knight.take_off_armour() == 11);
        assert(knight.get_armour_class() == 0);

        Knight new_knight = white_knight + little_knight;
        KNIGHT_PARAMS_ASSERTS(new_knight, 110, 21, 30);
        KNIGHT_PARAMS_ASSERTS(white_knight, 100, 21, 15);
        KNIGHT_PARAMS_ASSERTS(little_knight, 10, 20, 30);

        white_knight += little_knight;
        KNIGHT_PARAMS_ASSERTS(white_knight, 110, 21, 30);
        KNIGHT_PARAMS_ASSERTS(little_knight, 0, 20, 0);

        new_knight = Knight(Knight::MAX_GOLD, 1, 1) + Knight(1, 2, 0);
        KNIGHT_PARAMS_ASSERTS(new_knight, Knight::MAX_GOLD, 2, 1);

        assert(black_knight > Knight(2000, 14, 20));
        assert(black_knight >= Knight(2000, 14, 20));
        assert(black_knight < Knight(25, 21, 16));
        assert(black_knight <= Knight(25, 21, 16));
        assert(black_knight == Knight(150, 21, 15));
        assert(black_knight != Knight(2000, 14, 20));
        
        constexpr Knight const_knight(1000, 10, 20);
        KNIGHT_PARAMS_STATIC_ASSERTS(const_knight, 1000, 10, 20);

        constexpr Knight const_knight2(120, 21, 20);

        constexpr Knight const_knight3 = const_knight + const_knight2;
    }
} // koniec przestrzeni anonimowej

int main() {
    knight_examples();
}
