#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    Board getBoard() { return m_board; }
    void coinPlayerOverlap();
    
    //void activateCoinsPeach();
    //void activateCoinsYoshi();

    
private:
    Player* m_peach;
    Player* m_yoshi;
    std::vector<Actor*> m_actors;
    std::vector<Actor*> m_coinsquares;

    Board m_board;
};

#endif // STUDENTWORLD_H_
