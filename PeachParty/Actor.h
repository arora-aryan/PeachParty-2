#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Board.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth, double size) : GraphObject(imageID, startX, startY, right, 0, 1.0), m_world(world), sprite_direction(0) {}
    
    virtual void doSomething() = 0;
    StudentWorld* getWorld() { return m_world; }
    void setWalkDirection(int walkdir) { m_walkdirection = walkdir; }
    int getWalkDirection() const { return m_walkdirection; }
    virtual ~Actor() {}
    
    
private:
    StudentWorld* m_world;
    int sprite_direction;
    int m_walkdirection;
};

class Player : public Actor
{
public:
    Player(StudentWorld* world, int imageID, int startX, int startY, int playerNum) : Actor(world, imageID, startX, startY, right, 0, 1.0), ticks_to_move(0), waiting_to_roll(true), m_playernumber(playerNum), alive(true), m_activation(false) {}
    
    void updateCoinBalance(int coins) { m_numCoins += coins;}
    int numCoins() const { return m_numCoins; }
    void die() { alive = false; return; }
    virtual void doSomething();
    int dieRoll();
    virtual ~Player() {}
    bool isWaiting(){return waiting_to_roll;};
    bool canActivate(){return m_activation;};
    void setActivation(bool act){m_activation = act;};
    
private:
    bool m_activation;
    int ticks_to_move;
    bool waiting_to_roll;
    int m_numCoins;
    int m_playernumber;
    bool alive;

};

class Peach : public Player
{
public:
    Peach(StudentWorld* world, int board_x, int board_y, int playerNum) : Player(world, IID_PEACH, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y, 1) {}
    
private:
};

class Yoshi : public Player
{
public:
    Yoshi(StudentWorld* world, int board_x, int board_y, int playerNum) : Player(world, IID_YOSHI, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y, 2) {}
    
private:
};

class Square : public Actor
{
public:
    Square(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, right, 1, 1.0) {}
    virtual ~Square() {}
    void intersection();
private:
};

class CoinSquare : public Square
{
public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY) : Square(world, imageID, startX, startY), m_active(true) {}
    void coinDifference(int coins) { m_player->updateCoinBalance(coins); }
    bool isActive() { return m_active; }
    virtual void doSomething();
    virtual ~CoinSquare() {}
    
    //virtual void activate(){m_active = true;};
    
private:
    Player* m_player;
    bool m_active;
    //virtual void doSomethingElse() = 0;
};
    
class BlueCoinSquare : public CoinSquare
{
public:
    BlueCoinSquare(StudentWorld* world, int board_x, int board_y) : CoinSquare(world, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y) {}
    
private:
    //virtual void doSomethingElse();
};
    
class RedCoinSquare : public CoinSquare
{
public:
    RedCoinSquare(StudentWorld* world, int board_x, int board_y) : CoinSquare(world, IID_RED_COIN_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y) {}
    
private:
    //virtual void doSomethingElse();
};

class DirSquare : public Square
{
public:
    DirSquare(StudentWorld* world, int board_x, int board_y) : Square(world, IID_DIR_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y) {}
    virtual void doSomething() = 0;

    
private:
};






#endif // ACTOR_H_
