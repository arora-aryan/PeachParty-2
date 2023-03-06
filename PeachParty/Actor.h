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
    Actor(StudentWorld* world, int imageID, int startX, int startY, int depth) : GraphObject(imageID, startX, startY, right, depth, 1.0), m_world(world), sprite_direction(0), m_impactable(false), m_alive(true), m_beenhit(false) {}
    
    virtual void doSomething() = 0;
    void setWalkAngle(int angle) { m_walk_angle = angle; }
    StudentWorld* getWorld() { return m_world; }
    int getWalkAngle() const { return m_walk_angle; }
    virtual ~Actor() {}
    virtual void deadEnd();
    virtual void setImpact(){m_impactable = true;};
    bool impactable() const {return m_impactable;};
    void die() {m_alive = false; return;}
    bool toBeDeleted() const {return !m_alive;};
    bool beenHit() const {return m_beenhit;};
    void setHit(bool h){m_beenhit = h;};
    
  private:
    StudentWorld* m_world;
    int sprite_direction;
    int m_walk_angle;
    bool m_impactable;
    bool m_alive;
    bool m_beenhit;

};

class Player : public Actor
{
  public:
    Player(StudentWorld* world, int imageID, int startX, int startY, int playerNum) : Actor(world, imageID, startX, startY, 0), ticks_to_move(0), waiting_to_roll(true), m_playernumber(playerNum), m_activation(false), m_numStars(0), waiting_to_move(false), m_bankActivation(false), m_eventActivation(true), vortex(true), m_bowserActivation(true)
    {
        start_x = getX()/SPRITE_WIDTH;
        start_y = getY()/SPRITE_HEIGHT;
    }
    
    void allowAllActivation()
    {   
        m_activation = true;
        m_bankActivation = true;
        m_eventActivation = true;
        m_bowserActivation = true;
        m_booActivation = true;
    }
    void createVortex();
    void addStar() { m_numStars += 1;}
    void updateCoinBalance(int coins) { m_numCoins += coins;}
    void setStarBalance(int stars) { m_numStars = stars;}
    void setCoinBalance(int coins) { m_numCoins = coins;}
    int numCoins() const {return m_numCoins;}
    int numStars() const {return m_numStars;}
    virtual void doSomething();
    int dieRoll();
    virtual ~Player() {}
    bool isWaiting() const {return waiting_to_roll;};
    bool canActivate() const {return m_activation;};
    void setActivation(bool act){m_activation = act;};
    bool atFork();
    void setWaiting(bool wait){waiting_to_move = wait;};
    bool hasTicks() const {return ticks_to_move != 0;};
    bool canActivateBank() const {return m_bankActivation;};
    void setBankActivation(bool act){m_bankActivation = act;};
    bool canActivateEvent() const {return m_eventActivation;};
    void setEventActivation(bool act){m_eventActivation = act;};
    void setTicks(int ticks){ticks_to_move = ticks;};
    int getTicks() const {return ticks_to_move;};
    void getVortex(){vortex = true;};
    bool hasVortex() const {return vortex;};
    bool canActivateBowser() const {return m_bowserActivation;};
    void setBowserActivation(bool act){m_bowserActivation = act;};
    void removeStars(){m_numStars = 0;};
    bool canActivateBoo() const {return m_booActivation;};
    void setBooActivation(bool act){m_booActivation = act;};

  private:
    bool m_booActivation;
    bool m_bowserActivation;
    bool vortex;
    bool m_eventActivation;
    bool m_bankActivation;
    bool waiting_to_move;
    int start_x;
    int start_y;
    bool m_activation;
    int ticks_to_move;
    bool waiting_to_roll;
    int m_numCoins;
    int m_numStars;
    int m_playernumber;
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

class Baddie : public Actor
{
  public:
    Baddie(StudentWorld* world, int iid, int board_x, int board_y) : Actor(world, iid, board_x, board_y, 0), m_paused(true), m_travel_distance(0), m_pause_counter(180), ticks_to_move(0), squares_to_move(0) {setImpact();}
    virtual void doSomething();
    virtual void doSomethingPaused() = 0;
    virtual int pickSquaresToMove() = 0;
    virtual bool isPaused() const {return m_paused;};
    virtual void setPaused(bool p){m_paused = p;};
    virtual void specialTrick() = 0;
    void hitResult();
    void setTicks(int t){ticks_to_move = t;};
    
  private:
    int ticks_to_move;
    int squares_to_move;
    bool m_paused;
    int m_travel_distance;
    int m_pause_counter;
};

class Bowser : public Baddie
{
  public:
    Bowser(StudentWorld* world, int board_x, int board_y) : Baddie(world, IID_BOWSER, board_x, board_y) {}

  private:
    void doSomethingPaused();
    virtual int pickSquaresToMove();
    virtual void specialTrick();
};

class Boo : public Baddie
{
  public:
    Boo(StudentWorld* world, int board_x, int board_y) : Baddie(world, IID_BOO, board_x, board_y) {}

  private:
    void doSomethingPaused();
    virtual int pickSquaresToMove();
    virtual void specialTrick(){return;};
};

class Vortex : public Actor
{
  public:
    Vortex(StudentWorld* world, int board_x, int board_y, int start_dir) : Actor(world, IID_VORTEX, board_x, board_y, 0) {setWalkAngle(start_dir);}
    virtual void doSomething();
  private:
    
};
#endif // ACTOR_H
