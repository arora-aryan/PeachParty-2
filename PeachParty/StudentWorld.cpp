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
                        m_actors.push_back(new Boo(this, i *SPRITE_WIDTH, j * SPRITE_HEIGHT));
                        m_actors.push_back(new BlueCoinSquare(this,i, j));
                        break;
                }
            }
        }
    }
    
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    //setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() < 0)
        return GWSTATUS_NOT_IMPLEMENTED;
    
    if(timeRemaining() == 0)
        return winner();
        //return GWSTATUS_PEACH_WON

    determineText();
    setGameStatText(m_displayText);
    
    m_peach->doSomething();
    m_yoshi->doSomething();
    
    
    for(vector<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end();)
    {
        if((*it)->toBeDeleted())
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
        {
            (*it)->doSomething();
            it++;
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::winner()
{
    if(m_peach->numStars() == m_yoshi->numStars())
    {
        if(m_peach->numCoins() > m_yoshi->numCoins())
        {
            setFinalScore(m_peach->numStars(), m_peach->numCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if(m_peach->numCoins() < m_yoshi->numCoins())
        {
            setFinalScore(m_yoshi->numStars(), m_yoshi->numCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }
    else if(m_peach->numStars() > m_yoshi->numStars())
    {
        setFinalScore(m_peach->numStars(), m_peach->numCoins());
        return GWSTATUS_PEACH_WON;
    }
    else if(m_peach->numStars() < m_yoshi->numStars())
    {
        setFinalScore(m_yoshi->numStars(), m_yoshi->numCoins());
        return GWSTATUS_YOSHI_WON;
    }
    else
    {
        switch (randInt(1, 2)) {
            case 1:
                setFinalScore(m_peach->numStars(), m_peach->numCoins());
                return GWSTATUS_PEACH_WON;
                break;
            case 2:
                setFinalScore(m_yoshi->numStars(), m_yoshi->numCoins());
                return GWSTATUS_YOSHI_WON;
                break;
            default:
                break;
        }
    }
    setFinalScore(m_peach->numStars(), m_peach->numCoins());
    return GWSTATUS_PEACH_WON;
}

void StudentWorld::determineText()
{
    std::string peach_text;
    std::string yoshi_text;
    int tr = timeRemaining();
    int peachticks = 0;
    int yoshticks = 0;

    if(m_peach->hasTicks())
    {
        peachticks = (m_peach->getTicks()/8) + 1;
    }
    if(m_yoshi->hasTicks())
    {
        yoshticks = (m_yoshi->getTicks()/8) + 1;
    }
    
    peach_text = "P1 Roll: " + to_string(peachticks) + " Stars: " + to_string(m_peach->numStars()) + " $$: " + to_string(m_peach->numCoins()) + " |";
    if(m_peach->hasVortex())
        peach_text = "P1 Roll: " + to_string(peachticks) + " Stars: " + to_string(m_peach->numStars()) + " $$: " + to_string(m_peach->numCoins()) + " VOR |";
 
    yoshi_text = " P2 Roll: " + to_string(yoshticks) + " Stars: " + to_string(m_yoshi->numStars()) + " $$: " + to_string(m_yoshi->numCoins());
    if(m_yoshi->hasVortex())
        yoshi_text = " P2 Roll: " + to_string(yoshticks) + " Stars: " + to_string(m_yoshi->numStars()) + " $$: " + to_string(m_yoshi->numCoins()) + " VOR";

    m_displayText = peach_text + " Time: " + to_string(tr) + " | " + " Bank: " + to_string(m_banktotal) + " | " + yoshi_text;
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
                if(m_yoshi->hasTicks())
                {
                    playSound(SOUND_WITHDRAW_BANK);
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
                    playSound(SOUND_DEPOSIT_BANK);
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
                playSound(SOUND_WITHDRAW_BANK);
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
                    playSound(SOUND_DEPOSIT_BANK);
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

void StudentWorld::randomTeleport(Actor *m_actor)
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
    m_actor->moveTo(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
}

void StudentWorld::bowserPlayerPaused(Bowser *m_bowser)
{
    int randomevent = randInt(1, 2);
    
    if(m_yoshi->canActivateBowser() && !m_yoshi->hasTicks() && m_bowser->getX() == m_yoshi->getX() && m_bowser->getY() == m_yoshi->getY())
    {
        if(randomevent == 1)
        {
            m_yoshi->updateCoinBalance(-(m_yoshi->numCoins()));
            m_yoshi->removeStars();
            playSound(SOUND_BOWSER_ACTIVATE);
        }
        m_yoshi->setBowserActivation(false);
    }
    
    if(m_peach->canActivateBowser() && !m_peach->hasTicks() && m_bowser->getX() == m_peach->getX() && m_bowser->getY() == m_peach->getY())
    {
        if(randomevent == 1)
        {
            m_peach->updateCoinBalance(-(m_peach->numCoins()));
            m_peach->removeStars();
            playSound(SOUND_BOWSER_ACTIVATE);
        }
        m_peach->setBowserActivation(false);
    }

}

void StudentWorld::booPlayerPaused(Boo *m_boo)
{
    int randomevent = randInt(1, 2);
    
    if(m_yoshi->canActivateBoo() && !m_yoshi->hasTicks() && m_boo->getX() == m_yoshi->getX() && m_boo->getY() == m_yoshi->getY())
    {
        switch (randomevent)
        {
            case 1:
                swapCoins();
                m_yoshi->setBooActivation(false);
                break;
            case 2:
                swapStars();
                m_yoshi->setBooActivation(false);
                break;
            default:
                break;
        }
    }
    
    if(m_peach->canActivateBoo() && !m_peach->hasTicks() && m_boo->getX() == m_peach->getX() && m_boo->getY() == m_peach->getY())
    {
        switch (randomevent)
        {
            case 1:
                swapCoins();
                m_peach->setBooActivation(false);
                break;
            case 2:
                swapStars();
                m_peach->setBooActivation(false);
                break;
            default:
                break;
        }
    }
}

void StudentWorld::swapStars()
{
    int yoshi_new_stars = m_peach->numStars();
    m_peach->setStarBalance(m_yoshi->numStars());
    m_yoshi->setStarBalance(yoshi_new_stars);
}

void StudentWorld::swapCoins()
{
    int yoshi_new_coins = m_peach->numCoins();
    m_peach->setCoinBalance(m_yoshi->numCoins());
    m_yoshi->setCoinBalance(yoshi_new_coins);
}

void StudentWorld::setDroppingSquare(int x, int y)
{
    /*
    for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getX()/SPRITE_WIDTH == x && (*it)->getY()/SPRITE_WIDTH == x && (*it)->canBeExchanged() && (*it)->toBeDeleted() == false)
        {
            cerr<<"times died"<< endl;
            //m_actors.push_back(new DroppingSquare(this, x, y));
            //(*it)->die();
        }
    }
     */
}

void StudentWorld::setVortex(int x, int y, int start_direction)
{
    m_actors.push_back(new Vortex(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, start_direction));
}

void StudentWorld::fireVortex(Vortex *m_vortex)
{
    if(m_vortex->getX() / SPRITE_WIDTH < 0 || m_vortex->getY() / SPRITE_HEIGHT < 0 || m_vortex->getX() / SPRITE_WIDTH > BOARD_WIDTH || m_vortex->getY() / SPRITE_HEIGHT > BOARD_HEIGHT)
    {
        m_vortex->die();
        return;
    }
        
    for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->impactable())
        {
            if((*it)->getX() / SPRITE_WIDTH == m_vortex->getX() / SPRITE_WIDTH && (*it)->getY() / SPRITE_HEIGHT == m_vortex->getY() / SPRITE_HEIGHT)
            {
                playSound(SOUND_HIT_BY_VORTEX);
                (*it)->setHit(true);
                m_vortex->die();
            }
        }
    }
    m_vortex->moveAtAngle(m_vortex->getWalkAngle(), 2);
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


