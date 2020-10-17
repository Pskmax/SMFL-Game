#include<iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include<math.h>
using namespace std;
int main()
{
    int blank=3,blankcooldown=0;
    float bulletx[1001] = { NULL }, bullety[1001] = { NULL };
    float bbulletx[1001] = { NULL }, bbullety[1001] = { NULL }, bdeg[1001] = {NULL};
    float degree1=0;
    int bosstotalbullet = 0;
    int totalbullet = 0;
    float cooldown = 0;
    float bosscooldown = 0;
    int bosshealth = 1000;
    int r = 0.2;
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
    BulletSprite.setTextureRect(sf::IntRect(1, 1, 7, 7));
    if (!BulletTexture.loadFromFile("resources/bullet.png"))
    {
        std::cout << "Load failed" << std::endl;
    }
    int PBulletVelocity = 1;

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
            bullety[i] = bullety[i] - PBulletVelocity * timeElapsed.asMilliseconds();
            BulletSprite.setPosition(bulletx[i], bullety[i]);

            /*
            if (enemie.getGlobalBounds().intersects(BulletSprite.getGlobalBounds())) {
                colliy = 2839;
                bullety[i] = NULL;
                bulletx[i] = NULL;
                BulletSprite.setPosition(NULL, NULL);
            }
            */

            if (boss.getGlobalBounds().intersects(BulletSprite.getGlobalBounds())) {
                bosshealth--;
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
            bbullety[i] = bbullety[i] + 0.2*sin(bdeg[i]);
            bbulletx[i] = bbulletx[i] + 0.2*cos(bdeg[i]);
            BB.setPosition(bbulletx[i], bbullety[i]);
            window.draw(BB);
            
            //player hit bullet
            if (hitbox.getGlobalBounds().intersects(BB.getGlobalBounds())) {
                std::cout << "Hit" << endl;
                PlayerSprite.setPosition(spawnPoint);
                hitbox.setPosition(hbspawnPoint);
                BB.setPosition(NULL, NULL);
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
        window.draw(boss);
        if (bosscooldown <= 0)
        {
            bosstotalbullet = bosstotalbullet % 1000;
            bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
            bbullety[bosstotalbullet] = boss.getPosition().y + 160;
            bdeg[bosstotalbullet] = degree1;
            bosstotalbullet++;
            bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
            bbullety[bosstotalbullet] = boss.getPosition().y + 160;
            bdeg[bosstotalbullet] = -degree1;
            bosstotalbullet++;
            bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
            bbullety[bosstotalbullet] = boss.getPosition().y + 160;
            bdeg[bosstotalbullet] = Pi+degree1;
            bosstotalbullet++;
            bbulletx[bosstotalbullet] = boss.getPosition().x + 70;
            bbullety[bosstotalbullet] = boss.getPosition().y + 160;
            bdeg[bosstotalbullet] = Pi-degree1;
            bosstotalbullet++;
            bosscooldown = 15;
            degree1 = degree1 + Pi/30;
        }
        else{bosscooldown=bosscooldown-1;}



        //move
        window.display();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && PlayerSprite.getPosition().x < screensizex - 40)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x + PlayerVelocity * timeElapsed.asMilliseconds(),
                PlayerSprite.getPosition().y);
            hitbox.setPosition(hitbox.getPosition().x + PlayerVelocity * timeElapsed.asMilliseconds(),
                hitbox.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && PlayerSprite.getPosition().x > 0)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x - PlayerVelocity * timeElapsed.asMilliseconds(),
                PlayerSprite.getPosition().y);
            hitbox.setPosition(hitbox.getPosition().x - PlayerVelocity * timeElapsed.asMilliseconds(),
                hitbox.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && PlayerSprite.getPosition().y > 0)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                PlayerSprite.getPosition().y - PlayerVelocity * timeElapsed.asMilliseconds());
            hitbox.setPosition(hitbox.getPosition().x,
                hitbox.getPosition().y - PlayerVelocity * timeElapsed.asMilliseconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && PlayerSprite.getPosition().y < screensizey - 37)
        {
            PlayerSprite.setPosition(PlayerSprite.getPosition().x,
                PlayerSprite.getPosition().y + PlayerVelocity * timeElapsed.asMilliseconds());
            hitbox.setPosition(hitbox.getPosition().x,
                hitbox.getPosition().y + PlayerVelocity * timeElapsed.asMilliseconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)&&blank>0&&blankcooldown<=0)
        {
            for (int i = 0; i <= 1000; i++)
            {
                bbulletx[i] = { NULL };
                bbullety[i] = { NULL };
            }
            blank--;
            blankcooldown = 1000;
        }
        else { blankcooldown = blankcooldown - timeElapsed.asMilliseconds(); }

        //player bullet

        if (cooldown <= 0)
        {
            totalbullet = totalbullet % 1000;
            bulletx[totalbullet] = PlayerSprite.getPosition().x+15;
            bullety[totalbullet] = PlayerSprite.getPosition().y;
            totalbullet++;
            cooldown = 15;
        }
        else
        {
            cooldown=cooldown-1;
        }
        
        /*
        //player hit with enemie
        if (enemie.getGlobalBounds().intersects(hitbox.getGlobalBounds())) {
            cout << "Hit";
            PlayerSprite.setPosition(spawnPoint);
            hitbox.setPosition(hbspawnPoint);
        }
        */

        if (bosshealth == 0)
        {
            window.close();
        }
    }
    return 0;
}