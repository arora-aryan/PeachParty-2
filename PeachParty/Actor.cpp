#include "Actor.h"
#include "Board.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
    cerr<<"i have: "<<m_numCoins<<endl;
    
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
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
            cerr<<"my coins"<< m_numCoins << endl;
            
            bool rightOpen = (getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty);
            bool leftOpen = (getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty);
            bool upOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty);
            bool downOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty);
            
            if (getWalkDirection() == right && !rightOpen)
            {
                if(upOpen && downOpen)
                    setWalkDirection(up);
                else if (upOpen)
                    setWalkDirection(up);
                else if (downOpen)
                    setWalkDirection(down);
            }
            else if (getWalkDirection() == left && !leftOpen)
            {
                if(upOpen && downOpen)
                    setWalkDirection(up);
                else if (upOpen)
                    setWalkDirection(up);
                else if (downOpen)
                    setWalkDirection(down);
            }
            else if (getWalkDirection() == up && !upOpen)
            {
                if(rightOpen && downOpen)
                    setWalkDirection(right);
                else if (rightOpen)
                    setWalkDirection(right);
                else if (leftOpen)
                    setWalkDirection(left);
            }
            else if (getWalkDirection() == down && !downOpen)
            {
                if(rightOpen && downOpen)
                    setWalkDirection(right);
                else if (rightOpen)
                    setWalkDirection(right);
                else if (leftOpen)
                    setWalkDirection(left);
            }
        }
        
        if (getWalkDirection() == left) //update direction sprite faces
            setDirection(180);
        else
            setDirection(0);
        
        moveAtAngle(getWalkDirection(), 2); //Move two pixels in the walk direction.
        ticks_to_move--;
        if (ticks_to_move == 0)
            waiting_to_roll = true; //change avatar back to waiting to roll state
    }
}

void CoinSquare::doSomething()
{
    if(!m_active)
        return;
   
    getWorld()->coinPlayerOverlap();
    
    // m_active = false;
    
}

