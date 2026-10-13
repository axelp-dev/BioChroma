#ifndef TYPES_H
#define TYPES_H


// Internal states of a bacterium
enum class BacteriumState {
    ALIVE,      // Active agent: moves, feeds, metabolizes
    DEAD,       // Inactive agent scheduled for cleanup
    DIVISION,   // Undergoing binary fission
    SPORE       // Dormant state under severe nutrient scarcity
};


#endif