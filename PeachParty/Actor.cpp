#include "Actor.h"
#include "Board.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
    cerr<<"i have: " << numCoins() << "coins" << endl;
    cerr<<"i have " << numStars() << "stars" << endl;
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
    if (alive == false)
        return;
    
    bool overlapping_dir = getWorld()->isPlayerDirOverlap(board_x, board_y);
    
    if (waiting_to_roll == true && ((getX() % 16 == 0) && (getY() % 16 == 0)))
    {
        bool hasClicked = false;
        while(!hasClicked)
        {
            switch(getWorld()->getAction(m_playernumber))
            {
                default:
                    return;
                case ACTION_ROLL: {
                    int die_roll = randInt(1,10);
                    ticks_to_move = die_roll * 8;
                    waiting_to_roll = false;
                    hasClicked = true;
                    break;
                }
                case ACTION_FIRE:
                    hasClicked = true;
                    break;
            }
        }
        
    }
    
    if(atFork() == true && overlapping_dir == false && getX() % 16 == 0 && getY() % 16 == 0)
    {
        bool right_open = (getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty);
        bool left_open = (getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty);
        bool up_open = (getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty);
        bool down_open = (getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty);
        
        bool hasMoved = false;
        while(!hasMoved)
        {
            switch(getWorld()->getAction(m_playernumber))
            {
                default:
                    return;
                case ACTION_DOWN:
                    if(!down_open || getWalkAngle() == up)
                    {
                        break;
                    }
                    setWalkAngle(down);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
                case ACTION_UP:
                    cerr<<"press up"<<endl;
                    if(!up_open || getWalkAngle() == down)
                    {
                        cerr<<"you cant go" << up <<endl;
                        break;
                    }
                    cerr<<"yes im changing"<<endl;
                    cerr<<ticks_to_move<<endl;
                    setWalkAngle(up);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
                case ACTION_RIGHT:
                    cerr<<"press right"<<endl;
                    if(!right_open || getWalkAngle() == left)
                    {
                        cerr<<"you cant go" << right <<endl;
                        break;
                    }
                    cerr<<"yes im changing"<<endl;
                    cerr<<ticks_to_move<<endl;

                    setWalkAngle(right);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
                case ACTION_LEFT:
                    cerr<<"press left"<<endl;
                    if(!left_open || getWalkAngle() == right)
                    {
                        cerr<<"you cant go" << left <<endl;
                        break;
                    }
                    cerr<<"yes im changing"<<endl;
                    cerr<<ticks_to_move<<endl;
                    setWalkAngle(left);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
            }
        }
        if(ticks_to_move == 0)
            waiting_to_roll = true;
    }
    
    
    if (waiting_to_roll == false && ticks_to_move >= 0)
    {
        waiting_to_move = true;
        m_activation = true;
        m_bankActivation = true;
        m_eventActivation = true;
        
        if((getX() % 16 == 0) && (getY() % 16 == 0))
        {
            deadEnd();
        }
        
        if (getWalkAngle() == left)
            setDirection(180);
        else
            setDirection(0);
        
        moveAtAngle(getWalkAngle(), 2);
        ticks_to_move--;
        cerr<<"i got "<< ticks_to_move << "ticks" << endl;
        if (ticks_to_move == 0)
            waiting_to_roll = true;
    }
}

void Player::deadEnd()
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
        
    bool right_open = (getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty);
    bool left_open = (getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty);
    bool up_open = (getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty);
    bool down_open = (getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty);
    
    if (getWalkAngle() == right && !right_open)
    {
        if(up_open && down_open)
            setWalkAngle(up);
        else if (up_open)
            setWalkAngle(up);
        else if (down_open)
            setWalkAngle(down);
    }
    else if (getWalkAngle() == left && !left_open)
    {
        if(up_open && down_open)
            setWalkAngle(up);
        else if (up_open)
            setWalkAngle(up);
        else if (down_open)
            setWalkAngle(down);
    }
    else if (getWalkAngle() == up && !up_open)
    {
        if(right_open && down_open)
            setWalkAngle(right);
        else if (right_open)
            setWalkAngle(right);
        else if (left_open)
            setWalkAngle(left);
    }
    else if (getWalkAngle() == down && !down_open)
    {
        if(right_open && down_open)
            setWalkAngle(right);
        else if (right_open)
            setWalkAngle(right);
        else if (left_open)
            setWalkAngle(left);
    }
}

bool Player::atFork()
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
    if(start_x == board_x && start_y == board_y)
    {
        return false;
    }
    
    right_open = false;
    left_open = false;
    up_open = false;
    down_open = false;
    
    // Determine if the character has reached a fork in the path
    bool at_fork = false;
    
    // Count the number of available directions to move in
    int num_available_directions = 0;
    
    if(getWalkAngle() == right)
    {
        if((getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty)){
            num_available_directions++;
        }
        
        if((getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty)){
            num_available_directions++;

        }
        if((getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty)){
            num_available_directions++;
        }
        
    }
    else if(getWalkAngle() == left)
    {
        if((getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty)){
            num_available_directions++;
        }
        
        if((getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty)){
            num_available_directions++;
            
        }
        if((getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty)){
            num_available_directions++;
        }
    }
    else if(getWalkAngle() == up)
    {
        if((getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty)){
            num_available_directions++;
        }
        
        if((getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty)){
            num_available_directions++;
        }
        
        if((getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty)){
            num_available_directions++;
        }
        
    }
    else if(getWalkAngle() == down)
    {
        if((getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty)){
            num_available_directions++;
        }
        
        if((getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty)){
            num_available_directions++;
        }
        if((getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty)){
            num_available_directions++;
        }
        
    }
        // If there is more than one available direction, the character is at a fork
    if (num_available_directions > 1) {
      at_fork = true;
    }

    // If at_fork is true, the character has reached a fork in the path
    return at_fork;
}

void CoinSquare::doSomething()
{
    getWorld()->coinPlayerOverlap();
}

void DirSquare::doSomething()
{
    getWorld()->encounterDirSquare();
}


void StarSquare::doSomething()
{
    getWorld()->starPlayerOverlap();
}

//this is next
void BankSquare::doSomething()
{
    getWorld()->bankPlayerOverlap();
}

void EventSquare::doSomething()
{
    getWorld()->eventPlayerOverlap();
}

void Bowser::doSomething(){}
void Boo::doSomething(){}

