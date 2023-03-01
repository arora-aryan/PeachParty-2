#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

using std::endl, std::cerr;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


void Actor::setAlive(bool alive)
{
    m_alive = alive;
}

bool Actor::isValidSpot(const int x, const int y) const
{
    if(x<0||y<0||x>BOARD_WIDTH - 1||y>BOARD_HEIGHT - 1)
        return false;
    
    for(std::vector<Coords>::const_iterator it = m_empty_coords.begin(); it != m_empty_coords.end(); it++)
    {
        if(x == it->x && y == it->y)
        {
            return false;
        }

    }
    
    return true;
}

void Actor::getEmptySpots(std::vector<Coords> empty_coords)
{
    m_empty_coords = empty_coords;
}

void Player::doSomething()
{
    m_firesound = false;
    int next_x;
    int next_y;
    
    if(ticks_to_move > 0)
    {
        //lol i dont think i can use moveforward. prob use move in some direction or something, might add direction function to avatar class
        cerr<<"move: " << m_angle <<endl;
        moveAtAngle(m_angle, 2);
        ticks_to_move--;
    }
    else
    {
        waiting_to_roll = true;
    }
    if(m_action == ACTION_ROLL)
    {
        int die_roll = randInt(1,10);
        std::cerr<<die_roll<<std::endl;
        ticks_to_move = die_roll * 8;
        waiting_to_roll = false;
    }
    else if(m_action == ACTION_FIRE)
    {
        if(m_has_vortex == true)
        {
            getPositionInThisDirection(m_angle, SPRITE_WIDTH, next_x, next_y);
            Vortex *player_vortex = new Vortex(IID_VORTEX, next_x, next_y);
            m_firesound = true;
            player_vortex->moveTo(next_x, next_y);
            m_has_vortex = false;
            delete player_vortex;
        }
        //introduce vortex on square directly in front of avatar in walking direction
        //play sound_player_fire
        //update avatar so it doesnt have vortex anymore
    }
    if(waiting_to_roll == false)// && (ticks_to_move % 8) == 0)
    {
        if(((getX() % 16) == 0) && ((getY() % 16) == 0))
        {
            
            getPositionInThisDirection(m_angle, SPRITE_WIDTH, next_x, next_y);
            
            next_x/=BOARD_WIDTH;
            next_y/=BOARD_HEIGHT;
            
            cerr<<next_x<<endl;
            cerr<<start_x<<endl;
            
            cerr<<"IS THE SPOT "<< next_x << ' ' << next_y << " VALID?" <<endl;
            cerr<<"i'm at " << getX()/16 << ' ' << getY()/16 << endl;
            cerr<< isValidSpot(next_x, next_y) << endl;
            
            onDirSquare();
            cerr<<"aamama"<<isOnDir()<<endl;
            if(isOnDir() == true)
            {
                dirTF = false;
            }
            else if(!isValidSpot(next_x, next_y))
            {
                cerr<<"here: " << 20 <<endl;
                for(int i = 0; i <= 270; i+=90)
                {
                    //its super important to scale the x, y! 99% of my problems can be solved with that. always check for scale
                    getPositionInThisDirection(i, SPRITE_WIDTH, next_x, next_y);
                    next_x/=BOARD_WIDTH;
                    next_y/=BOARD_HEIGHT;
                    cerr<<"here: " << 21 <<endl;
                    
                    if(isValidSpot(next_x, next_y))
                    {
                        cerr<<"here: " << 22 << ' ' << m_angle << endl;
                        chooseDirection();
                        break;
                    }
                }
            }
            
        }
    }
    //if(waiting_to_roll == true)
    //{
        
    //}
    
}

void Actor::chooseDirection(){}

void Player::chooseDirection()
{
    int next_x;
    int next_y;

    getPositionInThisDirection(m_angle, SPRITE_WIDTH, next_x, next_y);

    
    next_x/=BOARD_WIDTH;
    next_y/=BOARD_HEIGHT;
        
    if(m_angle == right)
    {
        cerr<<"here: "<<1<<endl;
        m_angle = up;
        getPositionInThisDirection(up, SPRITE_WIDTH, next_x, next_y);
        next_x/=BOARD_WIDTH;
        next_y/=BOARD_HEIGHT;
        
        if(!isValidSpot(next_x, next_y))
        {
            cerr<<"here: "<<2<<endl;
            m_angle = down;
        }
    }
    
    else if(m_angle == left)
    {
        cerr<<"here: "<<3<<endl;
        m_angle = up;
        getPositionInThisDirection(up, SPRITE_WIDTH, next_x, next_y);
        setDirection(right);
        next_x/=BOARD_WIDTH;
        next_y/=BOARD_HEIGHT;
        
        if(!isValidSpot(next_x, next_y))
        {
            cerr<<"here: "<<4<<endl;
            m_angle = down;
        }
    }
    
    else if(m_angle == up)
    {
        cerr<<"here: "<<5<<endl;
        m_angle = right;
        getPositionInThisDirection(right, SPRITE_WIDTH, next_x, next_y);
        setDirection(right);
        next_x/=BOARD_WIDTH;
        next_y/=BOARD_HEIGHT;
        
        if(!isValidSpot(next_x, next_y))
        {
            cerr<<"here: "<<6<<endl;
            m_angle = left;
            setDirection(left);
        }
    }
    
    else if(m_angle == down)
    {
        cerr<<"here: "<<7<<endl;
        m_angle = right;
        getPositionInThisDirection(right, SPRITE_WIDTH, next_x, next_y);
        next_x/=BOARD_WIDTH;
        next_y/=BOARD_HEIGHT;
        
        if(!isValidSpot(next_x, next_y))
        {
            cerr<<"here: "<<8<<endl;
            m_angle = left;
            setDirection(left);

        }
    }
    else
        cerr<<"none?"<<endl;
}

void Player::getDirSquares(std::vector<Coords> r_coords, std::vector<Coords> l_coords, std::vector<Coords> d_coords, std::vector<Coords> u_coords)
{
    m_r_coords = r_coords;
    m_l_coords = l_coords;
    m_d_coords = d_coords;
    m_u_coords = u_coords;
}

void Player::onDirSquare()
{
    Coords curr_coords = {getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT};
    //int curr_y = getY()/SPRITE_HEIGHT;
    
    for(int i = 0; i < m_r_coords.size(); i++){
        if(curr_coords.x == m_r_coords[i].x && curr_coords.y == m_r_coords[i].y)
        {
            dirTF = true;
            setAngle(right);
        }
    }
    for(int i = 0; i < m_l_coords.size(); i++){
        if(curr_coords.x == m_l_coords[i].x && curr_coords.y == m_l_coords[i].y)
        {
            dirTF = true;
            setAngle(left);
        }
    }
    for(int i = 0; i < m_u_coords.size(); i++){
        if(curr_coords.x == m_u_coords[i].x && curr_coords.y == m_u_coords[i].y)
        {
            dirTF = true;
            setAngle(up);
        }
    }
    for(int i = 0; i < m_d_coords.size(); i++){
        if(curr_coords.x == m_d_coords[i].x && curr_coords.y == m_d_coords[i].y)
        {
            dirTF = true;
            cerr<<"im down"<<endl;
            setAngle(down);
            cerr<<"im down and angle is " <<m_angle<<endl;
        }
    }
}

void Baddie::doSomething(){}

void CoinSquare::doSomething()
{
    if(!isAlive())
        return;
}

//void RedCoinSquare::doSomething(){}

//void BlueCoinSquare::doSomething(){}

void DirSquare::doSomething(){}

void EventSquare::doSomething(){}

void StarSquare::doSomething(){}

void BankSquare::doSomething(){}

void Vortex::doSomething()
{
    
}
