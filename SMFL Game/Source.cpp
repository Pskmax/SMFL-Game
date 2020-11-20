#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <stdlib.h>
#include<math.h>
#include<fstream>
#include<string>
#include <sstream> 
using namespace std;
int main()
{
    int blank = 3, blankcooldown = 0, maxblank=blank;
    float bulletx[5500] = { NULL-1000 }, bullety[5500] = { NULL-1000 }, levelbullet[5500] = {NULL};
    float bbulletx[5500] = { NULL-1000 }, bbullety[5500] = { NULL-1000 }, bdeg[5500] = { NULL }, bvel[5500] = { NULL }, btype[5500] = {NULL};
    float degree1 = 0,degree2 = 0;
    float playerAnimationTime = 0;
    int bosstotalbullet = 0;
    int totalbullet = 0;
    int playerAnimationFrame = 0;
    float cooldown = 0;
    float bosscooldown1 = 0;
    float bosscooldown2 = 0;
    int bossmaxhealth = 1000;
    int bosshealth = bossmaxhealth;
    int r = 0.2;
    int maxhealth = 5;
    int playerhealth = maxhealth;
    int gamephase = 0; //0-5 for stage 1 
    int score=0;
    int timebonus = 0;
    int healthbonus = 0;
    int bossbonus = 0;
    int blankbonus = 0;
    float gamephase7timer = 0;
    float sumtime = 0;
    float phasetime = 0;
    const double Pi = 3.14159265358979323846;
    unsigned int screensizex = 480, screensizey = 650;
    int menu = 2; //0=menu 1=game 2=gameover 
    bool gameoverscorecheck = 0;
    //collision test
    srand(time(NULL));
    float Random = rand() % 380;
    sf::Texture enemietexture;
    sf::RectangleShape enemie;
    enemie.setSize(sf::Vector2f(100.f, 100.f));
    enemie.setTexture(&enemietexture);
    enemie.setPosition({ Random, 0.f });
    enemie.setTextureRect(sf::IntRect(0, 0, 100, 100));
    int colliy = 0;
    if (!enemietexture.loadFromFile("resources/enemie.png"))
    {
        std::cout << "Load failed" << std::endl;
    }

    //lesser (stage2 enemies)            50,72
    sf::RectangleShape lesser;
    lesser.setSize(sf::Vector2f(100.f, 100.f));
    lesser.setTexture(&enemietexture);
    lesser.setTextureRect(sf::IntRect(0, 0, 100, 100));
    int lesserHealth[150] = {NULL};
    float lesserCooldown[150] = { NULL };
    float lesserX[150] = { NULL };
    float lesserY[150] = { NULL };
    float lesserDeg[150] = { NULL };
    int lesserNow = 0;


    //boss
    sf::Texture BossTexture;
    sf::RectangleShape boss;
    boss.setSize(sf::Vector2f(160.f, 182.f));
    boss.setTexture(&BossTexture);
    boss.setPosition({ screensizex / 2 - 80.f,-200.f });
    boss.setTextureRect(sf::IntRect(0, 0, 162, 182));
    if (!BossTexture.loadFromFile("resources/boss.png"))
    {
        std::cout << "Load failed" << std::endl;
    }

    //window
    sf::RenderWindow window(sf::VideoMode(screensizex, screensizey), "Galaxy Defender");



    //map
    sf::Texture MapTexture;
    sf::Sprite Map1;
    Map1.setTexture(MapTexture);
    Map1.setTextureRect(sf::IntRect(0, 0, 480, 852));
    Map1.setPosition(0.f, 0.f);
    sf::Sprite Map2;
    Map2.setTexture(MapTexture);
    Map2.setTextureRect(sf::IntRect(0, 0, 480, 852));
    Map2.setPosition(0.f, -852.f);
    if (!MapTexture.loadFromFile("resources/map.jpg"))
    {
        std::cout << "Load failed" << std::endl;
    }
    int Mapy = 0;

    //player
    sf::Texture PlayerTexture;
    sf::Sprite PlayerSprite;
    PlayerSprite.setTexture(PlayerTexture);
    PlayerSprite.setTextureRect(sf::IntRect(0, 0, 40, 37));
    sf::Vector2f spawnPoint = { screensizex / 2 - 20.f, screensizey - 50.f };
    PlayerSprite.setPosition(spawnPoint);
    if (!PlayerTexture.loadFromFile("resources/player.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    sf::CircleShape hitbox(1.f);
    sf::Vector2f hbspawnPoint = { screensizex / 2 - 1.f,screensizey - 37.f };
    hitbox.setPosition(hbspawnPoint);
    hitbox.setFillColor(sf::Color::Red);
    double PlayerVelocity = 0.2;

    //bullet
    sf::Texture BulletTexture;
    sf::Sprite BulletSprite;
    BulletSprite.setTexture(BulletTexture);
    BulletSprite.setTextureRect(sf::IntRect(1, 1, 7, 7));//smallest red
    if (!BulletTexture.loadFromFile("resources/bullet.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    float PBulletVelocity = 0.5;
    int bulletlevel=1;

    //boss bullet
    sf::CircleShape BB(10.f);
    BB.setTexture(&BulletTexture);
    BB.setTextureRect(sf::IntRect(540, 34, 20, 20));
    BB.setPosition(0, 0);

    //fps counter
    sf::Font font;
    if (!font.loadFromFile("resources/trebuc.ttf"))
    {
        std::cout << "Load failed" << std::endl;
    }
    sf::Font pixelfont;
    if(!pixelfont.loadFromFile("resources/game_over.ttf"))
    {
        std::cout << "Load failed" << std::endl;
    }
    /*
    sf::Text frameRate;
    frameRate.setCharacterSize(20);
    frameRate.setFont(font);
    frameRate.setPosition(10, 10);
    frameRate.setFillColor(sf::Color::White);
    */
    //score
    sf::Text scoreText;
    scoreText.setCharacterSize(50);
    scoreText.setFont(pixelfont);
    scoreText.setPosition(90, 10);
    scoreText.setFillColor(sf::Color::White);
    sf::Text scorebase;
    scorebase.setCharacterSize(50);
    scorebase.setFont(pixelfont);
    scorebase.setPosition(10, 10);
    scorebase.setFillColor(sf::Color::White);
    scorebase.setString("Score :");
    sf::Text stageTime;
    stageTime.setCharacterSize(50);
    stageTime.setFont(pixelfont);
    stageTime.setPosition(230,150);
    stageTime.setFillColor(sf::Color(255,179,236));
    sf::Text stageTimeBase;
    stageTimeBase.setCharacterSize(50);
    stageTimeBase.setFont(pixelfont);
    stageTimeBase.setPosition(150, 150);
    stageTimeBase.setFillColor(sf::Color(255, 179, 236));
    stageTimeBase.setString("Time :");
    sf::Text stageTimeBonusBase;
    stageTimeBonusBase.setCharacterSize(50);
    stageTimeBonusBase.setFont(pixelfont);
    stageTimeBonusBase.setPosition(150, 180);
    stageTimeBonusBase.setFillColor(sf::Color(255, 179, 236));
    stageTimeBonusBase.setString("Bonus :");
    sf::Text stageTimeBonus;
    stageTimeBonus.setCharacterSize(50);
    stageTimeBonus.setFont(pixelfont);
    stageTimeBonus.setPosition(230, 180);
    stageTimeBonus.setFillColor(sf::Color(255, 179, 236));
    sf::Text HealthBase;
    HealthBase.setCharacterSize(50);
    HealthBase.setFont(pixelfont);
    HealthBase.setPosition(150, 210);
    HealthBase.setFillColor(sf::Color(179,181,255));
    HealthBase.setString("Health :");
    sf::Text HealthText;
    HealthText.setCharacterSize(50);
    HealthText.setFont(pixelfont);
    HealthText.setPosition(230, 210);
    HealthText.setFillColor(sf::Color(179, 181, 255));
    sf::Text HealthBonusBase;
    HealthBonusBase.setCharacterSize(50);
    HealthBonusBase.setFont(pixelfont);
    HealthBonusBase.setPosition(150, 240);
    HealthBonusBase.setFillColor(sf::Color(179, 181, 255));
    HealthBonusBase.setString("Bonus :");
    sf::Text HealthBonus;
    HealthBonus.setCharacterSize(50);
    HealthBonus.setFont(pixelfont);
    HealthBonus.setPosition(230, 240);
    HealthBonus.setFillColor(sf::Color(179, 181, 255));
    sf::Text BossKilled;
    BossKilled.setCharacterSize(50);
    BossKilled.setFont(pixelfont);
    BossKilled.setPosition(230, 270);
    BossKilled.setFillColor(sf::Color(181,255,179));
    sf::Text BossKilledBase;
    BossKilledBase.setCharacterSize(50);
    BossKilledBase.setFont(pixelfont);
    BossKilledBase.setPosition(150, 270);
    BossKilledBase.setFillColor(sf::Color(181, 255, 179));
    BossKilledBase.setString("Boss :");
    sf::Text BossKilledBonus;
    BossKilledBonus.setCharacterSize(50);
    BossKilledBonus.setFont(pixelfont);
    BossKilledBonus.setPosition(230, 300);
    BossKilledBonus.setFillColor(sf::Color(181, 255, 179));
    sf::Text BossKilledBonusBase;
    BossKilledBonusBase.setCharacterSize(50);
    BossKilledBonusBase.setFont(pixelfont);
    BossKilledBonusBase.setPosition(150, 300);
    BossKilledBonusBase.setFillColor(sf::Color(181, 255, 179));
    BossKilledBonusBase.setString("Bonus :");
    sf::Text blankText;
    blankText.setCharacterSize(50);
    blankText.setFont(pixelfont);
    blankText.setPosition(230, 330);
    blankText.setFillColor(sf::Color(255,250,179));
    sf::Text blankTextBase;
    blankTextBase.setCharacterSize(50);
    blankTextBase.setFont(pixelfont);
    blankTextBase.setPosition(150, 330);
    blankTextBase.setFillColor(sf::Color(255, 250, 179));
    blankTextBase.setString("Blank :");
    sf::Text blankBonus;
    blankBonus.setCharacterSize(50);
    blankBonus.setFont(pixelfont);
    blankBonus.setPosition(230, 360);
    blankBonus.setFillColor(sf::Color(255, 250, 179));
    sf::Text blankBonusBase;
    blankBonusBase.setCharacterSize(50);
    blankBonusBase.setFont(pixelfont);
    blankBonusBase.setPosition(150, 360);
    blankBonusBase.setFillColor(sf::Color(255, 250, 179));
    blankBonusBase.setString("Bonus :");
    float stagetimer = 0;
    sf::Text StageText;
    StageText.setCharacterSize(50);
    StageText.setFont(pixelfont);
    StageText.setPosition(180, 300);
    StageText.setFillColor(sf::Color::White);
    StageText.setString("Stage");
    sf::Text Stage;
    Stage.setCharacterSize(50);
    Stage.setFont(pixelfont);
    Stage.setPosition(260, 300);
    Stage.setFillColor(sf::Color::White);
    //menu
    sf::Sprite menupng;
    sf::Texture menuTexture;
    menuTexture.loadFromFile("resources/menu.png");
    menupng.setTexture(menuTexture);
    sf::Text gamename;
    gamename.setCharacterSize(100);
    gamename.setFont(pixelfont);
    gamename.setPosition(90, 100);
    gamename.setFillColor(sf::Color::White);
    gamename.setString("Galaxy Defender");
    sf::Text start;
    start.setCharacterSize(100);
    start.setFont(pixelfont);
    start.setPosition(190, 200);
    start.setFillColor(sf::Color::Cyan);
    start.setString("Start");
    sf::RectangleShape testblock;
    testblock.setSize(sf::Vector2f(200,30));
    testblock.setFillColor(sf::Color::Green);
    testblock.setPosition(140,370);
    //gameover
    sf::Sprite gameoverpng;
    sf::Texture gameoverTexture;
    gameoverTexture.loadFromFile("resources/gameover.jpg");
    gameoverpng.setTexture(gameoverTexture);
    sf::Text gameovertext;
    gameovertext.setCharacterSize(150);
    gameovertext.setFont(pixelfont);
    gameovertext.setPosition(100, 150);
    gameovertext.setFillColor(sf::Color::White);
    gameovertext.setString("Game Over");
    sf::Text mainmenu;
    mainmenu.setCharacterSize(100);
    mainmenu.setFont(pixelfont);
    mainmenu.setPosition(140, 500);
    mainmenu.setFillColor(sf::Color::White);
    mainmenu.setString("Main Menu");
    sf::Text highscoregameover;
    highscoregameover.setCharacterSize(100);
    highscoregameover.setFont(pixelfont);
    highscoregameover.setPosition(320, 350);
    highscoregameover.setFillColor(sf::Color::White);
    sf::Text highscoregameovertext;
    highscoregameovertext.setCharacterSize(100);
    highscoregameovertext.setFont(pixelfont);
    highscoregameovertext.setPosition(100, 350);
    highscoregameovertext.setFillColor(sf::Color::White);
    highscoregameovertext.setString("Highscore :");
    //music
    sf::Music stage;
    if (!stage.openFromFile("resources/stage.ogg"))
        return 0;
    sf::Music bossmusic;
    if (!bossmusic.openFromFile("resources/boss.ogg"))
        return 0;
    sf::Music victory;
    if (!victory.openFromFile("resources/victory.ogg"))
        return 0;
    stage.setVolume(30.f);
    bossmusic.setVolume(30.f);
    victory.setVolume(30.f);
    //healthbar
    sf::Sprite playerHealthBase;
    sf::Texture playerHealthTexture;
    playerHealthBase.setTexture(playerHealthTexture);
    playerHealthBase.setTextureRect(sf::IntRect(0, 41, 14, 17));
    playerHealthBase.setPosition(5,400);
    if (!playerHealthTexture.loadFromFile("resources/health.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    sf::Sprite playerHealthHave;
    playerHealthHave.setTexture(playerHealthTexture);
    playerHealthHave.setTextureRect(sf::IntRect(0, 38, 14, 2));
    sf::Sprite playerHealthEmpty;
    playerHealthEmpty.setTexture(playerHealthTexture);
    playerHealthEmpty.setTextureRect(sf::IntRect(0, 20, 14, 2));
    sf::Sprite playerHealthCap;
    playerHealthCap.setTexture(playerHealthTexture);
    playerHealthCap.setTextureRect(sf::IntRect(0, 0, 14, 4));
    playerHealthCap.setPosition(5, 397 - 2 * maxhealth*3);

    //blank
    sf::Sprite blankBase;
    sf::Texture blankTexture;
    blankBase.setTexture(blankTexture);
    blankBase.setTextureRect(sf::IntRect(0, 41, 14, 17));
    blankBase.setPosition(25, 400);
    if (!blankTexture.loadFromFile("resources/blank.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    sf::Sprite blankHave;
    blankHave.setTexture(blankTexture);
    blankHave.setTextureRect(sf::IntRect(0, 38, 14, 2));
    sf::Sprite blankEmpty;
    blankEmpty.setTexture(blankTexture);
    blankEmpty.setTextureRect(sf::IntRect(0, 20, 14, 2));
    sf::Sprite blankCap;
    blankCap.setTexture(blankTexture);
    blankCap.setTextureRect(sf::IntRect(0, 0, 14, 4));
    blankCap.setPosition(25, 397 - 2 * maxblank*3);

    //boss healthbar
    sf::Sprite bossHealthBase;
    sf::Texture bossHealthTexture;
    bossHealthBase.setTexture(bossHealthTexture);
    bossHealthBase.setTextureRect(sf::IntRect(0, 41, 14, 17));
    bossHealthBase.setPosition(screensizex-19, 400);
    if (!bossHealthTexture.loadFromFile("resources/bosshealth.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    sf::Sprite bossHealthHave;
    bossHealthHave.setTexture(bossHealthTexture);
    bossHealthHave.setTextureRect(sf::IntRect(0, 38, 14, 2));
    sf::Sprite bossHealthEmpty;
    bossHealthEmpty.setTexture(bossHealthTexture);
    bossHealthEmpty.setTextureRect(sf::IntRect(0, 20, 14, 2));
    sf::Sprite bossHealthCap;
    bossHealthCap.setTexture(bossHealthTexture);
    bossHealthCap.setTextureRect(sf::IntRect(0, 0, 14, 3));
    bossHealthCap.setPosition(screensizex-19, 398 - 2 * bossmaxhealth/10);
    int Explosionnow = 0;
    int ExplosionAnimation[110] = { NULL };
    double explosiontime = 0;
    float explosionx[110] = { NULL };
    float explosiony[110] = { NULL };
    int ExplosionSize[110] = { NULL };
    for (int i = 0; i < 110; i++)
    {
        explosionx[i] = NULL - 100;
        explosiony[i] = NULL - 100;

    }
    //small explosion
    sf::Sprite smallExplosion;
    sf::Texture smallExplosionTexture;
    smallExplosion.setTexture(smallExplosionTexture);
    smallExplosion.setTextureRect(sf::IntRect(0, 0, 36, 47));
    if (!smallExplosionTexture.loadFromFile("resources/Smallexplosion.png"))
    {
        std::cout << "Load Failed" << endl;
    }
    //big explosion
    sf::Sprite bigExplosion;
    sf::Texture bigExplosionTexture;
        bigExplosion.setTexture(bigExplosionTexture);
        bigExplosion.setTextureRect(sf::IntRect(88,4,82,82));
    if (!bigExplosionTexture.loadFromFile("resources/Bigexplosion.png"))
    {
        std::cout << "Load Failed" << endl;
    }
    double bossdefeatexplosion = 0;
    int bossdefeatexplosioncount = 0;

    //clock
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        sf::Time timeElapsed = clock.restart();
        double deltatime = timeElapsed.asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        if (menu == 0) {
            window.draw(menupng);
            window.draw(gamename);
            window.draw(start);
            window.display();
            sf::Vector2i position = sf::Mouse::getPosition(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&(position.x>=190&&position.x<=290)&&(position.y>=270&&position.y<=300))
            {
                bulletlevel = 1;
                boss.setPosition({ screensizex / 2 - 80.f,-200.f });
                bossdefeatexplosioncount = 0;
                stagetimer = 0;
                playerhealth = 5;
                blank = 3;
                phasetime = 0;
                score = 0;
                sumtime = 0;
                gamephase = 0;
                bosshealth = 1000;
                scorebase.setCharacterSize(50);
                scoreText.setCharacterSize(50);
                for (int i = 0; i < 1050; i++)
                {
                    bbulletx[i] = NULL;
                    bbullety[i] = NULL;
                    bulletx[i] = NULL;
                    bullety[i] = NULL;
                }
                for (int i = 0; i < 110; i++)
                {
                    explosionx[i] = NULL - 100;
                    explosiony[i] = NULL - 100;

                }
                PlayerSprite.setPosition(spawnPoint);
                hitbox.setPosition(hbspawnPoint);
                menu = 1;
            }
        }
        if (menu == 2) {
            window.draw(gameoverpng);
            window.draw(gameovertext);
            window.draw(highscoregameover);
            window.draw(highscoregameovertext);
            window.draw(mainmenu);
            scoreText.setString(std::to_string(score));
            scoreText.setPosition(270, 300);
            scorebase.setPosition(140, 300);
            scorebase.setCharacterSize(100);
            scoreText.setCharacterSize(100);
            window.draw(scorebase);
            window.draw(scoreText);
            window.display();
            if (gameoverscorecheck == 0) {
                string highscorein;
                ifstream in("Highscore.txt");
                getline(in, highscorein);
                in.close();
                stringstream highscoreinbuttext(highscorein);
                int highscoreinbuttextbutint = 0;
                highscoreinbuttext >> highscoreinbuttextbutint;
                ofstream out("Highscore.txt");
                if (highscoreinbuttextbutint < score)
                {
                    out << score;
                    highscoregameover.setString(std::to_string(score));
                }
                else
                {
                    out << highscoreinbuttextbutint;
                    highscoregameover.setString(std::to_string(highscoreinbuttextbutint));
                }
                out.close();
                gameoverscorecheck = 1;
            }
            sf::Vector2i position = sf::Mouse::getPosition(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (position.x >= 140 && position.x <= 340) && (position.y >= 570 && position.y <= 600))
            {
                gameoverscorecheck = 0;
                menu = 0;
            }
        }
        if (menu == 1) {
            //map
            window.draw(Map1);
            window.draw(Map2);
            Map1.move(0.f, 50 * deltatime);
            Map2.move(0.f, 50 * deltatime);
            if (Map2.getPosition().y > 0)
            {
                Map1.setPosition(0.f, 0.f);
                Map2.setPosition(0.f, -852.f);
            }
            window.draw(PlayerSprite);
            window.draw(hitbox);
            /*
            //fps
            frameRate.setString(std::to_string(1.0f / timeElapsed.asSeconds()));
            window.draw(frameRate);
            */

            //bullet update
            for (int i = 0; i <= 5000; i++)
            {
                if (bulletx[i] == NULL && bullety[i] == NULL)
                {
                    continue;
                }
                if (levelbullet[i] == 1)
                {
                    BulletSprite.setTexture(BulletTexture);
                    BulletSprite.setTextureRect(sf::IntRect(189, 14, 20, 15));
                }
                if (levelbullet[i] == 2)
                {
                    BulletSprite.setTexture(BulletTexture);
                    BulletSprite.setTextureRect(sf::IntRect(211, 14, 20, 15));
                }
                if (levelbullet[i] == 3)
                {
                    BulletSprite.setTexture(BulletTexture);
                    BulletSprite.setTextureRect(sf::IntRect(233, 14, 20, 15));
                }
                if (levelbullet[i] == 4)
                {
                    BulletSprite.setTexture(BulletTexture);
                    BulletSprite.setTextureRect(sf::IntRect(255, 14, 20, 15));
                }
                bullety[i] = bullety[i] - PBulletVelocity * timeElapsed.asSeconds() * 1200;
                BulletSprite.setPosition(bulletx[i], bullety[i]);

                if (enemie.getGlobalBounds().intersects(BulletSprite.getGlobalBounds()) && gamephase == 1 && enemie.getPosition().y > 0.f) {
                    bigExplosion.setPosition(enemie.getPosition());
                    explosionx[Explosionnow] = enemie.getPosition().x;
                    explosiony[Explosionnow] = enemie.getPosition().y;
                    ExplosionAnimation[Explosionnow] = 0;
                    ExplosionSize[Explosionnow] = 3;
                    float random = rand() % 380;
                    enemie.setPosition(random, -100.f);
                    bullety[i] = NULL;
                    bulletx[i] = NULL;
                    BulletSprite.setPosition(NULL, NULL);
                    score++;
                    Explosionnow++;
                }
                //lesser (stage 2) hit by bullet
                if (gamephase==7)
                for (int j = 0; j < 150; j++)
                {
                    if (lesserX[j] == NULL && lesserY[j] == NULL) continue;
                    lesser.setPosition(lesserX[j], lesserY[j]);
                    if (lesser.getGlobalBounds().intersects(BulletSprite.getGlobalBounds()))
                    {
                        bullety[i] = NULL;
                        bulletx[i] = NULL;
                        BulletSprite.setPosition(NULL, NULL);
                        float random = rand() % 90;
                        explosionx[Explosionnow] = lesserX[j]+random;
                        random = rand() % 90;
                        explosiony[Explosionnow] = lesserY[j]+random;
                        ExplosionAnimation[Explosionnow] = 0;
                        ExplosionSize[Explosionnow] = 1;
                        lesserHealth[j]--;
                        Explosionnow++;
                    }
                }
                if (boss.getGlobalBounds().intersects(BulletSprite.getGlobalBounds()) && gamephase == 3) {

                    //medium explosion

                    if (bosshealth % 100 == 0) {
                        float random = rand() % 1400 / 10;
                        explosionx[Explosionnow] = boss.getPosition().x + random - 40.f;
                        random = rand() % 1620 / 10;
                        explosiony[Explosionnow] = boss.getPosition().y + random - 40.f;
                        ExplosionAnimation[Explosionnow] = 0;
                        ExplosionSize[Explosionnow] = 3;
                        Explosionnow++;
                    }
                    if (bosshealth % 10 == 0) {
                        float random = rand() % 1400 / 10;
                        explosionx[Explosionnow] = boss.getPosition().x + random;
                        random = rand() % 1620 / 10;
                        explosiony[Explosionnow] = boss.getPosition().y + random;
                        ExplosionAnimation[Explosionnow] = 0;
                        ExplosionSize[Explosionnow] = 1;
                        Explosionnow++;
                    }
                    bosshealth = bosshealth - 1;
                    bullety[i] = NULL;
                    bulletx[i] = NULL;
                    BulletSprite.setPosition(NULL, NULL);
                }
                //out of bound
                if (bullety[i] < -5)
                {
                    bullety[i] = NULL;
                    bulletx[i] = NULL;
                }
                if (bulletx[i] != NULL && bullety[i] != NULL)
                    window.draw(BulletSprite);
            }
            //bossbulletupdate
            for (int i = 0; i <= 5000; i++)
            {
                if (bbulletx[i] == NULL && bbullety[i] == NULL)
                {
                    continue;
                }
                bbullety[i] = bbullety[i] + deltatime * 1000 * bvel[i] * sin(bdeg[i]);
                bbulletx[i] = bbulletx[i] + deltatime * 1000 * bvel[i] * cos(bdeg[i]);
                BB.setPosition(bbulletx[i], bbullety[i]);
                if (btype[i] == 1) //meduim pink
                {
                    BB.setTextureRect(sf::IntRect(540, 34, 20, 20));
                }
                if (btype[i] == 2) //small red
                {
                    BB.setTextureRect(sf::IntRect(1, 48, 14, 14));
                }
                if (btype[i] == 3)//small orange
                {
                    BB.setTextureRect(sf::IntRect(15, 48, 14, 14));
                }
                if (btype[i] == 4)//small yellow
                {
                    BB.setTextureRect(sf::IntRect(29, 48, 14, 14));
                }
                if (btype[i] == 5)//small lime
                {
                    BB.setTextureRect(sf::IntRect(43, 48, 14, 14));
                }
                if (btype[i] == 6)//small green
                {
                    BB.setTextureRect(sf::IntRect(57, 48, 14, 14));
                }
                window.draw(BB);

                //player hit bullet
                if (hitbox.getGlobalBounds().intersects(BB.getGlobalBounds())) {
                    std::cout << "Hit" << endl;
                    playerhealth--;
                    BB.setPosition(NULL, NULL);
                    bbulletx[i] = NULL;
                    bbullety[i] = NULL;
                }



                //out of bound
                if (bbullety[i] < -20 || bbullety[i]>screensizey || bbulletx[i]<-20 || bbulletx[i]>screensizex) {
                    bbullety[i] = NULL;
                    bbulletx[i] = NULL;
                }

            }


            //collision
            if (gamephase == 1) {
                window.draw(enemie);
                enemie.move(0.f, deltatime * 300.f);
                if (enemie.getPosition().y >= screensizey)
                {
                    Random = rand() % 470;
                    enemie.setPosition(Random, -100.f);
                }
            }


            //gamephase
            if (gamephase == 0) {
                Stage.setString(std::to_string(1));
                window.draw(StageText);
                window.draw(Stage);
                stage.play();
            }
            if (gamephase == 0 && phasetime >= 3) { gamephase = 1; }
            else phasetime = phasetime + deltatime;
            if (score >= 5 && gamephase == 1) { stage.stop(); gamephase = 2; }
            if (gamephase == 2)
            {
                boss.move(0.f, deltatime * 50.f);
                window.draw(boss);
            }
            if (boss.getPosition().y >= 0 && gamephase == 2) { bossmusic.play(); gamephase = 3; }
            if (bosshealth <= 0 && gamephase == 3) { gamephase = 4; }
            if (gamephase == 4)
            {
                window.draw(boss);
                if (bossdefeatexplosion > 0.1) {
                    float random = rand() % 1400 / 10;
                    explosionx[Explosionnow] = boss.getPosition().x + random - 40.f;
                    random = rand() % 1620 / 10;
                    explosiony[Explosionnow] = boss.getPosition().y + random - 40.f;
                    ExplosionAnimation[Explosionnow] = 0;
                    ExplosionSize[Explosionnow] = 3;
                    Explosionnow++;
                    bossdefeatexplosion = 0;
                    bossdefeatexplosioncount++;
                }
                else bossdefeatexplosion = bossdefeatexplosion + deltatime;
            }
            if (bossdefeatexplosioncount >= 100) { bossmusic.stop(); gamephase = 5; }
            if (gamephase == 5) //stagescorecalculation
            {
                if (sumtime == 0)
                {
                    victory.play();
                    timebonus = (200 - (int)stagetimer) * 5;
                    healthbonus = playerhealth * 100;
                    bossbonus = 500;
                    blankbonus = blank * 100;
                    boss.setPosition(NULL, NULL);
                    stageTime.setString(std::to_string(stagetimer));
                    stageTimeBonus.setString(std::to_string(timebonus));
                    HealthText.setString(std::to_string(playerhealth));
                    HealthBonus.setString(std::to_string(healthbonus));
                    BossKilled.setString("Destroyed");
                    BossKilledBonus.setString(std::to_string(bossbonus));
                    blankText.setString(std::to_string(blank));
                    blankBonus.setString(std::to_string(blankbonus));
                    score = score + timebonus + healthbonus + bossbonus + blankbonus;
                    scoreText.setString(std::to_string(score));
                    scoreText.setPosition(230, 450);
                    scorebase.setPosition(150, 450);
                }
                if (sumtime > 0.5)
                {
                    window.draw(stageTimeBase);
                    window.draw(stageTime);
                }
                if (sumtime > 1)
                {
                    window.draw(stageTimeBonusBase);
                    window.draw(stageTimeBonus);
                }
                if (sumtime > 1.5)
                {
                    window.draw(HealthBase);
                    window.draw(HealthText);
                }
                if (sumtime > 2)
                {
                    window.draw(HealthBonus);
                    window.draw(HealthBonusBase);
                }
                if (sumtime > 2.5)
                {
                    window.draw(BossKilled);
                    window.draw(BossKilledBase);
                }
                if (sumtime > 3)
                {
                    window.draw(BossKilledBonusBase);
                    window.draw(BossKilledBonus);
                }
                if (sumtime > 3.5)
                {
                    window.draw(blankText);
                    window.draw(blankTextBase);
                }
                if (sumtime > 4)
                {
                    window.draw(blankBonus);
                    window.draw(blankBonusBase);
                }
                if (sumtime > 4.5)
                {
                    window.draw(scoreText);
                    window.draw(scorebase);
                }
                if (sumtime > 10) // gameover
                {
                    boss.setPosition({ screensizex / 2 - 80.f,-200.f });
                    bossdefeatexplosioncount = 0;
                    stagetimer = 0;
                    playerhealth = 5;
                    blank = 3;
                    phasetime = 0;
                    sumtime = 0;
                    bosshealth = 1000;
                    scorebase.setCharacterSize(50);
                    scoreText.setCharacterSize(50);
                    for (int i = 0; i < 1050; i++)
                    {
                        bbulletx[i] = NULL;
                        bbullety[i] = NULL;
                        bulletx[i] = NULL;
                        bullety[i] = NULL;
                    }
                    for (int i = 0; i < 110; i++)
                    {
                        explosionx[i] = NULL - 100;
                        explosiony[i] = NULL - 100;

                    }
                    victory.stop();
                    gamephase = 6;
                }
                sumtime = sumtime + deltatime;
            }
            else {
                stagetimer = stagetimer + deltatime;
                //score
                scoreText.setString(std::to_string(score));
                scoreText.setPosition(90, 10);
                scorebase.setPosition(10, 10);
                window.draw(scoreText);
                window.draw(scorebase);
            }
            if (gamephase == 6) {
                stage.play();
                Stage.setString(std::to_string(2));
                window.draw(StageText);
                window.draw(Stage);
            }
            if (gamephase == 6 && phasetime >= 3) { gamephase = 7; }



            //lesser update (stage 2)  50,72
            for (int i = 0; i < 150; i++)
            {
                if (lesserX[i] == NULL && lesserY[i] == NULL)
                {
                    continue;
                }
                lesserX[i] = lesserX[i] + deltatime * 100.f * cos(lesserDeg[i]);
                lesserY[i] = lesserY[i] + deltatime * 100.f * sin(lesserDeg[i]);
                lesser.setPosition(lesserX[i], lesserY[i]);
                lesserCooldown[i] = lesserCooldown[i] - deltatime;
                if (lesserCooldown[i] <= 0)
                {
                    for (int j = 0; j <= 4; j++)
                    {
                        bbulletx[bosstotalbullet] = lesserX[i] + 25;
                        bbullety[bosstotalbullet] = lesserY[i] + 36;
                        bdeg[bosstotalbullet] = (Pi * j)/5;
                        bvel[bosstotalbullet] = 0.1;
                        btype[bosstotalbullet] = 2;
                        bosstotalbullet++;
                    }
                    lesserCooldown[i] = 1;
                }
                if (lesserX[i] > screensizex || lesserX[i] < -100.f || lesserY[i]<-100.f || lesserY[i]>screensizey)
                {
                    lesserX[i] = NULL;
                    lesserY[i] = NULL;
                }
                if (lesserHealth[i] <= 0)
                {
                    score = score + 50;
                    explosionx[Explosionnow] = lesserX[i];
                    explosiony[Explosionnow] = lesserY[i];
                    ExplosionAnimation[Explosionnow] = 0;
                    ExplosionSize[Explosionnow] = 3;
                    lesserHealth[i]--;
                    Explosionnow++;
                    lesserX[i] = NULL;
                    lesserY[i] = NULL;
                }
                if (lesserX[i] == NULL && lesserY[i] == NULL)
                {
                    continue;
                }
                window.draw(lesser);
            }

            //lesserspawn
            if (gamephase == 7)
            {
                if (gamephase7timer >= 1)
                {
                    lesserHealth[lesserNow] = 5;
                    int random = rand() % 300;
                    lesserX[lesserNow] = -100.f;
                    lesserY[lesserNow] = random;
                    lesserDeg[lesserNow] = 0;
                    lesserCooldown[lesserNow] = 1;
                    lesserNow++;
                    lesserHealth[lesserNow] = 5;
                    random = rand() % 300;
                    lesserX[lesserNow] = screensizex;
                    lesserY[lesserNow] = random;
                    lesserDeg[lesserNow] = Pi;
                    lesserCooldown[lesserNow] = 1;
                    gamephase7timer = 0;
                    lesserNow++;
                }

            gamephase7timer = gamephase7timer + deltatime;
            }
            if (lesserNow > 100)
                lesserNow = 0;

            //boss
            if (gamephase == 3)
            {
                window.draw(boss);
                bosstotalbullet = bosstotalbullet % 1000;
                if (bosscooldown1 <= 0 && bosshealth < 500) //pink
                {
                    bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                    bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                    bdeg[bosstotalbullet] = degree1;
                    bvel[bosstotalbullet] = 0.1;
                    btype[bosstotalbullet] = 1;
                    bosstotalbullet++;
                    bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                    bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                    bdeg[bosstotalbullet] = Pi / 2 + degree1;
                    bvel[bosstotalbullet] = 0.1;
                    btype[bosstotalbullet] = 1;
                    bosstotalbullet++;
                    bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                    bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                    bdeg[bosstotalbullet] = Pi + degree1;
                    bvel[bosstotalbullet] = 0.1;
                    btype[bosstotalbullet] = 1;
                    bosstotalbullet++;
                    bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                    bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                    bdeg[bosstotalbullet] = 3 * Pi / 2 + degree1;
                    bvel[bosstotalbullet] = 0.1;
                    btype[bosstotalbullet] = 1;
                    bosstotalbullet++;
                    bosscooldown1 = 0.2;
                    degree1 = degree1 + Pi / 25 + 0.001;
                }
                else { bosscooldown1 = bosscooldown1 - deltatime; }

                if (bosscooldown2 <= 0)
                {
                    for (int i = 0; i <= 20; i++)
                    {
                        bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                        bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                        bdeg[bosstotalbullet] = i * Pi / 10 + degree2;
                        bvel[bosstotalbullet] = 0.1;
                        btype[bosstotalbullet] = 2;
                        bosstotalbullet++;
                    }
                    degree2 = degree2 + 0.1;
                    for (int i = 0; i <= 20; i++)
                    {
                        bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                        bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                        bdeg[bosstotalbullet] = i * Pi / 10 + degree2;
                        bvel[bosstotalbullet] = 0.085;
                        btype[bosstotalbullet] = 3;
                        bosstotalbullet++;
                    }
                    degree2 = degree2 + 0.1;
                    for (int i = 0; i <= 20; i++)
                    {
                        bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                        bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                        bdeg[bosstotalbullet] = i * Pi / 10 + degree2;
                        bvel[bosstotalbullet] = 0.07;
                        btype[bosstotalbullet] = 4;
                        bosstotalbullet++;
                    }
                    degree2 = degree2 + 0.1;
                    for (int i = 0; i <= 20; i++)
                    {
                        bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                        bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                        bdeg[bosstotalbullet] = i * Pi / 10 + degree2;
                        bvel[bosstotalbullet] = 0.055;
                        btype[bosstotalbullet] = 5;
                        bosstotalbullet++;
                    }
                    degree2 = degree2 + 0.1;
                    for (int i = 0; i <= 20; i++)
                    {
                        bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                        bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                        bdeg[bosstotalbullet] = i * Pi / 10 + degree2;
                        bvel[bosstotalbullet] = 0.04;
                        btype[bosstotalbullet] = 6;
                        bosstotalbullet++;
                    }
                    degree2 = degree2 + 0.1;
                    bosscooldown2 = 5;
                }
                else { bosscooldown2 = bosscooldown2 - deltatime; }
            }


            //explosion update
            if (Explosionnow >= 100)Explosionnow = 0;
            explosiontime = explosiontime + deltatime;
            if (explosiontime > 0.03)
            {
                explosiontime = 0;
                for (int i = 0; i < 110; i++)
                {
                    ExplosionAnimation[i]++;
                }
            }
            for (int i = 0; i < 110; i++) {
                if (ExplosionSize[i] == 1) {
                    switch (ExplosionAnimation[i]) {
                    case 0:smallExplosion.setTextureRect(sf::IntRect(0, 0, 36, 47)); break;
                    case 1:smallExplosion.setTextureRect(sf::IntRect(36, 0, 36, 47)); break;
                    case 2:smallExplosion.setTextureRect(sf::IntRect(71, 0, 34, 47)); break;
                    case 3:smallExplosion.setTextureRect(sf::IntRect(102, 0, 34, 47)); break;
                    case 4:smallExplosion.setTextureRect(sf::IntRect(137, 0, 34, 47)); break;
                    case 5:smallExplosion.setTextureRect(sf::IntRect(170, 0, 34, 47)); break;
                    case 6:smallExplosion.setTextureRect(sf::IntRect(204, 0, 34, 47)); break;
                    case 7:smallExplosion.setTextureRect(sf::IntRect(237, 0, 34, 47)); break;
                    case 8:smallExplosion.setTextureRect(sf::IntRect(270, 0, 34, 47)); break;
                    case 9:smallExplosion.setTextureRect(sf::IntRect(303, 0, 34, 47)); break;
                    case 10:smallExplosion.setTextureRect(sf::IntRect(336, 0, 34, 47)); break;
                    case 11:smallExplosion.setTextureRect(sf::IntRect(369, 0, 34, 47)); break;
                    case 12:smallExplosion.setTextureRect(sf::IntRect(402, 0, 34, 47)); break;
                    case 13:smallExplosion.setTextureRect(sf::IntRect(434, 0, 34, 47)); break;
                    case 14:smallExplosion.setTextureRect(sf::IntRect(466, 0, 34, 47)); break;
                    case 15:smallExplosion.setTextureRect(sf::IntRect(497, 0, 34, 47)); break;
                    case 16:smallExplosion.setTextureRect(sf::IntRect(527, 0, 33, 47)); break;
                    case 17:smallExplosion.setTextureRect(sf::IntRect(557, 0, 33, 47)); break;
                    case 18:smallExplosion.setTextureRect(sf::IntRect(0, 47, 33, 47)); break;
                    case 19:smallExplosion.setTextureRect(sf::IntRect(30, 47, 31, 47)); break;
                    case 20:smallExplosion.setTextureRect(sf::IntRect(58, 47, 29, 47)); break;
                    case 21:smallExplosion.setTextureRect(sf::IntRect(84, 47, 27, 47)); break;
                    case 22:smallExplosion.setTextureRect(sf::IntRect(109, 47, 26, 47)); break;
                    case 23:smallExplosion.setTextureRect(sf::IntRect(134, 47, 26, 47)); break;
                    case 24:smallExplosion.setTextureRect(sf::IntRect(159, 47, 26, 47)); break;
                    case 25:smallExplosion.setTextureRect(sf::IntRect(182, 47, 24, 47)); break;
                    case 26:smallExplosion.setTextureRect(sf::IntRect(204, 47, 24, 47)); break;
                    case 27:smallExplosion.setTextureRect(sf::IntRect(222, 47, 24, 47)); break;
                    case 28:smallExplosion.setPosition(NULL - 100, NULL - 100); explosionx[i] = NULL - 100; explosiony[i] = NULL - 100; break;
                    }
                    smallExplosion.setPosition(explosionx[i], explosiony[i]);
                    window.draw(smallExplosion);
                }
                if (ExplosionSize[i] == 3) {
                    switch (ExplosionAnimation[i])
                    {
                    case 0: bigExplosion.setTextureRect(sf::IntRect(0, 0, 85, 85)); break;
                    case 1: bigExplosion.setTextureRect(sf::IntRect(85, 0, 85, 85)); break;
                    case 2: bigExplosion.setTextureRect(sf::IntRect(170, 0, 76, 85)); break;
                    case 3: bigExplosion.setTextureRect(sf::IntRect(245, 0, 82, 85)); break;
                    case 4: bigExplosion.setTextureRect(sf::IntRect(325, 0, 84, 85)); break;
                    case 5: bigExplosion.setTextureRect(sf::IntRect(410, 0, 84, 85)); break;
                    case 6: bigExplosion.setTextureRect(sf::IntRect(492, 0, 84, 85)); break;
                    case 7: bigExplosion.setTextureRect(sf::IntRect(0, 85, 84, 85)); break;
                    case 8: bigExplosion.setTextureRect(sf::IntRect(83, 85, 80, 85)); break;
                    case 9: bigExplosion.setTextureRect(sf::IntRect(163, 85, 80, 85)); break;
                    case 10: bigExplosion.setTextureRect(sf::IntRect(243, 85, 80, 85)); break;
                    case 11: bigExplosion.setTextureRect(sf::IntRect(321, 85, 80, 85)); break;
                    case 12: bigExplosion.setTextureRect(sf::IntRect(400, 85, 80, 85)); break;
                    case 13: bigExplosion.setTextureRect(sf::IntRect(480, 85, 80, 85)); break;
                    case 14: bigExplosion.setTextureRect(sf::IntRect(1, 174, 80, 85)); break;
                    case 15: bigExplosion.setTextureRect(sf::IntRect(80, 174, 75, 85)); break;
                    case 16: bigExplosion.setTextureRect(sf::IntRect(153, 174, 73, 85)); break;
                    case 17: bigExplosion.setTextureRect(sf::IntRect(223, 174, 73, 85)); break;
                    case 18: bigExplosion.setTextureRect(sf::IntRect(293, 174, 73, 85)); break;
                    case 19: bigExplosion.setTextureRect(sf::IntRect(366, 174, 70, 85)); break;
                    case 20: bigExplosion.setTextureRect(sf::IntRect(433, 174, 65, 85)); break;
                    case 21: bigExplosion.setTextureRect(sf::IntRect(495, 174, 65, 85)); break;
                    case 22: bigExplosion.setTextureRect(sf::IntRect(0, 263, 65, 85)); break;
                    case 23: bigExplosion.setTextureRect(sf::IntRect(61, 263, 65, 85)); break;
                    case 24: bigExplosion.setTextureRect(sf::IntRect(120, 263, 65, 85)); break;
                    case 25: bigExplosion.setTextureRect(sf::IntRect(178, 263, 65, 85)); break;
                    case 26: bigExplosion.setTextureRect(sf::IntRect(232, 263, 65, 85)); break;
                    case 27: bigExplosion.setTextureRect(sf::IntRect(282, 263, 65, 85)); break;
                    case 28: bigExplosion.setPosition(NULL - 100, NULL - 100); explosionx[i] = NULL - 100; explosiony[i] = NULL - 100; break;
                    }
                    bigExplosion.setPosition(explosionx[i], explosiony[i]);
                    window.draw(bigExplosion);
                }

            }

            //player health
            window.draw(playerHealthCap);
            window.draw(playerHealthBase);
            for (int healthi = 1; healthi <= maxhealth*3; healthi++)
            {
                playerHealthEmpty.setPosition(5, 401 - 2 * healthi);
                window.draw(playerHealthEmpty);
            }
            for (int healthi = 1; healthi <= playerhealth*3; healthi++)
            {
                playerHealthHave.setPosition(5, 401 - 2 * healthi);
                window.draw(playerHealthHave);
            }

            //blank
            window.draw(blankCap);
            window.draw(blankBase);
            for (int blanki = 1; blanki <= maxblank*3; blanki++)
            {
                blankEmpty.setPosition(25, 401 - 2 * blanki);
                window.draw(blankEmpty);
            }
            for (int blanki = 1; blanki <= blank*3; blanki++)
            {
                blankHave.setPosition(25, 401 - 2 * blanki);
                window.draw(blankHave);
            }


            //boss health
            if (gamephase == 3)
            {
                window.draw(bossHealthCap);
                window.draw(bossHealthBase);
                for (int healthi = 1; healthi <= bossmaxhealth / 10; healthi++)
                {
                    bossHealthEmpty.setPosition(screensizex - 19, 401 - 2 * healthi);
                    window.draw(bossHealthEmpty);
                }
                for (int healthi = 1; healthi <= bosshealth / 10; healthi++)
                {
                    bossHealthHave.setPosition(screensizex - 19, 401 - 2 * healthi);
                    window.draw(bossHealthHave);
                }
            }

            //move
            window.display();
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) >= 50) && PlayerSprite.getPosition().x < screensizex - 40)
            {
                PlayerSprite.setPosition(PlayerSprite.getPosition().x + PlayerVelocity * deltatime * 1000,
                    PlayerSprite.getPosition().y);
                hitbox.setPosition(hitbox.getPosition().x + PlayerVelocity * deltatime * 1000,
                    hitbox.getPosition().y);
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) <= -50) && PlayerSprite.getPosition().x > 0)
            {
                PlayerSprite.setPosition(PlayerSprite.getPosition().x - PlayerVelocity * deltatime * 1000,
                    PlayerSprite.getPosition().y);
                hitbox.setPosition(hitbox.getPosition().x - PlayerVelocity * deltatime * 1000,
                    hitbox.getPosition().y);
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <= -50) && PlayerSprite.getPosition().y > 0)
            {
                PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                    PlayerSprite.getPosition().y - PlayerVelocity * deltatime * 1000);
                hitbox.setPosition(hitbox.getPosition().x,
                    hitbox.getPosition().y - PlayerVelocity * deltatime * 1000);
                playerAnimationTime = playerAnimationTime + deltatime;
                if (playerAnimationTime > 0.25)
                {
                    playerAnimationFrame++;
                    playerAnimationTime = 0;
                }
                if (playerAnimationFrame == 2)
                {
                    playerAnimationFrame = 0;
                }
                PlayerSprite.setTextureRect(sf::IntRect(playerAnimationFrame * 40 + 40, 0, 40, 37));
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >= 50) && PlayerSprite.getPosition().y < screensizey - 37)
            {
                PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                    PlayerSprite.getPosition().y + PlayerVelocity * deltatime * 1000);
                hitbox.setPosition(hitbox.getPosition().x,
                    hitbox.getPosition().y + PlayerVelocity * deltatime * 1000);
                PlayerSprite.setTextureRect(sf::IntRect(40, 0, 40, 37));
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::J) || sf::Joystick::isButtonPressed(0, 2)) && blank > 0 && blankcooldown <= 0)
            {
                for (int i = 0; i <= 5000; i++)
                {
                    bbulletx[i] = { NULL };
                    bbullety[i] = { NULL };
                }
                blank--;
                blankcooldown = 1000;
            }
            else { blankcooldown = blankcooldown - deltatime * 1000; }

            if (!((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <= -50) && PlayerSprite.getPosition().y > 0|| (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >= 50) && PlayerSprite.getPosition().y < screensizey - 37)))
                PlayerSprite.setTextureRect(sf::IntRect(0, 0, 40, 37));

            //level test
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                bulletlevel = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                bulletlevel = 2;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                bulletlevel = 3;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                bulletlevel = 4;



            //player bullet

            if (cooldown <= 0 && gamephase != 2 && gamephase != 4 && gamephase != 5 && gamephase != 0&&gamephase!=6)
            {
                totalbullet = totalbullet % 1000;
                if (bulletlevel == 1)
                {
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 9.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                }
                if (bulletlevel == 2)
                {
                    bulletx[totalbullet] = PlayerSprite.getPosition().x;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 19;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                }
                if (bulletlevel == 3)
                {
                    bulletx[totalbullet] = PlayerSprite.getPosition().x;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 19;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 9.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y - 10.f;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                }
                if (bulletlevel == 4)
                {
                    bulletx[totalbullet] = PlayerSprite.getPosition().x - 10.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 29.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x - 0.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y - 10.f;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 19.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y - 10.f;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                    bulletx[totalbullet] = PlayerSprite.getPosition().x + 9.5;
                    bullety[totalbullet] = PlayerSprite.getPosition().y - 20.f;
                    levelbullet[totalbullet] = bulletlevel;
                    totalbullet++;
                }
                cooldown = 100;
            }
            else
            {
                cooldown = cooldown - timeElapsed.asSeconds() * 1000;
            }

            //player hit with enemie
            if (enemie.getGlobalBounds().intersects(hitbox.getGlobalBounds())) {
                playerhealth--;
                enemie.setPosition(rand() % (screensizex-100), -100);
            }

            if (playerhealth <= 0)
            {
                stage.stop();
                bossmusic.stop();
                menu = 2;
            }
        }
    }
    return 0;
}