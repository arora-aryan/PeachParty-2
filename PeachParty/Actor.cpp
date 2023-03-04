#include "Actor.h"
#include "Board.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
    cerr<<"mystars: "<<numStars()<<endl;
    if (alive == false)
        return;
    
    if (waiting_to_roll == true)
    {
        switch(getWorld()->getAction(m_playernumber))
        {
            default:
                return;
            case ACTION_ROLL: {
                int die_roll = randInt(1,10);
                ticks_to_move = die_roll * 8;
                waiting_to_roll = false;
                break;
            }
            case ACTION_FIRE:
                break;
        }
        
    }
    if (waiting_to_roll == false)
    {
        m_activation = true;
        
        if((getX() % 16 == 0) && (getY() % 16 == 0))
        {
            deadEnd();
        }
        
        if (getWalkAngle() == left) //update direction sprite faces
            setDirection(180);
        else
            setDirection(0);
        
        moveAtAngle(getWalkAngle(), 2); //Move two pixels in the walk direction.
        ticks_to_move--;
        if (ticks_to_move == 0)
            waiting_to_roll = true; //change avatar back to waiting to roll state
    }
}
void Player::deadEnd()
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
    cerr<<"my coins"<< m_numCoins << endl;
    
    bool rightOpen = (getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty);
    bool leftOpen = (getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty);
    bool upOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty);
    bool downOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty);
    
    if (getWalkAngle() == right && !rightOpen)
    {
        if(upOpen && downOpen)
            setWalkAngle(up);
        else if (upOpen)
            setWalkAngle(up);
        else if (downOpen)
            setWalkAngle(down);
    }
    else if (getWalkAngle() == left && !leftOpen)
    {
        if(upOpen && downOpen)
            setWalkAngle(up);
        else if (upOpen)
            setWalkAngle(up);
        else if (downOpen)
            setWalkAngle(down);
    }
    else if (getWalkAngle() == up && !upOpen)
    {
        if(rightOpen && downOpen)
            setWalkAngle(right);
        else if (rightOpen)
            setWalkAngle(right);
        else if (leftOpen)
            setWalkAngle(left);
    }
    else if (getWalkAngle() == down && !downOpen)
    {
        if(rightOpen && downOpen)
            setWalkAngle(right);
        else if (rightOpen)
            setWalkAngle(right);
        else if (leftOpen)
            setWalkAngle(left);
    }
}


void CoinSquare::doSomething() {
    if(!m_active)
        return;
    getWorld()->coinPlayerOverlap();
}
void DirSquare::doSomething()
{
    getWorld()->encounterDirSquare();
}

//void RightDirSquare::doSomething(){}
//void LeftDirSquare::doSomething(){}
//void UpDirSquare::doSomething(){}
//void DownDirSquare::doSomething(){}

void StarSquare::doSomething()
{
    getWorld()->starPlayerOverlap();
}

void BankSquare::doSomething()
{
    //getWorld()->bankPlayerOverlap();
}

void EventSquare::doSomething(){}

void Bowser::doSomething(){}
void Boo::doSomething(){}

