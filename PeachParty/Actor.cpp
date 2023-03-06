#include "Actor.h"
#include "Board.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
    cerr<<"do i got a vortex: " << hasVortex() << endl;
    //cerr<<"i have: " << numCoins() << "coins" << endl;
    //cerr<<"i have " << numStars() << "stars" << endl;
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
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
                    if(vortex == true)
                    {
                        createVortex();
                        vortex = false;
                    }
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
                    if(!up_open || getWalkAngle() == down)
                    {
                        break;
                    }
                    setWalkAngle(up);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
                case ACTION_RIGHT:
                    if(!right_open || getWalkAngle() == left)
                    {
                        break;
                    }
                    setWalkAngle(right);
                    waiting_to_move = false;
                    hasMoved = true;
                    break;
                case ACTION_LEFT:
                    if(!left_open || getWalkAngle() == right)
                    {
                        break;
                    }
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
        allowAllActivation();
        if((getX() % 16 == 0) && (getY() % 16 == 0))
        {
            deadEnd();
        }
        
        if (getWalkAngle() == left)
            setDirection(left);
        else
            setDirection(right);
        
        moveAtAngle(getWalkAngle(), 2);
        ticks_to_move--;
        cerr<<"i got "<< ticks_to_move << "ticks" << endl;
        if (ticks_to_move == 0)
            waiting_to_roll = true;
    }
}

void Actor::deadEnd()
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

void Player::createVortex()
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
    switch (getWalkAngle())
    {
        case right:
            getWorld()->setVortex(board_x + 1, board_y, right);
            break;
        case left:
            getWorld()->setVortex(board_x - 1, board_y, left);
            break;
        case up:
            getWorld()->setVortex(board_x, board_y + 1, up);
            break;
        case down:
            getWorld()->setVortex(board_x, board_y - 1, down);
            break;
        default:
            break;
    }
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

void Baddie::doSomething()
{
    if(m_paused)
    {
        doSomethingPaused();
        m_pause_counter--;
        if(m_pause_counter == 0)
        {
            squares_to_move = pickSquaresToMove();
            ticks_to_move = squares_to_move * 8;
            m_paused = false;
        }
    }
    if(!m_paused)
    {
        if((getX() % 16 == 0) && (getY() % 16 == 0))
        {
            deadEnd();
        }
        if (getWalkAngle() == left)
            setDirection(left);
        else
            setDirection(right);
        
        moveAtAngle(getWalkAngle(), 2);
        ticks_to_move--;
        if(ticks_to_move == 0)
        {
            m_paused = true;
            m_pause_counter = 180;
            specialTrick();
        }
    }
    if(beenHit())
    {
        setHit(false);
        hitResult();
    }
}

void Baddie::hitResult()
{
    getWorld()->randomTeleport(this);
    setWalkAngle(right);
    setDirection(0);
    setPaused(true);
    setTicks(180);
}

void Bowser::specialTrick()
{
    getWorld()->setDroppingSquare(getX()/SPRITE_WIDTH, getX()/SPRITE_HEIGHT);
}

void Bowser::doSomethingPaused()
{
    getWorld()->bowserPlayerPaused(this);
}

void Boo::doSomethingPaused()
{
    getWorld()->booPlayerPaused(this);
}

int Boo::pickSquaresToMove()
{
    return randInt(1, 3);
}

int Bowser::pickSquaresToMove()
{
    return randInt(1, 10);
}

void Vortex::doSomething()
{
    getWorld()->fireVortex(this);
}
