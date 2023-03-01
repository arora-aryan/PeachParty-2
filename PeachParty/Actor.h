#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
#include <random>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
struct Coords
{
    int x;
    int y;
};

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY): GraphObject(imageID, startX, startY){}
    virtual void doSomething() = 0;
    void setAlive(bool alive);
    bool isAlive() const {return m_alive;};
    bool isValidSpot(const int x, const int y) const;
    void getEmptySpots(std::vector<Coords> empty_coords);
    virtual void chooseDirection();
    
  private:
    int m_angle = right;
    bool m_alive;
    std::vector<Coords> m_empty_coords;

};

class Vortex: public Actor
{
  public:
    Vortex(int imageID, int startX, int startY): Actor(imageID, startX, startY){}
    void doSomething() override;

  private:
};

class Player: public Actor
{
  public:
    Player(int imageID, int startX, int startY) : Actor(imageID, startX, startY), numCoins(0), numStars(0), ticks_to_move(0), m_has_vortex(true), waiting_to_roll(true), m_action(0), m_firesound(false)
    {
        start_x = getX()/BOARD_WIDTH;
        start_y = getY()/BOARD_HEIGHT;
        setDirection(0);
    }
    
    void doSomething() override;
    bool hasVortex() const {return m_has_vortex;};
    bool rollStatus() const {return waiting_to_roll;};
    void setRollStatus(bool roll){waiting_to_roll = roll;};
    void setActionNum(int action){m_action = action;};
    bool fireSound(){return m_firesound;};
    void chooseDirection() override;
    void getDirSquares(std::vector<Coords> r_coords, std::vector<Coords> l_coords, std::vector<Coords> d_coords, std::vector<Coords> u_coords);
    void onDirSquare();
    bool isOnDir(){return dirTF;};
    void setAngle(int angle){m_angle = angle;};

  private:
    //Coordinates m_pairs;
    bool dirTF = false;
    int numCoins;
    int numStars;
    int ticks_to_move;
    bool m_has_vortex;
    bool waiting_to_roll;
    int m_action;
    int start_x;
    int start_y;
    bool m_firesound;
    int m_angle;
    std::vector<Coords> m_r_coords;
    std::vector<Coords> m_l_coords;
    std::vector<Coords> m_d_coords;
    std::vector<Coords> m_u_coords;
    
};

class Baddie: public Actor
{
  public:
    Baddie(int imageID, int startX, int startY): Actor(imageID, startX, startY){}
    void doSomething() override;

  private:

};

class Square: public Actor
{
  public:
    Square(int imageID, int startX, int startY): Actor(imageID, startX, startY){}
    virtual void doSomething() = 0;

  private:
};

class DirSquare: public Square
{
  public:
    DirSquare(int imageID, int startX, int startY): Square(imageID, startX, startY){}
    void doSomething() override;

  private:
    
};

class EventSquare: public Square
{
  public:
    EventSquare(int imageID, int startX, int startY): Square(imageID, startX, startY){}
    void doSomething() override;

  private:
    
};

class StarSquare: public Square
{
  public:
    StarSquare(int imageID, int startX, int startY): Square(imageID, startX, startY){}
    void doSomething() override;

  private:
    
};

class BankSquare: public Square
{
  public:
    BankSquare(int imageID, int startX, int startY): Square(imageID, startX, startY){}
    void doSomething() override;

  private:
    
};


class CoinSquare: public Square
{
  public:
    CoinSquare(int imageID, int startX, int startY): Square(imageID, startX, startY){}
    virtual void doSomething();

  private:
    
};

class BlueCoinSquare: public CoinSquare
{
  public:
    BlueCoinSquare(int imageID, int startX, int startY): CoinSquare(imageID, startX, startY){}

  private:
    
};

class RedCoinSquare: public CoinSquare
{
  public:
    RedCoinSquare(int imageID, int startX, int startY): CoinSquare(imageID, startX, startY){}

  private:
    
};

/*
 IID_PEACH
 IID_YOSHI
 IID_BLUE_COIN_SQUARE
 IID_RED_COIN_SQUARE
 IID_DIR_SQUARE
 IID_EVENT_SQUARE
 IID_BANK_SQUARE
 IID_STAR_SQUARE
 IID_DROPPING_SQUARE
 IID_BOWSER
 IID_BOO
 IID_VORTEX
 */

#endif // ACTOR_H_
