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
    Actor(StudentWorld* world, int imageID, int startX, int startY, int depth) : GraphObject(imageID, startX, startY, right, depth, 1.0), m_world(world), sprite_direction(0) {}
    
    virtual void doSomething() = 0;
    void setWalkAngle(int angle) { m_walk_angle = angle; }
    StudentWorld* getWorld() { return m_world; }
    int getWalkAngle() const { return m_walk_angle; }
    virtual ~Actor() {}
    
    
  private:
    StudentWorld* m_world;
    int sprite_direction;
    int m_walk_angle;
};

class Player : public Actor
{
  public:
    Player(StudentWorld* world, int imageID, int startX, int startY, int playerNum) : Actor(world, imageID, startX, startY, 0), ticks_to_move(0), waiting_to_roll(true), m_playernumber(playerNum), alive(true), m_activation(false), m_numStars(0), waiting_to_move(false)
    {
        start_x = getX() / SPRITE_WIDTH;
        start_y = getY()/SPRITE_HEIGHT;
    }
    
    void addStar() { m_numStars += 1;}
    void updateCoinBalance(int coins) { m_numCoins += coins;}
    int numCoins() const { return m_numCoins; }
    int numStars() const { return m_numStars; }
    void die() { alive = false; return; }
    virtual void doSomething();
    int dieRoll();
    virtual ~Player() {}
    bool isWaiting(){return waiting_to_roll;};
    bool canActivate(){return m_activation;};
    void setActivation(bool act){m_activation = act;};
    void deadEnd();
    bool atFork();
    void setWaiting(bool wait){waiting_to_move = wait;};
    bool chooseDirection(int dir);
    
  private:
    bool waiting_to_move;
    int start_x;
    int start_y;
    bool m_activation;
    int ticks_to_move;
    bool waiting_to_roll;
    int m_numCoins;
    int m_numStars;
    int m_playernumber;
    bool alive;
    bool right_open;
    bool left_open;
    bool up_open;
    bool down_open;

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
    Square(StudentWorld* world, int imageID, int startX, int startY, int startDirection) : Actor(world, imageID, startX, startY, 1) {}
    virtual ~Square() {}
    void intersection();
  private:
};

class CoinSquare : public Square
{
  public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY) : Square(world, imageID, startX, startY, right), m_active(true) {}
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
    DirSquare(StudentWorld* world, int startX, int startY, int startDirection) : Square(world, IID_DIR_SQUARE, startX, startY, startDirection) {setDirection(startDirection);}
    virtual void doSomething();

  private:
};

class StarSquare : public Square
{
  public:
    StarSquare(StudentWorld* world, int board_x, int board_y) : Square(world, IID_STAR_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y, right) {}
    virtual void doSomething();

};

class EventSquare : public Square
{
  public:
    EventSquare(StudentWorld* world, int board_x, int board_y) : Square(world, IID_EVENT_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y, right) {}
    virtual void doSomething();

};

class BankSquare : public Square
{
  public:
    BankSquare(StudentWorld* world, int board_x, int board_y) : Square(world, IID_BANK_SQUARE, SPRITE_WIDTH * board_x, SPRITE_HEIGHT * board_y, right) {}
    virtual void doSomething();

};

class Bowser : public Actor
{
  public:
    Bowser(StudentWorld* world, int board_x, int board_y) : Actor(world, IID_BOWSER, board_x, board_y, 0) {}
    virtual void doSomething();
};

class Boo : public Actor
{
  public:
    Boo(StudentWorld* world, int board_x, int board_y) : Actor(world, IID_BOO, board_x, board_y, 0) {}
    virtual void doSomething();
};

#endif // ACTOR_H_
