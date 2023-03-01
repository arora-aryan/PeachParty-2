#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <utility>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

  private:
    std::vector<Actor*> m_actors;
    std::vector<Coords> m_empty_coords;
    std::vector<Coords> m_up_coords;
    std::vector<Coords> m_down_coords;
    std::vector<Coords> m_left_coords;
    std::vector<Coords> m_right_coords;
    Coords m_empty;
    Coords m_up;
    Coords m_down;
    Coords m_left;
    Coords m_right;
    Player* m_peach;
    Player* m_yoshi;
    Baddie* m_bowser;
    Baddie* m_boo;
    BlueCoinSquare* m_blueCoin;
    RedCoinSquare* m_redCoin;
    StarSquare* m_starSquare;
    BankSquare* m_bankSquare;
    EventSquare* m_eventSquare;
    DirSquare* m_leftDir;
    DirSquare* m_rightDir;
    DirSquare* m_upDir;
    DirSquare* m_downDir;
    Vortex* m_vortex;
};
/*
IID_DIR_SQUARE
IID_EVENT_SQUARE
IID_BANK_SQUARE
IID_STAR_SQUARE
IID_DROPPING_SQUARE
IID_BOWSER
IID_BOO
IID_VORTEX
 */
#endif // STUDENTWORLD_H_
