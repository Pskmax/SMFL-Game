#include<iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include<math.h>
using namespace std;
int main()
{
    int blank = 3, blankcooldown = 0;
    float bulletx[1500] = { NULL }, bullety[1500] = { NULL }, levelbullet[1500] = {NULL};
    float bbulletx[1500] = { NULL }, bbullety[1500] = { NULL }, bdeg[1500] = { NULL }, bvel[1500] = { NULL }, btype[1500] = {NULL};
    float degree1 = 0,degree2 = 0;
    int bosstotalbullet = 0;
    int totalbullet = 0;
    float cooldown = 0;
    float bosscooldown1 = 0;
    float bosscooldown2 = 0;
    int bossmaxhealth = 1000;
    int bosshealth = bossmaxhealth;
    int r = 0.2;
    int maxhealth = 10;
    int playerhealth = maxhealth;
    int gamephase = 1; // 1 for stage, 2 for boss
    const double Pi = 3.14159265358979323846;
    unsigned int screensizex = 480, screensizey = 650;

    /*
    //collision test
    float Random = rand() % 470;
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
    */


    //boss
    sf::Texture BossTexture;
    sf::RectangleShape boss;
    boss.setSize(sf::Vector2f(160.f, 182.f));
    boss.setTexture(&BossTexture);
    boss.setPosition({ screensizex / 2 - 80.f,0.f });
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
    sf::Text frameRate;
    frameRate.setCharacterSize(20);
    frameRate.setFont(font);
    frameRate.setPosition(10, 10);
    frameRate.setFillColor(sf::Color::White);
    
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
    playerHealthCap.setTextureRect(sf::IntRect(0, 0, 14, 3));
    playerHealthCap.setPosition(5, 398 - 2 * maxhealth);

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
    //clock
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        sf::Time timeElapsed = clock.restart();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        //map
        window.draw(Map1);
        window.draw(Map2);
        Map1.move(0.f, .1f);
        Map2.move(0.f, .1f);
        Mapy++;
        if (Mapy == 8520)
        {
            Map1.setPosition(0.f, 0.f);
            Map2.setPosition(0.f, -852.f);
            Mapy = 0;
        }
        window.draw(PlayerSprite);
        window.draw(hitbox);
        //fps
        frameRate.setString(std::to_string(1.0f / timeElapsed.asSeconds()));
        window.draw(frameRate);

        //bullet update
        for (int i = 0; i <= 1000; i++)
        {
            if (bulletx[i] == NULL && bullety[i] == NULL)
            {
                continue;
            }
            if (levelbullet[i] == 1)
            {
                BulletSprite.setTexture(BulletTexture);
                BulletSprite.setTextureRect(sf::IntRect(1,1,7,7));
            }
            if (levelbullet[i] == 2)
            {
                BulletSprite.setTexture(BulletTexture);
                BulletSprite.setTextureRect(sf::IntRect(109,1,11,11));
            }
            if (levelbullet[i] == 3)
            {
                BulletSprite.setTexture(BulletTexture);
                BulletSprite.setTextureRect(sf::IntRect(0, 12, 15, 15));
            }
            if (levelbullet[i] == 4)
            {
                BulletSprite.setTexture(BulletTexture);
                BulletSprite.setTextureRect(sf::IntRect(189, 14, 20, 15));
            }
            bullety[i] = bullety[i] - PBulletVelocity * timeElapsed.asSeconds()*1000;
            BulletSprite.setPosition(bulletx[i], bullety[i]);

            /*
            if (enemie.getGlobalBounds().intersects(BulletSprite.getGlobalBounds())) {
                colliy = 2839;
                bullety[i] = NULL;
                bulletx[i] = NULL;
                BulletSprite.setPosition(NULL, NULL);
            }
            */

            if (boss.getGlobalBounds().intersects(BulletSprite.getGlobalBounds())&&gamephase==2) {
                bosshealth=bosshealth - levelbullet[i];
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
            window.draw(BulletSprite);
        }
        for (int i = 0; i <= 1000; i++)
        {
            if (bbulletx[i] == NULL && bbullety[i] == NULL)
            {
                continue;
            }
            bbullety[i] = bbullety[i] + timeElapsed.asSeconds() * 1000 * bvel[i] * sin(bdeg[i]);
            bbulletx[i] = bbulletx[i] + timeElapsed.asSeconds() * 1000 * bvel[i] * cos(bdeg[i]);
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
            if (bbullety[i] < -20||bbullety[i]>screensizey||bbulletx[i]<-20||bbulletx[i]>screensizex){
                bbullety[i] = NULL;
                bbulletx[i] = NULL;
            }

        }

        /*
        //collision
        window.draw(enemie);
        enemie.move(0.f, timeElapsed.asMilliseconds()*.3f);
        colliy++;
        if (colliy == 2840)
        {
            Random = rand() % 470;
            enemie.setPosition(Random, -100.f);
            colliy = 0;
        }
        */

        //boss
        if (gamephase == 2) 
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
                bosscooldown1 = 30;
                degree1 = degree1 + Pi / 25 + 0.001;
            }
            else { bosscooldown1 = bosscooldown1 - 1; }

            if (bosscooldown2 <= 0)
            {
                for (int i = 0; i <= 20; i++)
                {
                    bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
                    bbullety[bosstotalbullet] = boss.getPosition().y + 160;
                    bdeg[bosstotalbullet] = i * Pi / 10+ degree2;
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
                bosscooldown2 = 1000;
            }
            else { bosscooldown2 = bosscooldown2 - 1; }
        }

        //player health
        window.draw(playerHealthCap);
        window.draw(playerHealthBase);
        for (int healthi = 1; healthi <= maxhealth; healthi++)
        {
            playerHealthEmpty.setPosition(5, 401 -2*healthi);
            window.draw(playerHealthEmpty);
        }
        for (int healthi = 1; healthi <= playerhealth; healthi++)
        {
            playerHealthHave.setPosition(5, 401 - 2*healthi);
            window.draw(playerHealthHave);
        }

        //boss health
        if (gamephase == 2) 
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && PlayerSprite.getPosition().x < screensizex-40)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x + PlayerVelocity * timeElapsed.asSeconds()*1000,
                PlayerSprite.getPosition().y);
            hitbox.setPosition(hitbox.getPosition().x + PlayerVelocity * timeElapsed.asSeconds() * 1000,
                hitbox.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && PlayerSprite.getPosition().x > 0)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x - PlayerVelocity * timeElapsed.asSeconds() * 1000,
                PlayerSprite.getPosition().y);
            hitbox.setPosition(hitbox.getPosition().x - PlayerVelocity * timeElapsed.asSeconds() * 1000,
                hitbox.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && PlayerSprite.getPosition().y > 0)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                PlayerSprite.getPosition().y - PlayerVelocity * timeElapsed.asSeconds() * 1000);
            hitbox.setPosition(hitbox.getPosition().x,
                hitbox.getPosition().y - PlayerVelocity * timeElapsed.asSeconds() * 1000);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && PlayerSprite.getPosition().y < screensizey - 37)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                PlayerSprite.getPosition().y + PlayerVelocity * timeElapsed.asSeconds() * 1000);
            hitbox.setPosition(hitbox.getPosition().x,
                hitbox.getPosition().y + PlayerVelocity * timeElapsed.asSeconds() * 1000);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)&&blank>0&&blankcooldown<=0)
        {
            for (int i = 0; i <= 1000; i++)
            {
                bbulletx[i] = { NULL };
                bbullety[i] = { NULL };
            }
            blank--;
            blankcooldown = 1000;
        }
        else { blankcooldown = blankcooldown - timeElapsed.asSeconds() * 1000; }



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

        if (cooldown <= 0)
        {
            totalbullet = totalbullet % 1000;
            if (bulletlevel == 1) 
            {
                bulletx[totalbullet] = PlayerSprite.getPosition().x + 16.5;
                bullety[totalbullet] = PlayerSprite.getPosition().y;
            }
            if (bulletlevel == 2)
            {
                bulletx[totalbullet] = PlayerSprite.getPosition().x + 14.5;
                bullety[totalbullet] = PlayerSprite.getPosition().y;
            }
            if (bulletlevel == 3)
            {
                bulletx[totalbullet] = PlayerSprite.getPosition().x + 12.5;
                bullety[totalbullet] = PlayerSprite.getPosition().y;
            }
            if (bulletlevel == 4)
            {
                bulletx[totalbullet] = PlayerSprite.getPosition().x + 9.5;
                bullety[totalbullet] = PlayerSprite.getPosition().y;
            }
            levelbullet[totalbullet] = bulletlevel;
            totalbullet++;
            cooldown = 250;
        }
        else
        {
            cooldown = cooldown - timeElapsed.asSeconds()*1000;
        }
        
        /*
        //player hit with enemie
        if (enemie.getGlobalBounds().intersects(hitbox.getGlobalBounds())) {
            cout << "Hit";
            PlayerSprite.setPosition(spawnPoint);
            hitbox.setPosition(hbspawnPoint);
        }
        */

        if (bosshealth <= 0)
        {
            window.close();
        }
        if (playerhealth <= 0)
        {
            window.close();
        }
    }
    return 0;
}