#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    Board bd;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    
    if(result == Board::load_fail_file_not_found || result == Board::load_fail_bad_format)
        return GWSTATUS_BOARD_ERROR;
        
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
        Board::GridEntry ge = bd.getContentsOf(i, j); // x=5, y=10
            switch (ge) {
                case Board::empty:
                    cout << "Location " << i <<' '<< j << " is empty\n"<<endl;
                    m_empty.x = i;
                    m_empty.y = j;
                    m_empty_coords.push_back(m_empty);
                    break;
                    
                case Board::boo:
                    cout << "Location " << i <<' '<< j << " has a Boo and a blue coin square\n"<<endl;
                    m_boo = new Baddie(IID_BOO, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_boo);
                    break;
                    
                case Board::bowser:
                    cout << "Location " << i <<' '<< j << " has a Bowser and a blue coin square\n"<<endl;
                    m_bowser = new Baddie(IID_BOWSER, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_bowser);
                    break;
                    
                case Board::player:
                    cout << "Location " << i <<' '<< j << " has Peach & Yoshi and a blue coin square\n"<<endl;
                    m_blueCoin = new BlueCoinSquare(IID_BLUE_COIN_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_peach = new Player(IID_PEACH, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_yoshi = new Player(IID_YOSHI, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_blueCoin);
                    break;
                    
                case Board::red_coin_square:
                    cout << "Location " << i <<' '<< j << " has a red coin square\n"<<endl;
                    m_redCoin = new RedCoinSquare(IID_RED_COIN_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_redCoin);
                    break;
                    
                case Board::blue_coin_square:
                    cout << "Location " << i <<' '<< j << " has a blue coin square\n"<<endl;
                    m_blueCoin = new BlueCoinSquare(IID_BLUE_COIN_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_blueCoin);
                    break;
                    
                case Board::up_dir_square:
                    cout << "Location " << i <<' '<< j << " has an up direction square\n"<<endl;
                    m_upDir = new DirSquare(IID_DIR_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_upDir->setDirection(90);
                    m_actors.push_back(m_upDir);
                    m_up.x = i;
                    m_up.y = j;
                    m_up_coords.push_back(m_up);
                    break;
                    
                case Board::down_dir_square:
                    cout << "Location " << i <<' '<< j << " has a down direction square\n"<<endl;
                    m_downDir = new DirSquare(IID_DIR_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_downDir->setDirection(270);
                    m_actors.push_back(m_downDir);
                    m_down.x = i;
                    m_down.y = j;
                    m_down_coords.push_back(m_down);
                    break;
                    
                case Board::left_dir_square:
                    cout << "Location " << i <<' '<< j << " has a left direction square\n"<<endl;
                    m_leftDir = new DirSquare(IID_DIR_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_leftDir->setDirection(180);
                    m_actors.push_back(m_leftDir);
                    m_left.x = i;
                    m_left.y = j;
                    m_left_coords.push_back(m_left);
                    break;
                    
                case Board::right_dir_square:
                    cout << "Location " << i <<' '<< j << " has a right direction square\n"<<endl;
                    m_rightDir = new DirSquare(IID_DIR_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_rightDir);
                    m_right.x = i + 1;
                    m_right.y = j;
                    m_right_coords.push_back(m_right);
                    break;
                    
                case Board::event_square:
                    cout << "Location " << i <<' '<< j << " has an event square\n"<<endl;
                    m_eventSquare = new EventSquare(IID_EVENT_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_eventSquare);
                    break;
                    
                case Board::bank_square:
                    cout << "Location " << i <<' '<< j << " has a bank square\n"<<endl;
                    m_bankSquare = new BankSquare(IID_BANK_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_bankSquare);
                    break;
                    
                case Board::star_square:
                    cout << "Location " << i <<' '<< j << " has a star square\n"<<endl;
                    m_starSquare = new StarSquare(IID_STAR_SQUARE, i*BOARD_HEIGHT, j*BOARD_WIDTH);
                    m_actors.push_back(m_starSquare);
                    break;
                    
            }
        }
    }
    m_peach->getEmptySpots(m_empty_coords);
    m_yoshi->getEmptySpots(m_empty_coords);
    m_peach->getDirSquares(m_right_coords, m_left_coords, m_down_coords, m_up_coords);
    m_yoshi->getDirSquares(m_right_coords, m_left_coords, m_down_coords, m_up_coords);

    //cerr<<m_yoshi->onDirSquare()<<endl;
    
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

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


int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    
    
    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        //display score of winner w/ something like if yoshi win display yoshi stuff
        setFinalScore(0, 0);
        //if peach win or yoshi win
        
        //whoever is winner return winner
        return GWSTATUS_PEACH_WON;
        
        //if tie return random winner
    }
    
    //update game status
    setGameStatText("P1 Roll: 3 Stars: 2 $$: 15 | Time: 75 | Bank: 9 | P2 Roll: 0 Stars: 1 $$: 22 VOR");
    
    m_peach->setActionNum(getAction(1));
    m_yoshi->setActionNum(getAction(2));
        
    m_peach->doSomething();
    m_yoshi->doSomething();
    if(m_peach->fireSound() == true || m_yoshi->fireSound() == true)
        playSound(SOUND_PLAYER_FIRE);
    
    for(std::vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        (*it)->doSomething();
        
        
        //delete actors that needa be deleted
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    delete m_yoshi;
    
    for(std::vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        delete *it;
    }
    return;
}
