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
    void encounterDirSquare();
    void starPlayerOverlap();
    void bankPlayerOverlap();
    void eventPlayerOverlap();
    bool isPlayerDirOverlap(int x, int y);
    int chooseRandomEvent();
    void swapPlayers();
    void randomTeleport(Player *m_player);
    
  private:
    static const int left = 180;
    static const int right = 0;
    static const int up = 90;
    static const int down = 270;
    int m_num_squares;
    int m_banktotal;
    Player* m_peach;
    Player* m_yoshi;
    std::vector<Actor*> m_actors;
    Board m_board;
};

#endif // STUDENTWORLD_H_
