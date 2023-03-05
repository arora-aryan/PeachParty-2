#include "Actor.h"
#include "Board.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
    if (alive == false)
        return;
    
    cerr<<waiting_to_move<<endl;
    
    */
    if(atFork() == true && getX() % 16 == 0 && getY() % 16 == 0)
    {
        
        cerr<<"at fork"<<endl;

        int temp_ticks = ticks_to_move;
        ticks_to_move = 0;
        cerr<<"down"<<down<<endl;
        
        int dir = 0;
        while(dir==0)
        {
            dir = getWorld()->getAction(m_playernumber);
        }
        cerr<<"direction"<<dir<<endl;
        if(chooseDirection(dir))
        {
            ticks_to_move = temp_ticks;
        }
    }
    */
    if (waiting_to_roll == true && ((getX() % 16 == 0) && (getY() % 16 == 0)))
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

    if (waiting_to_roll == false && ticks_to_move >= 0)
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

//
bool Player::chooseDirection(int dir)
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
    
    bool rightOpen = (getWorld()->getBoard().getContentsOf(board_x+1, board_y) != Board::empty);
    bool leftOpen = (getWorld()->getBoard().getContentsOf(board_x-1, board_y) != Board::empty);
    bool upOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y+1) != Board::empty);
    bool downOpen = (getWorld()->getBoard().getContentsOf(board_x, board_y-1) != Board::empty);
    cerr<<"mydir"<<dir<<endl;
    
    if(dir == down)
    {
        cerr<<"hi"<<endl;
        setWalkAngle(down);
        moveAtAngle(getWalkAngle(), 2);
    }
    /*
    switch(dir)
    {
        default:
            return false;
        
        case ACTION_RIGHT:
            if(getWalkAngle() == left || !rightOpen)
            {
                break;
            }
            setWalkAngle(right);
            return true;
            break;
        
        case ACTION_LEFT:
            if(getWalkAngle() == right || !leftOpen)
            {
                break;
            }
            setWalkAngle(left);
            return true;
            break;
        case ACTION_UP:
            
            if(getWalkAngle() == down || !upOpen)
            {
                break;
            }
            setWalkAngle(up);
            return true;
            break;
             
        case ACTION_DOWN:
            
            if(getWalkAngle() == up || !downOpen)
            {
                break;
            }
            setWalkAngle(down);
            return true;
            break;
             
    }
     */
    
    return false;
    
}

void Player::deadEnd()
{
    int board_x = getX()/SPRITE_WIDTH;
    int board_y = getY()/SPRITE_HEIGHT;
        
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

//this is next
void BankSquare::doSomething()
{
    //getWorld()->bankPlayerOverlap();
}

void EventSquare::doSomething(){}

void Bowser::doSomething(){}
void Boo::doSomething(){}

