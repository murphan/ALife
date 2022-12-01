#ifndef ALIFE_FOOD_H
#define ALIFE_FOOD_H
/*
// Food is consumed by organisims to get energy. It spawns
// in mapCells when the game is started and then is generated
// by other organisims when they die.
*/
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

#endif //ALIFE_FOOD_H
