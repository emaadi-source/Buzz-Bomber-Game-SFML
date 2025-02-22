#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream> 

using namespace std;
using namespace sf;

// Constants
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;      // 30
const int gameColumns = resolutionY / boxPixelsY;   // 20

// Enum for game states
enum GameState { MENU, ENTER_NAME, PLAYING, VIEW_HIGH_SCORES, EXIT, LEVEL_SELECT, GAME_OVER, GAME_COMPLETED, LEVEL_TRANSITION };
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(RenderWindow& window, float player_x, float player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float bullet_x, float bullet_y, Sprite& bulletSprite);
void drawSprayCansOnGround(RenderWindow& window, Sprite& spraySprite, int totalSprayCans, int sprayLevels[], int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns, Font& font);
void drawSprayCounts(RenderWindow& window, Font& font, int sprayLevels[], int totalSprayCans, int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns);
void useSpray(int sprayLevels[], int& currentSprayIndex, int& bulletsFired, int sprayReductionThreshold, int totalSprayCans, int& sprayCansLeft);
bool areAllSprayCansEmpty(int sprayLevels[], int totalSprayCans);
void drawMenu(RenderWindow& window, Text menuOptions[], int menuSize, int selectedIndex);
void drawSubMenu(RenderWindow& window, Text subMenuOptions[], int subMenuSize, int selectedIndex);
void spawnBees(float beePositionsX[], float beePositionsY[], float beeDirections[], bool isFastBee[], int& beesSpawned, Clock& spawnClock, int totalBees, int fastBees, bool beeActive[], bool beesExiting[], int& fastBeesSpawned);
bool checkAndCreateHiveForBee(int i,
                              float beePositionsX[], float beePositionsY[], float beeDirections[],
                              bool beeActive[], bool beesExiting[],
                              const bool isHoneycomb[], const float honeycombX[], const float honeycombY[],
                              int beesSpawned,
                              const float preHoneycombX[], const float preHoneycombY[], const bool preHoneycombActive[], int totalPreHoneycombs,
                              bool beehiveActive[], float beehiveX[], float beehiveY[],
                              int maxBeehives, int& beehivesCreated);
void moveBees(float beePositionsX[], float beePositionsY[], float honeycombX[], float honeycombY[], float beeDirections[], bool isFastBee[], bool isRedHoneycombActive[],
             int beesSpawned, bool beeActive[], bool beePaused[], float beePauseTime[], Clock& beeMoveClock, int resolutionX,
             int boxPixelsX, int boxPixelsY, int gameColumns, int& firstBeeReachedBottom, bool flowers[], int& leftFlowerIndex,
             int& rightFlowerIndex, bool& firstBeeTriggered, bool& placeLeftFlowerNext, float player_y, bool& levelNeedsReset,
             bool beesExiting[], bool isHoneycomb[], float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs,
             bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated);
void drawBees(RenderWindow& window, float beePositionsX[], float beePositionsY[], Sprite beeSprites[], Sprite fastBeeSprite, bool isFastBee[], bool beeActive[], int beesSpawned);
void handleBulletBeeCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float beePositionsX[], float beePositionsY[], bool beeActive[], bool isHoneycomb[], bool isFastBee[], bool isRedHoneycombActive[], int totalBees, float honeycombX[], float honeycombY[], int& beesDestroyed, bool flowers[], int& leftFlowerIndex, int& rightFlowerIndex, int gameRows, int& score);
void handleBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float honeycombX[], float honeycombY[], bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score);
void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], Sprite& honeycombSprite, Sprite& redHoneycombSprite, bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees);
void setupLevel(int currentLevel, int& totalBees, int& fastBees, int& totalHoneycombs, Texture& backgroundTexture,
               Music& bgMusic, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[],
               int maxPreHoneycombs, int& totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[],
               int& beehivesCreated, int maxBeehives);
void moveHummingbird(float& hummingbird_x, float& hummingbird_y, float& hummingbird_directionX, float& hummingbird_directionY, bool& hummingbird_paused, float& hummingbird_pauseTime, Clock& hummingbirdMoveClock, float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY);
void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y, bool hummingbird_visible);
void handleHummingbirdHoneycombCollision(float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, float hummingbird_x, float hummingbird_y, int& score, int boxPixelsX, int boxPixelsY, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool isRedHoneycombActive[]);
void handleBulletHummingbirdCollision(float bullet_x, float bullet_y, bool& bullet_exists, float hummingbird_x, float hummingbird_y, int& hummingbird_health, bool& hummingbird_visible, Clock& hummingbirdRespawnClock, Sprite& hummingbirdSprite, int boxPixelsX, int boxPixelsY);
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[], int gameRows, int boxPixelsX, int boxPixelsY, int resolutionY);
void checkPlayerRestriction(float& player_x, bool flowers[], int gameRows, int boxPixelsX, int& sprayCansLeft, int totalSprayCans, int sprayLevels[], int& currentSprayIndex, int maxSpraysPerCan, GameState& currentState);
void initBeeHives(bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated);
void drawBeehives(RenderWindow& window, Sprite& beehiveSprite,
                  const bool beehiveActive[], const float beehiveX[], const float beehiveY[],
                  int beehivesCreated) ;
void handleBeehiveCompletion(bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score);
void displayLevelTransition(RenderWindow& window, Font& font, int currentLevel, Clock& transitionClock, bool& transitionComplete);

// High-Score Handling Functions
void loadHighScores(string filename, string names[], int scores[]);
void saveHighScores(const string& filename, const std::string names[], const int scores[]);
bool addNewScore(string filename, string playerName, int newScore);
void displayHighScoresOnScreen(RenderWindow& window, Font& font, string names[], int scores[]);
void showHighScores(RenderWindow& window, Font& font);

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Initialize game state and related variables
    GameState currentState = MENU;
    int selectedMenuIndex = 0;
    int selectedLevelIndex = 0;
    int currentLevel = 1;
    int score = 0; // Initialize score
    const int maxBees = 30;
    const int maxBeehives = 10;
    const int maxPreHoneycombs = 50;

    const int maxSpraysPerCan = 56;
    const int initialSprayCans = 3;
    const int maxSprayCans = 10; 
    int sprayLevels[maxSprayCans] = { maxSpraysPerCan, maxSpraysPerCan, maxSpraysPerCan, 0, 0, 0, 0, 0, 0, 0 };
    int currentSprayIndex = 0;
    int bulletsFired = 0;
    const int sprayReductionThreshold = 8;
    int sprayCansLeft = initialSprayCans;
    int totalSprayCans = initialSprayCans; 

    
    const int maxMilestones = 5;
    int milestones[maxMilestones] = {20000, 40000, 80000, 160000, 320000};
    int currentMilestoneIndex = 0;

   
    int beesSpawned = 0;
    int totalBees = 0;
    int fastBees = 0;
    int fastBeesSpawned = 0;
    int totalHoneycombs = 0;
    int beesDestroyed = 0;
    int firstBeeReachedBottom = 0;
    bool firstBeeTriggered = false;
    bool placeLeftFlowerNext = true;
    bool levelNeedsReset = false;

    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;

    float hummingbird_x = resolutionX / 2.0f;
    float hummingbird_y = resolutionY / 2.0f;
    float hummingbird_directionX = 0.0f;
    float hummingbird_directionY = 0.0f;
    bool hummingbird_paused = false;
    float hummingbird_pauseTime = 0.0f;
    int hummingbird_health = 3;
    bool hummingbird_visible = true;

    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    float beePositionsX[maxBees];
    float beePositionsY[maxBees];
    float beeDirections[maxBees];
    bool beeActive[maxBees] = { false };
    bool beePaused[maxBees] = { false };
    float beePauseTime[maxBees] = { 0.0f };
   bool beesExiting[maxBees] = { false };
    bool isHoneycomb[maxBees] = { false };
   bool isFastBee[maxBees] = { false };
    bool isRedHoneycombActive[maxBees] = { false };
    float honeycombX[maxBees], honeycombY[maxBees];

    for (int i = 0; i < maxBees; i++) {
        beePositionsX[i] = 0.0f;
      beePositionsY[i] = 0.0f;
        beeDirections[i] = 1.0f;
       honeycombX[i] = 0.0f;
        honeycombY[i] = 0.0f;
    }

    bool beehiveActive[maxBeehives]={true};
   float beehiveX[maxBeehives];
    float beehiveY[maxBeehives];
    int beehivesCreated = 0;

    bool flowers[gameRows];
    for (int i = 0; i < gameRows; i++) {
        flowers[i] = false;
    }
    int leftFlowerIndex = -1;
    int rightFlowerIndex = gameRows;

  float preHoneycombX[maxPreHoneycombs];
  float preHoneycombY[maxPreHoneycombs];
    bool preHoneycombActive[maxPreHoneycombs] = { false };
    int totalPreHoneycombs = 0;
    bool transitionComplete = false;

    // Load Textures
    Texture beeTexture, fastBeeTexture, honeycombTexture, redHoneycombTexture, backgroundTexture;
    Texture hummingbirdTexture, beehiveTexture, flowerTexture, playerTexture, bulletTexture;
 if (!beeTexture.loadFromFile("Textures/bee.png")) {
        cout << "Failed to load bee.png" << std::endl;
    }
    if (!fastBeeTexture.loadFromFile("Textures/fast_bee.png")) {
       cout << "Failed to load fast_bee.png" << std::endl;
    }
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
        cout << "Failed to load honeycomb.png" << std::endl;
    }
    if (!redHoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
      cout << "Failed to load honeycomb_red.png" << std::endl;
    }
    if (!hummingbirdTexture.loadFromFile("Textures/hummingbird.png")) {
       cout << "Failed to load hummingbird.png" << std::endl;
    }
    if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
       cout << "Failed to load hive.png" << std::endl;
    }
    if (!flowerTexture.loadFromFile("Textures/flower.png")) {
       cout << "Failed to load flower.png" << std::endl;
   }
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
     cout << "Failed to load spray.png" << std::endl;
    }
    if (!bulletTexture.loadFromFile("Textures/bullet.png")) {
        cout << "Failed to load bullet.png" << std::endl;
    }

    //  Sprites
    Sprite flowerSprite(flowerTexture);
   flowerSprite.setScale(2.0f, 2.0f);

    Sprite honeycombSprite(honeycombTexture);
    Sprite redHoneycombSprite(redHoneycombTexture);

   Sprite beeSprites[maxBees];
    for (int i = 0; i < maxBees; i++) {
        beeSprites[i].setTexture(beeTexture);
        beeSprites[i].setScale(1.0f, 1.0f);
    }
    Sprite fastBeeSprite(fastBeeTexture);
    fastBeeSprite.setScale(1.0f, 1.0f);

    Sprite beehiveSprite(beehiveTexture);
    Sprite playerSpriteObj(playerTexture);
    playerSpriteObj.setScale(1.0f, 1.0f);

    Sprite bulletSpriteObj(bulletTexture);
    bulletSpriteObj.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    Sprite hummingbirdSprite(hummingbirdTexture);
    hummingbirdSprite.setScale(2.0f, 2.0f);
    hummingbirdSprite.setTextureRect(IntRect(0, 0, 32, 32));

    RectangleShape groundRectangle(Vector2f(resolutionX, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);

    sf::Sprite spraySprite(playerTexture);
    spraySprite.setScale(1, 1);

    sf::Sprite backgroundSprite;

    // Sounds
    SoundBuffer bulletFireBuffer;
    if (!bulletFireBuffer.loadFromFile("Sound Effects/fire.wav")) {
        cout << "Failed to load fire.wav" << endl;
    }
  Sound bulletFireSound;
    bulletFireSound.setBuffer(bulletFireBuffer);

    Music bgMusic;

    // Font
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        std::cout << "Failed to load arial.ttf" << endl;
    }

    //  Menu 
    const int menuSize = 4; 
    Text menuOptions[menuSize];
    const char* menuTexts[menuSize] = { "Start Game", "Select Level", "View High Scores", "Exit" };
    for (int i = 0; i < menuSize; i++) {
        menuOptions[i].setFont(font);
        menuOptions[i].setString(menuTexts[i]);
        menuOptions[i].setCharacterSize(50);
        menuOptions[i].setPosition(350, 200 + i * 80);
        menuOptions[i].setFillColor(sf::Color::White);
    }

    const int subMenuSize = 4;
    sf::Text subMenuOptions[subMenuSize];
    const char* subMenuTexts[subMenuSize] = { "Level 1", "Level 2", "Level 3","Boss Level" };
    for (int i = 0; i < subMenuSize; i++) {
        subMenuOptions[i].setFont(font);
        subMenuOptions[i].setString(subMenuTexts[i]);
        subMenuOptions[i].setCharacterSize(50);
        subMenuOptions[i].setPosition(350, 200 + i * 80);
        subMenuOptions[i].setFillColor(sf::Color::White);
    }

    //  window
    RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Buzz Bombers", sf::Style::Close);
    window.setPosition(sf::Vector2i(500, 200));

 
    Clock spawnClock, beeMoveClock, bulletClock, hummingbirdMoveClock, hummingbirdRespawnClock, hummingbirdAnimationClock, transitionClock;

 
    initBeeHives(beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated);

    {
        ifstream finCheck("highscores.txt");
        if (!finCheck.is_open()) {
           
            string defaultNames[10];
            int defaultScores[10];
            for (int i = 0; i < 10; i++) {
                defaultNames[i] = "AAA";
                defaultScores[i] = 0;
            }
            ofstream fout("highscores.txt"); 
            if (!fout.is_open()) {
                cout << "Failed to create highscores.txt.\n";
            }
            for (int i = 0; i < 10; i++) {
                fout << defaultNames[i] << " " << defaultScores[i] << "\n";
            }
            fout.close();
        }
        finCheck.close();
    }

    string playerName = "";

   
    string inputName = "";
    bool isEnteringName =false;
    Text nameInputText;
   nameInputText.setFont(font);
    nameInputText.setCharacterSize(40);
    nameInputText.setFillColor(Color::White);
  nameInputText.setPosition(300.0f, 300.0f);
    nameInputText.setString("Enter your name: ");

    
    bool highScoreAdded = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

                     if (event.type == sf::Event::TextEntered && currentState == ENTER_NAME) {
                if (event.text.unicode == '\b') { // Handle backspace
                    if (!inputName.empty()) {
                        inputName.pop_back();
                    }
                }
                else if (event.text.unicode == '\r' || event.text.unicode == '\n') { 
                    if (!inputName.empty()) {
                        playerName = inputName;
                        inputName = "";
                     currentState = PLAYING;
                        // Setup the level
                        setupLevel(currentLevel, totalBees, fastBees, totalHoneycombs, backgroundTexture, bgMusic,
                                   preHoneycombX, preHoneycombY, preHoneycombActive,
                                   maxPreHoneycombs, totalPreHoneycombs, beehiveActive,  beehiveX,  beehiveY,  beehivesCreated,maxBeehives);
                        backgroundSprite.setTexture(backgroundTexture);
                        // Reset game 
                        beesSpawned = 0;
                        fastBeesSpawned = 0;
                        for (int i = 0; i < maxBees; i++) {
                            beeActive[i] = false;
                           isHoneycomb[i] =false;
                            isFastBee[i] = false;
                            isRedHoneycombActive[i] =false;
                            beesExiting[i] = false;
                            beePaused[i] = false;
                            beePauseTime[i] = 0.0f;
                        }

                        for (int i = 0; i < totalSprayCans; i++) {
                            sprayLevels[i] = maxSpraysPerCan;
                        }
                        sprayCansLeft = totalSprayCans;
                        currentSprayIndex = 0;
                      bulletsFired = 0;
                        bullet_exists = false;

                        hummingbird_x = resolutionX / 2.0f;
                        hummingbird_y = resolutionY / 2.0f;
                        hummingbird_directionX = 0.0f;
                      hummingbird_directionY = 0.0f;
                        hummingbird_paused = false;
                        hummingbird_pauseTime = 0.0f;
                      hummingbird_health = 3;
                        hummingbird_visible = true;
                        hummingbirdSprite.setColor(sf::Color::White);

                        for (int i = 0; i < maxBeehives; i++) {
                            beehiveActive[i] = false;
                            beehiveX[i] = 0.0f;
                           beehiveY[i] = 0.0f;
                        }
                        beehivesCreated = 0;

                        for (int i = 0; i < gameRows; i++) {
                            flowers[i] = false;
                        }
                        leftFlowerIndex = -1;
                        rightFlowerIndex = gameRows;
                      beesDestroyed = 0;
                        firstBeeReachedBottom = 0;
                        firstBeeTriggered = false;
                        placeLeftFlowerNext = true;
                        levelNeedsReset = false;

                        player_x = (gameRows / 2) * boxPixelsX;
                        player_y = (gameColumns - 4) * boxPixelsY;
                    }
                }
                else if (event.text.unicode < 128 && inputName.size() < 20) { 
                    inputName += static_cast<char>(event.text.unicode);
                }
            }

           
            if (event.type == sf::Event::KeyPressed) {
                if (currentState == MENU) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedMenuIndex = (selectedMenuIndex - 1 + menuSize) % menuSize;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedMenuIndex = (selectedMenuIndex + 1) % menuSize;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedMenuIndex == 0) {
                            
                            currentState = ENTER_NAME;
                            inputName = "";
                            highScoreAdded = false; 
                        }
                        else if (selectedMenuIndex == 1) {
                            
                            currentState = LEVEL_SELECT;
                        }
                        else if (selectedMenuIndex == 2) {
                            
                            currentState = VIEW_HIGH_SCORES;
                        }
                        else if (selectedMenuIndex == 3) {
                            
                            window.close();
                        }
                    }
                }
                else if (currentState == LEVEL_SELECT) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedLevelIndex = (selectedLevelIndex - 1 + subMenuSize) % subMenuSize;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedLevelIndex = (selectedLevelIndex + 1) % subMenuSize;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        currentLevel = selectedLevelIndex + 1;
                        currentState = MENU;
                        highScoreAdded = false; // Reset the flag
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        currentState = MENU;
                    }
                }
                else if (currentState == PLAYING) {
                    int playerColumn = static_cast<int>(player_x / boxPixelsX);
                    if (event.key.code == sf::Keyboard::Left) {
                        if (playerColumn > 0 && !flowers[playerColumn - 1]) {
                            player_x -= boxPixelsX;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        if (playerColumn < gameRows - 1 && !flowers[playerColumn + 1]) {
                            player_x += boxPixelsX;
                        }
                    }

                    if (event.key.code == sf::Keyboard::Space && !bullet_exists) {
                        if (!areAllSprayCansEmpty(sprayLevels, totalSprayCans)) {
                            bullet_x = player_x;
                            bullet_y = player_y;
                            bullet_exists = true;
                            bulletFireSound.play();
                            useSpray(sprayLevels, currentSprayIndex, bulletsFired, sprayReductionThreshold, totalSprayCans, sprayCansLeft);
                        }
                        else {
                            currentState = GAME_OVER;
                            
                            bool qualifies = false;
                           string names[10];
                            int scores[10];
                            loadHighScores("highscores.txt", names, scores);
                            if (score > scores[9]) { 
                                qualifies = true;
                            }
                            if (qualifies) {
                              
                                bool added = addNewScore("highscores.txt", playerName, score);
                                if (added) {
                                    std::cout << "Congratulations! Your score has been added to the High Scores.\n";
                                    highScoreAdded = true;
                                }
                            }
                        }
                    }
                }
                else if (currentState == GAME_OVER || currentState == GAME_COMPLETED) {
                    if (event.key.code == sf::Keyboard::Enter) {
                     
                        currentState = MENU;
                        score = 0;
                        currentLevel = 1; 
                        highScoreAdded = false; 
                    }
                }
                else if (currentState == VIEW_HIGH_SCORES) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentState = MENU;
                    }
                }
                else if (currentState == LEVEL_TRANSITION) {
                    // No input handled
                }
            }
        }

        window.clear();

        if (currentState == MENU) {
          
            window.draw(backgroundSprite);

          
            drawMenu(window, menuOptions, menuSize, selectedMenuIndex);
        }
        else if (currentState == ENTER_NAME) {
          
            window.draw(backgroundSprite);

          
            sf::Text prompt;
            prompt.setFont(font);
            prompt.setString("Enter your name:");
            prompt.setCharacterSize(40);
          prompt.setFillColor(sf::Color::White);
            prompt.setPosition(300.0f, 250.0f);
            window.draw(prompt);

            // Display current input
            sf::Text inputDisplay;
            inputDisplay.setFont(font);
            inputDisplay.setString(inputName);
            inputDisplay.setCharacterSize(40);
            inputDisplay.setFillColor(sf::Color::Cyan);
            inputDisplay.setPosition(300.0f, 300.0f);
            window.draw(inputDisplay);
        }
        else if (currentState == LEVEL_SELECT) {
            // background
            window.draw(backgroundSprite);

            // submenu
            drawSubMenu(window, subMenuOptions, subMenuSize, selectedLevelIndex);
        }
        else if (currentState == VIEW_HIGH_SCORES) {
            // Draw background
            window.draw(backgroundSprite);

            // Display high scores
            std::string names[10];
          int scores[10];
            loadHighScores("highscores.txt", names, scores);
            displayHighScoresOnScreen(window, font, names, scores);
        }
        else if (currentState == PLAYING) {
            window.draw(backgroundSprite);

            // pre-generated honeycombs
            for (int i = 0; i < totalPreHoneycombs; i++) {
                if (preHoneycombActive[i]) {
                honeycombSprite.setPosition(preHoneycombX[i], preHoneycombY[i]);
                    window.draw(honeycombSprite);
                }
            }
             for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
            beehiveSprite.setPosition(beehiveX[i], beehiveY[i]);
            window.draw(beehiveSprite);
        }
    }
 
           
            drawBeehives(window,  beehiveSprite,
                  beehiveActive,  beehiveX, beehiveY,
                   beehivesCreated) ;

           
            if (bullet_exists) {
                drawBullet(window, bullet_x, bullet_y, bulletSpriteObj);
                moveBullet(bullet_y, bullet_exists, bulletClock);
                handleBulletBeeCollision(bullet_x, bullet_y, bullet_exists, beePositionsX, beePositionsY, beeActive, isHoneycomb, isFastBee, isRedHoneycombActive, totalBees, honeycombX, honeycombY, beesDestroyed, flowers, leftFlowerIndex, rightFlowerIndex, gameRows, score);
                handleBulletHoneycombCollision(bullet_x, bullet_y, bullet_exists, honeycombX, honeycombY, isHoneycomb, isRedHoneycombActive, totalBees, preHoneycombX, preHoneycombY, preHoneycombActive, maxPreHoneycombs, beehiveActive, beehiveX, beehiveY, beehivesCreated, score);
                handleBulletHummingbirdCollision(bullet_x, bullet_y, bullet_exists, hummingbird_x, hummingbird_y, hummingbird_health, hummingbird_visible, hummingbirdRespawnClock, hummingbirdSprite, boxPixelsX, boxPixelsY);
            }

         
            spawnBees(beePositionsX, beePositionsY, beeDirections, isFastBee, beesSpawned, spawnClock, totalBees, fastBees, beeActive, beesExiting, fastBeesSpawned);

           
            moveBees(
                beePositionsX, beePositionsY, honeycombX, honeycombY, beeDirections, isFastBee, isRedHoneycombActive,
                beesSpawned, beeActive, beePaused, beePauseTime,
                beeMoveClock, resolutionX, boxPixelsX, boxPixelsY, gameColumns,
                firstBeeReachedBottom, flowers, leftFlowerIndex, rightFlowerIndex,
                firstBeeTriggered, placeLeftFlowerNext, player_y, levelNeedsReset,
                beesExiting, isHoneycomb, preHoneycombX, preHoneycombY, preHoneycombActive, totalPreHoneycombs,
                beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated
            );

           
            if (hummingbird_visible) {
                moveHummingbird(hummingbird_x, hummingbird_y, hummingbird_directionX, hummingbird_directionY,
                              hummingbird_paused, hummingbird_pauseTime, hummingbirdMoveClock,
                                honeycombX, honeycombY, isHoneycomb, beesSpawned, resolutionX, resolutionY, boxPixelsX, boxPixelsY);
            }

            // hummingbird
            if (hummingbird_visible && hummingbirdAnimationClock.getElapsedTime().asMilliseconds() > 200) {
                sf::IntRect currentRect = hummingbirdSprite.getTextureRect();
              if (currentRect.left == 0) {
                    hummingbirdSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
                }
                else {
                    hummingbirdSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                }
                hummingbirdAnimationClock.restart();
            }

           
            handleHummingbirdHoneycombCollision(honeycombX, honeycombY, isHoneycomb, beesSpawned, hummingbird_x, hummingbird_y, score, boxPixelsX, boxPixelsY, preHoneycombX, preHoneycombY, preHoneycombActive, totalPreHoneycombs, isRedHoneycombActive);

            window.draw(groundRectangle);

          
            drawSprayCansOnGround(window, spraySprite, totalSprayCans, sprayLevels, currentSprayIndex, boxPixelsX, boxPixelsY, gameColumns, font);
            drawSprayCounts(window, font, sprayLevels, totalSprayCans, currentSprayIndex, boxPixelsX, boxPixelsY, gameColumns);
            drawPlayer(window, player_x, player_y, playerSpriteObj);
            drawBees(window, beePositionsX, beePositionsY, beeSprites, fastBeeSprite, isFastBee, beeActive, beesSpawned);
            drawHoneycombs(window, honeycombX, honeycombY, honeycombSprite, redHoneycombSprite, isHoneycomb, isRedHoneycombActive, beesSpawned);
            drawFlowers(window, flowerSprite, flowers, gameRows, boxPixelsX, boxPixelsY, resolutionY);
            drawHummingbird(window, hummingbirdSprite, hummingbird_x, hummingbird_y, hummingbird_visible);

            sf::Text scoreText;
            scoreText.setFont(font);
           scoreText.setString("Score: " + std::to_string(score));
          scoreText.setCharacterSize(50);
            scoreText.setFillColor(sf::Color::White);
          scoreText.setPosition(640, 570);
            window.draw(scoreText);

           
            if (!hummingbird_visible && hummingbirdRespawnClock.getElapsedTime().asSeconds() >= 5.0f) {
                hummingbird_visible = true;
                hummingbird_health = 3;
                hummingbirdSprite.setColor(sf::Color::White);
                hummingbird_x = resolutionX / 2.0f;
                hummingbird_y = resolutionY / 2.0f;
            }

          
            checkPlayerRestriction(player_x, flowers, gameRows, boxPixelsX, sprayCansLeft, totalSprayCans, sprayLevels, currentSprayIndex, maxSpraysPerCan, currentState);

           
            if (levelNeedsReset) {
              
                currentState = LEVEL_TRANSITION;
                transitionClock.restart();

              
                for (int i = 0; i < gameRows; i++) {
                   flowers[i] = false;
                }
                leftFlowerIndex = -1;
                rightFlowerIndex = gameRows;

             
                player_x = (gameRows / 2) * boxPixelsX;
                player_y = (gameColumns - 4) * boxPixelsY;

                for (int i = 0; i < maxBees; i++) {
                    beeActive[i] = false;
                  beesExiting[i] = false;
                   isHoneycomb[i] = false;
                    isFastBee[i] = false;
                    isRedHoneycombActive[i] = false;
                }

                firstBeeTriggered = false;
                placeLeftFlowerNext = true;
                  beesSpawned = 0;
                levelNeedsReset = false;
            }

            
            if (beesSpawned >= totalBees) {
                bool allGone = true;
                for (int i = 0; i < beesSpawned; i++) {
                    if (beeActive[i]) {
                        allGone = false;
                        break;
                    }
                }
                if (allGone) {
                   
                    handleBeehiveCompletion(beehiveActive, beehiveX, beehiveY, beehivesCreated, score);
                    
                  
                    if (currentMilestoneIndex < maxMilestones && score >= milestones[currentMilestoneIndex]) {
                        if (totalSprayCans < maxSprayCans) {
                          sprayLevels[totalSprayCans] = maxSpraysPerCan;
                            totalSprayCans++;
                              sprayCansLeft++;
                            currentMilestoneIndex++;
                        }
                    }

                       if (currentLevel < 3) {
                        currentLevel++;
                       
                        currentState = LEVEL_TRANSITION;
                        transitionClock.restart();
                    }
                    else {
                       
                        currentState = GAME_COMPLETED;
                        bgMusic.stop();

                        
                        if (!highScoreAdded) { 
                            bool qualifies = false;
                            string names[10];
                              int scoresHigh[10];
                            loadHighScores("highscores.txt", names, scoresHigh);
                              if (score > scoresHigh[9]) { // Correct condition
                                qualifies = true;
                            }
                            if (qualifies) {
                                // Add to high scores
                                bool added = addNewScore("highscores.txt", playerName, score);
                                if (added) {
                                    cout << "Congratulations! Your score has been added to the High Scores.\n";
                                    highScoreAdded = true;
                             }
                            }
                          }
                    }
                }
            }

        }
        else if (currentState == LEVEL_TRANSITION) {
            
            displayLevelTransition(window, font, currentLevel, transitionClock, transitionComplete);
            if (transitionClock.getElapsedTime().asSeconds() > 0.5f) { 
                // Setup the new level
                setupLevel(currentLevel, totalBees, fastBees, totalHoneycombs, backgroundTexture, bgMusic,
                           preHoneycombX, preHoneycombY, preHoneycombActive,
                           maxPreHoneycombs, totalPreHoneycombs, beehiveActive, beehiveX, beehiveY,
                           beehivesCreated, maxBeehives);
                backgroundSprite.setTexture(backgroundTexture);
                currentState = PLAYING;

                // Reset game variables
                beesSpawned = 0;
                fastBeesSpawned = 0;
                for (int i = 0; i < maxBees; i++) {
                    beeActive[i] = false;
                    isHoneycomb[i] = false;
                    isFastBee[i] = false;
                    isRedHoneycombActive[i] = false;
                    beesExiting[i] = false;
                    beePaused[i] = false;
                    beePauseTime[i] = 0.0f;
                }

                hummingbird_x = resolutionX / 2.0f;
                hummingbird_y = resolutionY / 2.0f;
                hummingbird_directionX = 0.0f;
                hummingbird_directionY = 0.0f;
                hummingbird_paused = false;
                hummingbird_pauseTime = 0.0f;
                hummingbird_health = 3;
                hummingbird_visible = true;
                hummingbirdSprite.setColor(sf::Color::White);

                for (int i = 0; i < maxBeehives; i++) {
                    beehiveActive[i] = false;
                    beehiveX[i] = 0.0f;
                    beehiveY[i] = 0.0f;
                }
                beehivesCreated = 0;

                for (int i = 0; i < gameRows; i++) {
                    flowers[i] = false;
                }
                leftFlowerIndex = -1;
                rightFlowerIndex = gameRows;
                beesDestroyed = 0;
                firstBeeReachedBottom = 0;
                firstBeeTriggered = false;
                placeLeftFlowerNext = true;
                levelNeedsReset = false;

                player_x = (gameRows / 2) * boxPixelsX;
                player_y = (gameColumns - 4) * boxPixelsY;
            }
        }
        else if (currentState == GAME_OVER) {
            // Display Game Over screen
            
            sf::RectangleShape overlay(sf::Vector2f(resolutionX, resolutionY));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);

            // Game Over Text
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!\nPress Enter to return to Menu");
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(resolutionX / 2.0f - 300.0f, resolutionY / 2.0f - 100.0f);
            window.draw(gameOverText);

            // final score
            sf::Text finalScoreText;
            finalScoreText.setFont(font);
            finalScoreText.setString("Final Score: " + std::to_string(score));
            finalScoreText.setCharacterSize(40);
            finalScoreText.setFillColor(sf::Color::White);
            finalScoreText.setPosition(resolutionX / 2.0f - 150.0f, resolutionY / 2.0f);
            window.draw(finalScoreText);

            //  high score addition message
            if (highScoreAdded) {
                sf::Text highScoreText;
                highScoreText.setFont(font);
                highScoreText.setString("New High Score Added!");
                highScoreText.setCharacterSize(30);
                highScoreText.setFillColor(sf::Color::Green);
                highScoreText.setPosition(resolutionX / 2.0f - 150.0f, resolutionY / 2.0f + 50.0f);
                window.draw(highScoreText);
            }
        }
        else if (currentState == GAME_COMPLETED) {
               sf::RectangleShape overlay(sf::Vector2f(resolutionX, resolutionY));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);

            // Game Completed Text
            sf::Text gameCompletedText;
            gameCompletedText.setFont(font);
            gameCompletedText.setString("Congratulations!\nYou've completed all levels!\nPress Enter to return to Menu");
            gameCompletedText.setCharacterSize(50);
            gameCompletedText.setFillColor(sf::Color::Green);
            gameCompletedText.setPosition(resolutionX / 2.0f - 350.0f, resolutionY / 2.0f - 150.0f);
            window.draw(gameCompletedText);

            // Display final score
            sf::Text finalScoreText;
            finalScoreText.setFont(font);
            finalScoreText.setString("Final Score: " + std::to_string(score));
            finalScoreText.setCharacterSize(40);
            finalScoreText.setFillColor(sf::Color::White);
            finalScoreText.setPosition(resolutionX / 2.0f - 150.0f, resolutionY / 2.0f);
            window.draw(finalScoreText);

           
            if (highScoreAdded) {
                sf::Text highScoreText;
                highScoreText.setFont(font);
                highScoreText.setString("New High Score Added!");
                highScoreText.setCharacterSize(30);
                highScoreText.setFillColor(sf::Color::Green);
                highScoreText.setPosition(resolutionX / 2.0f - 150.0f, resolutionY / 2.0f + 50.0f);
                window.draw(highScoreText);
            }
        }

        window.display();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


void drawPlayer(RenderWindow& window, float player_x, float player_y, Sprite& playerSprite) {
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
}


void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() > 10) {
      bullet_y -= 30.0f;
        bulletClock.restart();
    }
    if (bullet_y < 0) {
        bullet_exists = false;
    }
}


void drawBullet(RenderWindow& window, float bullet_x, float bullet_y, Sprite& bulletSprite) {
    bulletSprite.setScale(2, 2);
  bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}


void drawSprayCansOnGround(RenderWindow& window, Sprite& spraySprite, int totalSprayCans, int sprayLevels[], int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns, Font& font) {
    for (int i = 0; i < totalSprayCans; i++) {
        spraySprite.setPosition(static_cast<float>(i) * (boxPixelsX + 10), static_cast<float>(gameColumns - 2) * boxPixelsY);
      if (sprayLevels[i] <= 0) {
            spraySprite.setColor(Color::Red);
        }
       else {
            spraySprite.setColor(Color::White);
        }
        window.draw(spraySprite);

        
        if (i == currentSprayIndex) {
            RectangleShape outline(Vector2f(static_cast<float>(boxPixelsX) * spraySprite.getScale().x, static_cast<float>(boxPixelsY) * spraySprite.getScale().y));
            outline.setPosition(static_cast<float>(i) * (boxPixelsX + 10), static_cast<float>(gameColumns - 2) * boxPixelsY);
           outline.setFillColor(Color::Transparent);
          outline.setOutlineThickness(2.0f);
            outline.setOutlineColor(Color::Yellow);
            window.draw(outline);
        }
    }
}


void drawSprayCounts(RenderWindow& window, Font& font, int sprayLevels[], int totalSprayCans, int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns) {
    for (int i = 0; i < totalSprayCans; i++) {
       
        Text sprayCountText;
        sprayCountText.setFont(font);
        sprayCountText.setString(to_string(sprayLevels[i]));
      sprayCountText.setCharacterSize(20);
     sprayCountText.setFillColor(Color::White);
        sprayCountText.setPosition(static_cast<float>(i) * (boxPixelsX + 10) + 10, static_cast<float>(gameColumns - 2) * boxPixelsY - 30);
        window.draw(sprayCountText);
    }
}


void useSpray(int sprayLevels[], int& currentSprayIndex, int& bulletsFired, int sprayReductionThreshold, int totalSprayCans, int& sprayCansLeft) {
    if (sprayLevels[currentSprayIndex] > 0) {
        sprayLevels[currentSprayIndex]--;
        bulletsFired++;
        if (sprayLevels[currentSprayIndex] == 0) {
            sprayCansLeft--;
          currentSprayIndex++;
            if (currentSprayIndex >= totalSprayCans) {
                currentSprayIndex = 0;
         }
        }
   }
}


bool areAllSprayCansEmpty(int sprayLevels[], int totalSprayCans) {
    for (int i = 0; i < totalSprayCans; i++) {
        if (sprayLevels[i] > 0) return false;
    }
    return true;
}


void drawMenu(RenderWindow& window, Text menuOptions[], int menuSize, int selectedIndex) {
    for (int i = 0; i < menuSize; i++) {
        menuOptions[i].setFillColor(i == selectedIndex ? Color::Yellow : Color::White);
        window.draw(menuOptions[i]);
    }
}

// submenu
void drawSubMenu(RenderWindow& window, Text subMenuOptions[], int subMenuSize, int selectedIndex) {
    for (int i = 0; i < subMenuSize; i++) {
        subMenuOptions[i].setFillColor(i == selectedIndex ? Color::Yellow : Color::White);
        window.draw(subMenuOptions[i]);
    }
}


void spawnBees(float beePositionsX[], float beePositionsY[], float beeDirections[], bool isFastBee[], int& beesSpawned, Clock& spawnClock, int totalBees, int fastBees, bool beeActive[], bool beesExiting[], int& fastBeesSpawned) {
    const float spawnInterval = 2.0f;
    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval && beesSpawned < totalBees) {
        beePositionsX[beesSpawned] = 0.0f;
        beePositionsY[beesSpawned] = 0.0f;
        beeDirections[beesSpawned] = 1.0f;
        beeActive[beesSpawned] = true;
        beesExiting[beesSpawned] = false;

        if (fastBeesSpawned < fastBees) {
            isFastBee[beesSpawned] = true;
            fastBeesSpawned++;
        }
        else {
            isFastBee[beesSpawned] = false;
        }

        beesSpawned++;
        spawnClock.restart();
    }
}


bool checkAndCreateHiveForBee(int i,
                              float beePositionsX[], float beePositionsY[], float beeDirections[],
                              bool beeActive[], bool beesExiting[],
                              const bool isHoneycomb[], const float honeycombX[], const float honeycombY[],
                              int beesSpawned,
                              const float preHoneycombX[], const float preHoneycombY[], const bool preHoneycombActive[], int totalPreHoneycombs,
                              bool beehiveActive[], float beehiveX[], float beehiveY[],
                              int maxBeehives, int& beehivesCreated) {
    if (!beeActive[i] || beesExiting[i]) return false;

    float beeX = beePositionsX[i];
    float beeY = beePositionsY[i];
      float beeWidth = static_cast<float>(boxPixelsX);
      float beeHeight = static_cast<float>(boxPixelsY);

    bool leftHoneycomb = false;
    bool rightHoneycomb = false;

    // Check honeycombs from bees
    for (int j = 0; j < beesSpawned; j++) {
        if (isHoneycomb[j]) {
            // Check left side honeycomb
            if (beeX - beeWidth > honeycombX[j] && (beeX - beeWidth) < (honeycombX[j] + boxPixelsX) &&
                beeY + beeHeight > honeycombY[j] && beeY < honeycombY[j] + boxPixelsY) {
                leftHoneycomb = true;
            }

            //  right side honeycomb
            if ((beeX + beeWidth) > honeycombX[j] && (beeX + beeWidth) < (honeycombX[j] + boxPixelsX) &&
                beeY + beeHeight > honeycombY[j] && beeY < honeycombY[j] + boxPixelsY) {
                rightHoneycomb = true;
            }
        }
    }

    // pre-generated honeycombs
    for (int h = 0; h < totalPreHoneycombs; h++) {
        if (!preHoneycombActive[h]) continue;

        //  left side pre-honeycomb
        if (beeX - beeWidth > preHoneycombX[h] && (beeX - beeWidth) < (preHoneycombX[h] + boxPixelsX) &&
            beeY + beeHeight > preHoneycombY[h] && beeY < preHoneycombY[h] + boxPixelsY) {
            leftHoneycomb = true;
        }

        // right side pre-honeycomb
        if ((beeX + beeWidth) > preHoneycombX[h] && (beeX + beeWidth) < (preHoneycombX[h] + boxPixelsX) &&
            beeY + beeHeight > preHoneycombY[h] && beeY < preHoneycombY[h] + boxPixelsY) {
            rightHoneycomb = true;
        }
    }

    // honeycombs on both sides
    if (leftHoneycomb && rightHoneycomb && beehivesCreated < maxBeehives) {
        beeActive[i] = false;
        for (int k = 0; k < maxBeehives; k++) {
            if (!beehiveActive[k]) {
                beehiveActive[k] = true;
              beehiveX[k] = beeX;
                beehiveY[k] = beeY;
                beehivesCreated++;
                      return true;
            }
        }
    }

    return false;
}

void moveBees(float beePositionsX[], float beePositionsY[], float honeycombX[], float honeycombY[], float beeDirections[], bool isFastBee[], bool isRedHoneycombActive[],
             int beesSpawned, bool beeActive[], bool beePaused[], float beePauseTime[], Clock& beeMoveClock, int resolutionX,
             int boxPixelsX, int boxPixelsY, int gameColumns, int& firstBeeReachedBottom, bool flowers[], int& leftFlowerIndex,
             int& rightFlowerIndex, bool& firstBeeTriggered, bool& placeLeftFlowerNext, float player_y, bool& levelNeedsReset,
             bool beesExiting[], bool isHoneycomb[], float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs,
             bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated) {

    const float moveInterval = 50.0f; 
    const float beeSpeed = 10.0f;
    const float beeSpeedFast = 20.0f;
    const float tierHeight = 30.0f;
    int beeWidth = boxPixelsX;
    int beeHeight = boxPixelsY;

    float tierAbovePlayer = player_y - boxPixelsY;

    if (beeMoveClock.getElapsedTime().asMilliseconds() > moveInterval) {
        float deltaTime = beeMoveClock.getElapsedTime().asSeconds();
        for (int i = 0; i < beesSpawned; i++) {
            if (!beeActive[i]) continue;

            if (beePaused[i]) {
                beePauseTime[i] -= deltaTime;
                if (beePauseTime[i] <= 0.0f) {
                    beePaused[i] = false;
                    beePauseTime[i] = 0.0f;
                }
                if (beePaused[i]) continue;
            }
            else {
                if ((rand() % 1000) < 5) { 
                    beePaused[i] = true;
                    beePauseTime[i] = 1.0f; 
                    continue;
                }
            }

            float currentSpeed = isFastBee[i] ? beeSpeedFast : beeSpeed;
            beePositionsX[i] += beeDirections[i] * currentSpeed;

            // check honeycomb collisions
            if (!beesExiting[i]) {
                if (!isFastBee[i]) {
                    for (int h = 0; h < totalPreHoneycombs; h++) {
                        if (!preHoneycombActive[h]) continue;
                        bool collidesX = (beePositionsX[i] + beeWidth > preHoneycombX[h]) && (beePositionsX[i] < preHoneycombX[h] + boxPixelsX);
                        bool collidesY = (beePositionsY[i] + beeHeight >preHoneycombY[h]) && (beePositionsY[i] < preHoneycombY[h] + boxPixelsY);
                        if (collidesX && collidesY) {
                            beeDirections[i] *= -1;
                            beePositionsY[i] += tierHeight;
                        }
                    }

                    // Check bee honeycomb collision
                    for (int j = 0; j < beesSpawned; j++) {
                        if (!isHoneycomb[j]) continue;
                        bool collidesX = (beePositionsX[i] + beeWidth > honeycombX[j]) && (beePositionsX[i] < (honeycombX[j] + boxPixelsX));
                        bool collidesY = (beePositionsY[i] + beeHeight > honeycombY[j]) && (beePositionsY[i] < (honeycombY[j] + boxPixelsY));
                        if (collidesX && collidesY) {
                            beeDirections[i] *= -1;
                            beePositionsY[i] += tierHeight;
                            break;
                        }
                    }
                }
                else {
                    // Fast bees skip 
                      for (int j = 0; j < beesSpawned; j++) {
                        if (!isHoneycomb[j]) continue;
                        bool collidesX = (beePositionsX[i] + beeWidth > honeycombX[j]) && (beePositionsX[i] < honeycombX[j] + boxPixelsX);
                        bool collidesY = (beePositionsY[i] + beeHeight > honeycombY[j]) && (beePositionsY[i] < honeycombY[j] + boxPixelsY);
                        if (collidesX && collidesY) {
                            if (!isRedHoneycombActive[j]) {
                                beeDirections[i] *= -1;
                                beePositionsY[i] += tierHeight;
                            }
                            break;
                        }
                    }
                }
            }

          
            if (!beesExiting[i]) {
                if (beePositionsX[i] <= 0 || beePositionsX[i] >= resolutionX - beeWidth) {
                    beeDirections[i] *= -1;
                    beePositionsY[i] += tierHeight;
                }
            }

           
            if (beePositionsY[i] >= tierAbovePlayer && !beesExiting[i]) {

                if (!firstBeeTriggered) {
                    firstBeeTriggered = true;
                    firstBeeReachedBottom = 1;

                    leftFlowerIndex++;
                    rightFlowerIndex--;

                    if (leftFlowerIndex >= 0 && leftFlowerIndex < gameRows) flowers[leftFlowerIndex] = true;
                    if (rightFlowerIndex >= 0 && rightFlowerIndex < gameRows) flowers[rightFlowerIndex] = true;
                }
                else {
                    if (placeLeftFlowerNext) {
                        leftFlowerIndex++;
                        if (leftFlowerIndex >= 0 && leftFlowerIndex < gameRows) flowers[leftFlowerIndex] = true;
                        placeLeftFlowerNext = false;
                    }
                    else {
                        rightFlowerIndex--;
                        if (rightFlowerIndex >= 0 && rightFlowerIndex < gameRows) flowers[rightFlowerIndex] = true;
                        placeLeftFlowerNext = true;
                    }
                }
                beesExiting[i] = true;
            }

            if (beesExiting[i]) {
                beePositionsX[i] += beeDirections[i] * currentSpeed * 2.0f;
                if (beePositionsX[i] < -boxPixelsX || beePositionsX[i] > resolutionX + boxPixelsX) {
                    beeActive[i] = false;
                }
            }

               checkAndCreateHiveForBee(i, beePositionsX, beePositionsY, beeDirections, beeActive, beesExiting,
                                     isHoneycomb, honeycombX, honeycombY, beesSpawned,
                                     preHoneycombX, preHoneycombY, preHoneycombActive, totalPreHoneycombs,
                                     beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated);
        }

           bool playerTrapped = true;
        for (int c = 0; c < gameRows; c++) {
            if (!flowers[c]) {
                playerTrapped = false;
                break;
            }
        }
        if (playerTrapped) levelNeedsReset = true;

        beeMoveClock.restart();
    }
}


void drawBees(RenderWindow& window, float beePositionsX[], float beePositionsY[], Sprite beeSprites[], Sprite fastBeeSprite, bool isFastBee[], bool beeActive[], int beesSpawned) {
    for (int i = 0; i < beesSpawned; i++) {
        if (!beeActive[i]) continue;
        if (isFastBee[i]) {
            fastBeeSprite.setPosition(beePositionsX[i], beePositionsY[i]);
           window.draw(fastBeeSprite);
        }
        else {
            beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);
            window.draw(beeSprites[i]);
        }
    }
}

void handleBulletBeeCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float beePositionsX[], float beePositionsY[], bool beeActive[], bool isHoneycomb[], bool isFastBee[], bool isRedHoneycombActive[], int totalBees, float honeycombX[], float honeycombY[], int& beesDestroyed, bool flowers[], int& leftFlowerIndex, int& rightFlowerIndex, int gameRows, int& score) {
    for (int i = 0; i < totalBees; i++) {
        if (bullet_exists && beeActive[i] &&
            bullet_x + (boxPixelsX * 2) > beePositionsX[i] && bullet_x < beePositionsX[i] + boxPixelsX &&
          bullet_y + (boxPixelsY * 2) > beePositionsY[i] && bullet_y < beePositionsY[i] + boxPixelsY) {
            beeActive[i] = false;
            isHoneycomb[i] = true;
          honeycombX[i] = beePositionsX[i];
            honeycombY[i] = beePositionsY[i];

            if (isFastBee[i]) {
                isRedHoneycombActive[i] = true;
                score += 1000; 
            }
            else {
                isRedHoneycombActive[i] = false;
                score += 100; 
            }

            bullet_exists = false;
            beesDestroyed++;
        }
    }
}

void handleBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float honeycombX[], float honeycombY[], bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score) {
     for (int i = 0; i < totalPreHoneycombs; i++) {
        if (!preHoneycombActive[i]) continue;
        bool collidesX = (bullet_x + (boxPixelsX * 2) > preHoneycombX[i]) && (bullet_x < preHoneycombX[i] + boxPixelsX);
        bool collidesY = (bullet_y + (boxPixelsY * 2) > preHoneycombY[i]) && (bullet_y < preHoneycombY[i] + boxPixelsY);
        if (collidesX && collidesY) {
            bullet_exists = false;
            preHoneycombActive[i] = false;

            int tier = static_cast<int>(preHoneycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { 
                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { 
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

            return;
        }
    }
   for (int i = 0; i < totalBees; i++) {
        if (bullet_exists && isHoneycomb[i] &&
            bullet_x + (boxPixelsX * 2) > honeycombX[i] && bullet_x < honeycombX[i] + boxPixelsX &&
            bullet_y + (boxPixelsY * 2) > honeycombY[i] && bullet_y < honeycombY[i] + boxPixelsY) {
           int tier = static_cast<int>(honeycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { 
                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { 
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
              }
                else {
                    score += 1500;
                }
            }

               bullet_exists = false;
            isHoneycomb[i] = false;
            isRedHoneycombActive[i] = false;
        }
    }

      for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
            bool collidesX = (bullet_x + (boxPixelsX * 2) > beehiveX[i]) && (bullet_x < beehiveX[i] + boxPixelsX);
            bool collidesY = (bullet_y + (boxPixelsY * 2) > beehiveY[i]) && (bullet_y < beehiveY[i] + boxPixelsY);
            if (collidesX && collidesY) {
                    int tier = static_cast<int>(beehiveY[i] / boxPixelsY) + 1;

                // Scoring based on tier 
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1600;
                }
                else {
                    score += 1000;
                }

                // Destroy the beehive
                bullet_exists = false;
                beehiveActive[i] = false;  // Destroy the hive
                return; // Stop checking once bullet is gone
            }
      }
   }
}

void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], Sprite& honeycombSprite, Sprite& redHoneycombSprite, bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees) {
    for (int i = 0; i < totalBees; i++) {
        if (isHoneycomb[i]) {
            if (isRedHoneycombActive[i]) {
              redHoneycombSprite.setPosition(honeycombX[i], honeycombY[i]);
                window.draw(redHoneycombSprite);
            }
            else {
                honeycombSprite.setPosition(honeycombX[i], honeycombY[i]);
                window.draw(honeycombSprite);
            }
      }
   }
}

void setupLevel(int currentLevel, int& totalBees, int& fastBees, int& totalHoneycombs, Texture& backgroundTexture,
               Music& bgMusic, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[],
               int maxPreHoneycombs, int& totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[],
               int& beehivesCreated, int maxBeehives) {
      bgMusic.stop();

    if (currentLevel == 1) {
        totalBees = 20;
        fastBees = 0;
        totalHoneycombs = 3;
        if (!bgMusic.openFromFile("Music/Music1.ogg")) {
            cout << "Failed to load Music1.ogg" << endl;
        }
    }
    else if (currentLevel == 2) {
        totalBees = 25;
        fastBees = 5;
        totalHoneycombs = 9;
        if (!bgMusic.openFromFile("Music/Music2.ogg")) {
            cout << "Failed to load Music2.ogg" << endl;
        }
    }
    else if (currentLevel == 3) {
        totalBees = 30;
        fastBees = 10;
        totalHoneycombs = 15;
        
        if (!bgMusic.openFromFile("Music/Music3.ogg")) { 
            cout << "Failed to load Music3.ogg" << endl;
        }
    }
    else if (currentLevel == 4) { 
        totalBees = 35; 
        fastBees = 15;
      beehivesCreated=5;
        totalHoneycombs = 15;
         if (!bgMusic.openFromFile("Music/Music1.ogg")) { // Corrected music file
            cout << "Failed to load Music1.ogg" << endl;
        }
    }


    bgMusic.setLoop(true);
    bgMusic.play();
     
    totalPreHoneycombs = totalHoneycombs;
    for (int i = 0; i < totalPreHoneycombs; i++) {
        bool positionOK = false;
        float x, y;
        int attempts = 0;
        const int maxAttempts = 100;
        while (!positionOK && attempts < maxAttempts) {
            x = static_cast<float>(rand() % (resolutionX - boxPixelsX));
            y = static_cast<float>(rand() % ((gameColumns - 3) * boxPixelsY));

            positionOK = true;
            for (int j = 0; j < i; j++) {
                float dx = x - preHoneycombX[j];
                float dy = y - preHoneycombY[j];
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist < boxPixelsX) {
                    positionOK = false;
                    break;
                }
            }
            attempts++;
        }
        preHoneycombX[i] = x;
        preHoneycombY[i] = y;
        preHoneycombActive[i] = true;
    }
     if (currentLevel == 4) {
        int hivesToGenerate = 5;
        while (hivesToGenerate > 0 && beehivesCreated < maxBeehives) {
            bool positionOK = false;
            float x, y;
           int attempts = 0;
            const int maxAttempts = 100;
            while (!positionOK && attempts < maxAttempts) {
                x = static_cast<float>(rand() % (resolutionX - boxPixelsX));
            y = static_cast<float>(rand() % ((gameColumns - 3) * boxPixelsY));

                positionOK = true;
                for (int j = 0; j < beehivesCreated; j++) {
                    float dx = x - beehiveX[j];
                    float dy = y - beehiveY[j];
                    float dist = sqrtf(dx * dx + dy * dy);
                    if (dist < boxPixelsX) {
                        positionOK = false;
                        break;
                    }
                }
                attempts++;
            }

            if (positionOK) {
                beehiveActive[beehivesCreated] = true;
                beehiveX[beehivesCreated] = x;
                beehiveY[beehivesCreated] = y;
                   beehivesCreated++;
                hivesToGenerate--;
            }
               }

              
    }
}

void moveHummingbird(float& hummingbird_x, float& hummingbird_y, float& hummingbird_directionX, float& hummingbird_directionY, bool& hummingbird_paused, float& hummingbird_pauseTime, Clock& hummingbirdMoveClock, float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY) {
    const float hummingbirdSpeed = 5.0f;
    const float moveInterval = 0.05f; 
    const float pauseDuration = 1.0f; 

    if (hummingbirdMoveClock.getElapsedTime().asSeconds() >= moveInterval) {
        if (hummingbird_paused) {
            hummingbird_pauseTime -= moveInterval;
            if (hummingbird_pauseTime <= 0.0f) {
                hummingbird_paused = false;
                bool honeycombFound = false;
              float targetX = 0.0f;
                float targetY = 0.0f;
                float minDistance = 100000.0f;

                for (int i = 0; i < totalBees; i++) {
                    if (isHoneycomb[i]) {
                        float dx = honeycombX[i] - hummingbird_x;
                        float dy = honeycombY[i] - hummingbird_y;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                          minDistance = distance;
                            targetX = honeycombX[i];
                            targetY = honeycombY[i];
                            honeycombFound = true;
                        }
                    }
                }

                if (honeycombFound) {
                  float dx = targetX - hummingbird_x;
                    float dy = targetY - hummingbird_y;
                    float magnitude = sqrt(dx * dx + dy * dy);
                    if (magnitude != 0) {
                        hummingbird_directionX = dx / magnitude;
                        hummingbird_directionY = dy / magnitude;
                    }
                }
                else {
                    
                    hummingbird_directionX = static_cast<float>(rand() % 3 - 1);
                    hummingbird_directionY = static_cast<float>(rand() % 3 - 1);
                }
            }
        }
        else {
            hummingbird_x += hummingbird_directionX * hummingbirdSpeed;
            hummingbird_y += hummingbird_directionY * hummingbirdSpeed;

            // Boundary checks
            if (hummingbird_x < 0) {
                hummingbird_x = 0;
                hummingbird_directionX *= -1;
                hummingbird_y += boxPixelsY;
            }
            if (hummingbird_x > resolutionX - boxPixelsX) {
                hummingbird_x = resolutionX - boxPixelsX;
                hummingbird_directionX *= -1;
                hummingbird_y += boxPixelsY;
            }
            if (hummingbird_y < 0) {
                hummingbird_y = 0;
                hummingbird_directionY *= -1;
            }
            if (hummingbird_y > resolutionY - boxPixelsY) {
                hummingbird_y = resolutionY - boxPixelsY;
                hummingbird_directionY *= -1;
            }

            // Random pause
            if ((rand() % 1000) < 10) { 
                hummingbird_paused = true;
                hummingbird_pauseTime = pauseDuration;
            }
        }
        hummingbirdMoveClock.restart();
    }
}


void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y, bool hummingbird_visible) {
    if (hummingbird_visible) {
        hummingbirdSprite.setPosition(hummingbird_x, hummingbird_y);
       window.draw(hummingbirdSprite);
    }
}


void handleHummingbirdHoneycombCollision(float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, float hummingbird_x, float hummingbird_y, int& score, int boxPixelsX, int boxPixelsY, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool isRedHoneycombActive[]) {
    
    for (int i = 0; i < totalPreHoneycombs; i++) {
        if (!preHoneycombActive[i]) continue;
        bool collideX = (hummingbird_x + boxPixelsX > preHoneycombX[i]) && (hummingbird_x < preHoneycombX[i] + boxPixelsX);
        bool collideY = (hummingbird_y + boxPixelsY > preHoneycombY[i]) && (hummingbird_y < preHoneycombY[i] + boxPixelsY);
        if (collideX && collideY) {
           
            int tier = static_cast<int>(preHoneycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { // Yellow Honeycomb
                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
               }
                else {
                    score += 500;
                }
            }
            else { 
                if (tier <= 2) {
                    score += 2000;
                }
              else if (tier <= 5) {
                    score += 1800;
                }
                 else {
                    score += 1500;
                }
            }

            //  honeycomb dead
            preHoneycombActive[i] = false;
        }
    }

    //  honeycombs & bees
    for (int i = 0; i < totalBees; i++) {
        if (isHoneycomb[i] &&
            hummingbird_x + boxPixelsX > honeycombX[i] && hummingbird_x < honeycombX[i] + boxPixelsX &&
            hummingbird_y + boxPixelsY > honeycombY[i] && hummingbird_y < honeycombY[i] + boxPixelsY) {
            
           
            int tier = static_cast<int>(honeycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { 
                if (tier <= 2) {
                   score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { // Red Honeycomb
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

         
            isHoneycomb[i] = false;
            isRedHoneycombActive[i] = false;
        }
    }
}


void handleBulletHummingbirdCollision(float bullet_x, float bullet_y, bool& bullet_exists, float hummingbird_x, float hummingbird_y, int& hummingbird_health, bool& hummingbird_visible, Clock& hummingbirdRespawnClock, Sprite& hummingbirdSprite, int boxPixelsX, int boxPixelsY) {
    if (bullet_exists && hummingbird_visible &&
        bullet_x + (boxPixelsX * 2) > hummingbird_x && bullet_x < hummingbird_x + boxPixelsX &&
        bullet_y + (boxPixelsY * 2) > hummingbird_y && bullet_y < hummingbird_y + boxPixelsY) {
        hummingbird_health--;
        bullet_exists = false;
        
        if (hummingbird_health <= 0) {
            hummingbird_visible = false;
            hummingbirdRespawnClock.restart();
        }
        else if (hummingbird_health == 1) {
            hummingbirdSprite.setColor(Color::Green);
        }
    }
}


void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[], int gameRows, int boxPixelsX, int boxPixelsY, int resolutionY) {
    for (int i = 0; i < gameRows; i++) {
        if (flowers[i]) {
            flowerSprite.setPosition(static_cast<float>(i * boxPixelsX), static_cast<float>(resolutionY - boxPixelsY * 3.8f));
            window.draw(flowerSprite);
      }
    }
}

void checkPlayerRestriction(float& player_x, bool flowers[], int gameRows, int boxPixelsX, int& sprayCansLeft, int totalSprayCans, int sprayLevels[], int& currentSprayIndex, int maxSpraysPerCan, GameState& currentState) {
    int playerColumn = static_cast<int>(player_x / boxPixelsX);
    bool restricted = false;

    bool leftBlocked = (playerColumn <= 0) || (playerColumn > 0 && flowers[playerColumn - 1]);
      bool rightBlocked = (playerColumn >= gameRows - 1) || (playerColumn < gameRows - 1 && flowers[playerColumn + 1]);
    if (leftBlocked && rightBlocked) {
        restricted = true;
    }

    if (restricted) {
        bool openSpaceFound = false;
         for (int i = 0; i < gameRows; i++) {
            if (!flowers[i]) {
                player_x = static_cast<float>(i * boxPixelsX);
                openSpaceFound = true;
                break;
            }
        }
        if (!openSpaceFound) {
            if (sprayCansLeft > 0) {
                for (int i = 0; i < gameRows; i++) {
                    flowers[i] = false;
                 }
                sprayCansLeft--;
                for (int i = 0; i < totalSprayCans; i++) {
                   sprayLevels[i] = maxSpraysPerCan;
                }
                currentSprayIndex = 0;
                player_x = static_cast<float>((gameRows / 2) * boxPixelsX);
            }
            else {
                currentState = GAME_OVER;
           }
      }
    }
}

void initBeeHives(bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated) {
    for (int i = 0; i < maxBeehives; i++) {
        beehiveActive[i] = false;
       beehiveX[i] = 0.0f;
        beehiveY[i] = 0.0f;
    }
    beehivesCreated = 0;
}

void drawBeehives(RenderWindow& window, Sprite& beehiveSprite,
                  const bool beehiveActive[], const float beehiveX[], const float beehiveY[],
                  int beehivesCreated) {
      for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
                  if (beehiveX[i] < 0 || beehiveX[i] > resolutionX - boxPixelsX ||
                beehiveY[i] < 0 || beehiveY[i] > resolutionY - boxPixelsY) {
                     continue; 
            }
            beehiveSprite.setPosition(beehiveX[i], beehiveY[i]);
            window.draw(beehiveSprite);
        }
    }
}


void handleBeehiveCompletion(bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score) {
    for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
                int tier = static_cast<int>(beehiveY[i] / boxPixelsY) + 1;

                  if (tier <= 2) {
                score += 2000;
            }
            else if (tier <= 5) {
                score += 1600;
            }
            else {
                score += 1000;
            }

                    beehiveActive[i] = false;
        }
    }
}

void displayLevelTransition(RenderWindow& window, Font& font, int currentLevel, Clock& transitionClock, bool& transitionComplete) {
    Text levelText;
    levelText.setFont(font);
    levelText.setString("Level " + to_string(currentLevel));
    levelText.setCharacterSize(60);
   levelText.setFillColor(Color::Yellow);
    levelText.setPosition(static_cast<float>(resolutionX) / 2 - 150.0f, static_cast<float>(resolutionY) / 2 - 30.0f);
    window.draw(levelText);

    if (transitionClock.getElapsedTime().asSeconds() > 1.0f) { 
        transitionComplete = true;
         transitionClock.restart();
    }
}

void loadHighScores(string filename, string names[], int scores[]) {
      for (int i = 0; i < 10; i++) {
        names[i] = "AAA";
        scores[i] = 0;
    }
    ifstream fin(filename.c_str());
    if (!fin.fail()) {
        int idx = 0;
        while (!fin.eof() && idx < 10) {
            string n;
            int s;
            fin >> n >> s;
            if (fin.fail()) break;
            names[idx] = n;
            scores[idx] = s;
            idx++;
        }
    }
    fin.close();
}

void saveHighScores(const string& filename, const std::string names[], const int scores[]) {
    ofstream fout(filename.c_str());
    if (!fout.is_open()) {
        cout << "Failed to open high score file for writing.\n";
        return;
    }
    for (int i = 0; i < 10; i++) {
        fout << names[i] << " " << scores[i] << "\n";
    }
    fout.close();
}

bool addNewScore(string filename, string playerName, int newScore) {
    string names[10];
    int scores[10];
    loadHighScores(filename, names, scores);
    
      if (newScore > scores[9]) { 
        int pos = 0;
        while (pos < 10 && newScore <= scores[pos]) {
            pos++;
        }
        if (pos < 10) {
               for (int i = 9; i > pos; i--) {
                names[i] = names[i - 1];
                scores[i] = scores[i - 1];
            }
              names[pos] = playerName;
            scores[pos] = newScore;
            
                 
                saveHighScores(filename, names, scores);
            return true; 
        }
    }
    return false; 
}

void displayHighScoresOnScreen(RenderWindow& window, Font& font, string names[], int scores[]) {
      RectangleShape overlay(Vector2f(resolutionX, resolutionY));
    overlay.setFillColor(Color(0, 0, 0, 150));
    window.draw(overlay);
    
      Text title;
    title.setFont(font);
    title.setString("High Scores");
    title.setCharacterSize(60);
    title.setFillColor(Color::Yellow);
    title.setPosition(resolutionX / 2.0f - 150.0f, 50.0f);
    window.draw(title);
    
      float startY = 150.0f;
    for (int i = 0; i < 10; i++) {
        Text line;
        line.setFont(font);
        line.setString(to_string(i + 1) + ". " + names[i] + " - " + to_string(scores[i]));
        line.setCharacterSize(30);
        line.setFillColor(Color::White);
        line.setPosition(resolutionX / 2.0f - 150.0f, startY + i * 40.0f);
        window.draw(line);
    }
    
        Text backText;
    backText.setFont(font);
    backText.setString("Press Escape to return");
    backText.setCharacterSize(25);
    backText.setFillColor(Color::Cyan);
    backText.setPosition(resolutionX / 2.0f - 120.0f, startY + 10 * 40.0f);
    window.draw(backText);
}

void showHighScores(RenderWindow& window, Font& font) {
    string names[10];
    int scores[10];
    loadHighScores("highscores.txt", names, scores);
    displayHighScoresOnScreen(window, font, names, scores);
}

