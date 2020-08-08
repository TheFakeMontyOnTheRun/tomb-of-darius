//
// Created by Daniel Monteiro on 2020-08-08.
//

#ifndef TOMB_OF_DARIUS_MAPS_H
#define TOMB_OF_DARIUS_MAPS_H



enum ECellType{ kNothing,
                kHero,
                kWall,
                kPrize,
                kAltar,
                kSword,
                kKey1,
                kKey2,
                kKey3,
                kKey4,
                kEnemyGenerator,
                kEnemy,
                kVictory,
                kGameOver
};

extern int8_t map[2][32][32];

#endif //TOMB_OF_DARIUS_MAPS_H
