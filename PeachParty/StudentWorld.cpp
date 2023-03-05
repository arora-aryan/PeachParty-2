#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_num_squares(0)
{}

int StudentWorld::init()
{
    Board bd;
    string boardNum = "board0" + to_string(getBoardNumber());
    string boardFile = assetPath() + boardNum + ".txt";
    Board::LoadResult result = bd.loadBoard(boardFile);
    
    if (result == Board::load_fail_file_not_found)
        cout << "Could not find board" + boardNum + " file" << endl;
    else if (result == Board::load_fail_bad_format)
        cout << "Board not loaded" << endl;
    else if (result == Board::load_success)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                m_board = bd;
                Board::GridEntry ge = bd.getContentsOf(i,j);
                switch (ge)
                {
                    case Board::empty:
                        break;
                    case Board::player:
                        m_peach = new Peach(this, i, j, 1);
                        m_yoshi = new Yoshi(this, i, j, 2);
                        m_actors.push_back(new BlueCoinSquare(this, i, j));
                        break;
                    case Board::blue_coin_square:
                        m_actors.push_back(new BlueCoinSquare(this, i, j));
                        break;
                    case Board::red_coin_square:
                        m_actors.push_back(new RedCoinSquare(this, i, j));
                        break;
                    case Board::left_dir_square:
                        m_actors.push_back(new DirSquare(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, left));
                        break;
                    case Board::right_dir_square:
                        m_actors.push_back(new DirSquare(this, i * SPRITE_WIDTH ,j * SPRITE_HEIGHT, right));
                        break;
                    case Board::up_dir_square:
                        m_actors.push_back(new DirSquare(this, i * SPRITE_WIDTH ,j * SPRITE_HEIGHT, up));
                        break;
                    case Board::down_dir_square:
                        m_actors.push_back(new DirSquare(this, i * SPRITE_WIDTH ,j * SPRITE_HEIGHT, down));
                        break;
                    case Board::event_square:
                        m_actors.push_back(new EventSquare(this, i ,j));
                        break;
                    case Board::bank_square:
                        m_actors.push_back(new BankSquare(this, i ,j));
                        break;
                    case Board::star_square:
                        m_actors.push_back(new StarSquare(this, i,j));
                        break;
                    case Board::bowser:
                        m_actors.push_back(new Bowser(this, i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                        m_actors.push_back(new BlueCoinSquare(this,i, j));
                        break;
                    case Board::boo:
                        m_actors.push_back(new Boo(this, i *SPRITE_WIDTH,j * SPRITE_HEIGHT));
                        m_actors.push_back(new BlueCoinSquare(this,i, j));
                        break;
                }
            }
        }
    }
    
    startCountdownTimer(5);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    //setGameStatText("Game will end in a few seconds");
    
    //if (timeRemaining() <= 0)
        //return GWSTATUS_NOT_IMPLEMENTED;
    
    m_peach->doSomething();
    m_yoshi->doSomething();
    
    vector<Actor*>::const_iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        (*it)->doSomething();
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::coinPlayerOverlap()
{
    
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY() / SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;
    
    if(m_peach->canActivate() && m_peach->isWaiting())
    {
        if(getBoard().getContentsOf(peach_x, peach_y) == Board::blue_coin_square){
            playSound(SOUND_GIVE_COIN);
            m_peach->updateCoinBalance(3);
            m_peach->setActivation(false);
        }
        if(getBoard().getContentsOf(peach_x, peach_y) == Board::red_coin_square && m_peach->numCoins() >= 3){
            playSound(SOUND_TAKE_COIN);
            m_peach->updateCoinBalance(-3);
            m_peach->setActivation(false);
        }
       

    }
    if(m_yoshi->canActivate() && m_yoshi->isWaiting())
    {
        if(getBoard().getContentsOf(yoshi_x, yoshi_y) == Board::blue_coin_square){
            playSound(SOUND_GIVE_COIN);
            m_yoshi->updateCoinBalance(3);
            m_yoshi->setActivation(false);
            
        }
        if(getBoard().getContentsOf(yoshi_x, yoshi_y) == Board::red_coin_square && m_yoshi->numCoins() >= 3){
            playSound(SOUND_TAKE_COIN);
            m_yoshi->updateCoinBalance(-3);
            m_yoshi->setActivation(false);
        }
    }
    
}

bool StudentWorld::isPlayerDirOverlap(int x, int y)
{
    Board::GridEntry ge = getBoard().getContentsOf(x,y);
    switch (ge)
    {
        case Board::left_dir_square:
            return true;
            break;
        case Board::right_dir_square:
            return true;
            break;
        case Board::up_dir_square:
            return true;
            break;
        case Board::down_dir_square:
            return true;
            break;
    }
    return false;
}

void StudentWorld::encounterDirSquare()
{
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY()/SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;
    
    if(m_peach->canActivate() && m_peach->getX() % 16 == 0 && m_peach->getY() % 16 == 0)
    {
        Board::GridEntry ge = getBoard().getContentsOf(peach_x,peach_y);
        switch (ge)
        {
            case Board::left_dir_square:
                m_peach->setWalkAngle(left);
                break;
            case Board::right_dir_square:
                m_peach->setWalkAngle(right);
                break;
            case Board::up_dir_square:
                m_peach->setWalkAngle(up);
                break;
            case Board::down_dir_square:
                m_peach->setWalkAngle(down);
                break;
        }
        
    }
    if(m_yoshi->canActivate() && m_yoshi->getX() % 16 == 0 && m_yoshi->getY() % 16 == 0)
    {
        
        Board::GridEntry ge = getBoard().getContentsOf(yoshi_x,yoshi_y);
        switch (ge)
        {
            case Board::left_dir_square:
                m_yoshi->setWalkAngle(left);
                break;
            case Board::right_dir_square:
                m_yoshi->setWalkAngle(right);
                break;
            case Board::up_dir_square:
                m_yoshi->setWalkAngle(up);
                break;
            case Board::down_dir_square:
                m_yoshi->setWalkAngle(down);
                break;
        }
    }
}

void StudentWorld::starPlayerOverlap()
{
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY()/SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;
    
    
    if(m_yoshi->canActivate() && m_yoshi->getX() % 16 == 0 && m_yoshi->getY() % 16 == 0 && m_yoshi->numCoins() >= 20)
    {
        
        Board::GridEntry ge = getBoard().getContentsOf(yoshi_x,yoshi_y);
        switch (ge)
        {
            case Board::star_square:
                playSound(SOUND_GIVE_STAR);
                m_yoshi->updateCoinBalance(-20);
                m_yoshi->addStar();
        }
    }
    if(m_peach->canActivate() && m_peach->getX() % 16 == 0 && m_peach->getY() % 16 == 0 && m_peach->numCoins() >= 20)
    {
        
        Board::GridEntry ge = getBoard().getContentsOf(peach_x,peach_y);
        switch (ge)
        {
            case Board::star_square:
                playSound(SOUND_GIVE_STAR);
                m_peach->updateCoinBalance(-20);
                m_peach->addStar();
        }
    }
}

void StudentWorld::bankPlayerOverlap()
{
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY()/SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;

    if(m_yoshi->canActivateBank() && m_yoshi->getX() % 16 == 0 && m_yoshi->getY() % 16 == 0)
    {
        Board::GridEntry ge = getBoard().getContentsOf(yoshi_x,yoshi_y);
        switch (ge)
        {
            case Board::bank_square:
                playSound(SOUND_GIVE_STAR);
                if(m_yoshi->hasTicks())
                {
                    if(m_yoshi->numCoins() < 5)
                    {
                        m_banktotal += m_yoshi->numCoins();
                        m_yoshi->updateCoinBalance(-(m_yoshi->numCoins()));
                        m_yoshi->setBankActivation(false);
                    }
                    else
                    {
                        m_banktotal += 5;
                        m_yoshi->updateCoinBalance(-5);
                        m_yoshi->setBankActivation(false);
                    }
                }
                else
                {
                    m_yoshi->updateCoinBalance(m_banktotal);
                    m_yoshi->setBankActivation(false);
                }
        }
    }
    
    if(m_peach->canActivateBank() && m_peach->getX() % 16 == 0 && m_peach->getY() % 16 == 0)
    {
        Board::GridEntry ge = getBoard().getContentsOf(peach_x,peach_y);
        switch (ge)
        {
            case Board::bank_square:
                playSound(SOUND_GIVE_STAR);
                if(m_peach->hasTicks())
                {
                    if(m_peach->numCoins() < 5)
                    {
                        m_banktotal += m_peach->numCoins();
                        m_peach->updateCoinBalance(-(m_peach->numCoins()));
                        m_peach->setBankActivation(false);
                    }
                    else
                    {
                        m_banktotal += 5;
                        m_peach->updateCoinBalance(-5);
                        m_peach->setBankActivation(false);
                    }
                }
                else
                {
                    m_peach->updateCoinBalance(m_banktotal);
                    m_peach->setBankActivation(false);
                }
        }
    }
}

void StudentWorld::eventPlayerOverlap()
{
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY()/SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;
    
    
    if(m_yoshi->canActivateEvent() && m_yoshi->getX() % 16 == 0 && m_yoshi->getY() % 16 == 0 && !m_yoshi->hasTicks())
    {
        Board::GridEntry ge = getBoard().getContentsOf(yoshi_x, yoshi_y);
        switch (ge)
        {
            case Board::event_square:
                switch (chooseRandomEvent(m_yoshi))
                {
                    case 1:
                        swapPlayers();
                        m_yoshi->setEventActivation(false);
                        break;
                    case 2:
                        randomTeleport(m_yoshi);
                        m_yoshi->setEventActivation(false);
                    case 3:
                        playSound(SOUND_GIVE_VORTEX);
                        m_yoshi->getVortex();
                        m_yoshi->setEventActivation(false);

                    default:
                        break;
                }
        }
    }
    if(m_peach->canActivateEvent() && m_peach->getX() % 16 == 0 && m_peach->getY() % 16 == 0 && !m_peach->hasTicks())
    {
        Board::GridEntry ge = getBoard().getContentsOf(peach_x, peach_y);
        switch (ge)
        {
            case Board::event_square:
                switch (chooseRandomEvent(m_peach))
                {
                    case 1:
                        swapPlayers();
                        m_peach->setEventActivation(false);
                        break;
                    case 2:
                        randomTeleport(m_peach);
                        m_peach->setEventActivation(false);
                    case 3:
                        playSound(SOUND_GIVE_VORTEX);
                        m_peach->getVortex();
                        m_peach->setEventActivation(false);
                    default:
                        break;
                }
        }
    }
}

int StudentWorld::chooseRandomEvent(Player* m_player)
{
    if(m_player->hasVortex()){
        return randInt(1, 2);
    }
    else{
        return randInt(1, 3);
    }
}

void StudentWorld::swapPlayers()
{
    int yoshi_new_x = m_peach->getX();
    int yoshi_new_y = m_peach->getY();
    int yoshi_new_ticks = m_peach->getTicks();
    int yoshi_new_angle = m_peach->getWalkAngle();
    int yoshi_new_facing_direction = m_peach->getDirection();
    
    playSound(SOUND_PLAYER_TELEPORT);
    
    m_peach->setWalkAngle(m_yoshi->getWalkAngle());
    m_yoshi->setWalkAngle(yoshi_new_angle);

    m_peach->setDirection(m_yoshi->getDirection());
    m_yoshi->setDirection(yoshi_new_facing_direction);
    
    m_peach->moveTo(m_yoshi->getX(), m_yoshi->getY());
    m_yoshi->moveTo(yoshi_new_x, yoshi_new_y);

    m_peach->setTicks(m_yoshi->getTicks());
    m_yoshi->setTicks(yoshi_new_ticks);
}

void StudentWorld::randomTeleport(Player *m_player)
{
    int x;
    int y;
    bool getResult = false;
    
    while(!getResult)
    {
        x = randInt(0, 16);
        y = randInt(0, 16);
        
        Board::GridEntry ge = getBoard().getContentsOf(x,y);
        if(ge != Board::empty)
            getResult = true;
    }
    playSound(SOUND_PLAYER_TELEPORT);
    m_player->moveTo(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    delete m_yoshi;
    for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        delete *it;
    }
}

StudentWorld::~StudentWorld()
{}


