//
// Created by Anna on 11/10/2022.
//

#ifndef ALIFE_FOOD_H
#define ALIFE_FOOD_H

namespace std {

    class Food {
    private:
        int energy;    //Amount of energy in the food
        float decay;   // rate at which food decays (looses energy). Range (0,1)
        int type;      // indicates type of food. **currently placeholder**
    public:
        Food(int, float, int);
        void foodStep();
        int getEnergy();
        int getType();


    };

} // std

#endif //ALIFE_FOOD_H
