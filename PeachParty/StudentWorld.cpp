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
: GameWorld(assetPath)
{
    m_peach = nullptr;
    m_yoshi = nullptr;
}

int StudentWorld::init()
{
    Board bd;
    string boardNum = "board0" + to_string(getBoardNumber());
    string boardFile = assetPath() + boardNum + ".txt";
    Board::LoadResult result = bd.loadBoard(boardFile);
    
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board" + boardNum + " file \n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Board not loaded\n";
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
                        m_coinsquares.push_back(new BlueCoinSquare(this, i, j));
                        break;
                    case Board::blue_coin_square:
                        m_actors.push_back(new BlueCoinSquare(this, i, j));
                        m_coinsquares.push_back(new BlueCoinSquare(this, i, j));

                        break;
                    case Board::red_coin_square:
                        m_actors.push_back(new RedCoinSquare(this, i, j));
                        m_coinsquares.push_back(new RedCoinSquare(this, i, j));
                        break;
                    case Board::left_dir_square:
                       // m_actors.push_back(new DirSquare(this, i, j));
                        break;
                    case Board::right_dir_square:
                        break;
                    case Board::up_dir_square:
                        break;
                    case Board::down_dir_square:
                        break;
                    case Board::event_square:
                        break;
                    case Board::bank_square:
                        break;
                    case Board::star_square:
                        break;
                    case Board::bowser:
                        break;
                    case Board::boo:
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
    
    //cerr<<"hello here"<<endl;
    int peach_x = m_peach->getX()/SPRITE_WIDTH;
    int peach_y = m_peach->getY() / SPRITE_HEIGHT;
    int yoshi_x = m_yoshi->getX()/SPRITE_WIDTH;
    int yoshi_y = m_yoshi->getY()/SPRITE_HEIGHT;
    
/* kuda
    cerr<<getBoard().getContentsOf(peach_x, peach_y)<<endl;
    cerr<<getBoard().getContentsOf(yoshi_x, yoshi_y)<<endl;
    cerr<<Board::blue_coin_square<<endl;
    cerr<<Board::red_coin_square<<endl;
 */
   // cerr<<"peach: "<<peach_x<< ' ' << peach_y<<endl;
    //cerr<<"yoshi: "<< yoshi_x<< ' ' << yoshi_y<<endl;
    
    if(m_peach->canActivate() && m_peach->isWaiting())
    {
        cerr<<1<<endl;
        if(getBoard().getContentsOf(peach_x, peach_y) == Board::blue_coin_square){
            // cerr<<1<<endl;
            m_peach->updateCoinBalance(3);
            m_peach->setActivation(false);
        }
        if(getBoard().getContentsOf(peach_x, peach_y) == Board::red_coin_square){
            // cerr<<2<<endl;
            m_peach->updateCoinBalance(-3);
            m_peach->setActivation(false);
        }
       

    }
    if(m_yoshi->canActivate() && m_yoshi->isWaiting())
    {
        cerr<<2<<endl;
        if(getBoard().getContentsOf(yoshi_x, yoshi_y) == Board::blue_coin_square){
            // cerr<<3<<endl;
            m_yoshi->updateCoinBalance(3);
            m_yoshi->setActivation(false);
            
        }
        if(getBoard().getContentsOf(yoshi_x, yoshi_y) == Board::red_coin_square){
            // cerr<<4<<endl;
            m_yoshi->updateCoinBalance(-3);
            m_yoshi->setActivation(false);
        }
    }
    
}



















void StudentWorld::cleanUp()
{
    if (m_peach != nullptr) {
            delete m_peach;
            m_peach = nullptr;
        }
    if (m_yoshi != nullptr) {
            delete m_yoshi;
            m_yoshi = nullptr;
        }
        
   // for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    //{
      //  delete *it;
    //}
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


