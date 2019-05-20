
#include <SFML/Window.hpp>

#include "Core/common.h"


#define WIDTH 800
#define HEIGHT 800

float gameSpeed = 700;

//gameplay scene
Scene * gameScene = nullptr;

//ball paddles
Mesh paddleLeft, paddleRight;

//actual ball
struct GameBall {
    Mesh ball;
    glm::vec2 velocity;
    unsigned int hits = 0;
};

GameBall gameBall;

void updateGame(float delta);

bool ballHitPaddle(Mesh &paddle);

int main() {
    sf::ContextSettings ctxSettings;
    ctxSettings.majorVersion = 3;
    ctxSettings.minorVersion = 3;
    ctxSettings.depthBits = 24;
    ctxSettings.antialiasingLevel = 0;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "2D Demo", sf::Style::Default, ctxSettings);

    Log::setFilter(true, LogType::Error);
    initCore();

    gameScene = new Scene(WIDTH, HEIGHT);

    OrthographicCamera orthoCamera(glm::vec3(WIDTH * 0.5, HEIGHT * 0.5, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    orthoCamera.setClipping(0.0, 100.0);
    orthoCamera.setSize(WIDTH, HEIGHT);


    FlatRenderer renderer(WIDTH, HEIGHT, orthoCamera);
    gameScene->addSubScene(renderer, Size(1, 1));

    gameBall.ball = pd::circle(20.0, 20, glm::vec4(1.0, 0.7, 0.0, 1.0));
    paddleLeft = pd::rectangle(50, 250, glm::vec4(0.4, 0.4, 1.0, 1.0)); 
    paddleRight = pd::rectangle(paddleLeft.getSize().width, paddleLeft.getSize().height, glm::vec4(0.4, 0.4, 1.0, 1.0));

    gameBall.ball.translate(WIDTH * 0.5, HEIGHT * 0.5, 1.0);
    gameScene->addObject(gameBall.ball);

    paddleLeft.translate(paddleLeft.getSize().width * 0.5, HEIGHT * 0.5, 1.0);
    gameScene->addObject(paddleLeft);

    paddleRight.translate(WIDTH - paddleRight.getSize().width * 0.5, HEIGHT * 0.5, 1.0);
    gameScene->addObject(paddleRight);

    PointGenerator2D pointParticles(10000);
    pointParticles.translate(0.5 * WIDTH, 0.5 * HEIGHT, 1.0);
    pointParticles.scale(WIDTH * 0.5, HEIGHT * 0.5, 1.0);
    gameScene->addObject(pointParticles);

    Texture billboardTexture(ResourceManager::GetPath("/Assets/smoke.png").c_str());
    BillboardGenerator2D billboardParticles(10);
    billboardParticles.translate(0.5 * WIDTH, 0.5 * HEIGHT, 1.0);
    billboardParticles.addTexture(billboardTexture, TextureType::Diffuse);
    billboardParticles.setLifeTime(3.0f);
    gameScene->addObject(billboardParticles);

    Mesh graph;
    graph.settings.lineThickness = 2;
    graph.settings.drawType = DrawType::LINEG;
    graph.line(glm::vec3(0, 0, 1), glm::vec3(200, 0, 1));
    graph.lineTo(glm::vec3(300, 100, 1));
    graph.lineTo(glm::vec3(200, 200, 1));
    gameScene->addObject(graph);

    sf::Clock sfClock;
    sf::Time deltaTime = sfClock.getElapsedTime();
    float delta;
    while (window.isOpen())
    {   
        gameScene->clear(0.3, 0.3, 0.3, 1.0);

        updateGame(delta);

        pointParticles.update(delta * 0.01);
        billboardParticles.update(delta);

        gameScene->draw(delta);
        window.display();

        sf::Event e;
        while (window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if(e.type == sf::Event::Resized) {
                sf::Vector2u size = window.getSize();
                gameScene->resize(size.x, size.y);
                orthoCamera.setSize(size.x, size.y);
                orthoCamera.translate(0.5 * gameScene->getSize().width, 0.5 * gameScene->getSize().height, 0.0);
                paddleLeft.translate(paddleLeft.getSize().width * 0.5, gameScene->getSize().height * 0.5, 1.0);
                paddleRight.translate(gameScene->getSize().width - paddleRight.getSize().width * 0.5, gameScene->getSize().height * 0.5, 1.0);
            }
        }
        
        delta = (sfClock.getElapsedTime() - deltaTime).asMilliseconds() * 0.001f;
        deltaTime = sfClock.getElapsedTime();
    }
    

    return 0;
}

void updateGame(float delta) {
    Size sceneSize = gameScene->getSize();
    glm::vec3 ballPos = gameBall.ball.getPosition();
    Size ballSize = gameBall.ball.getSize();

    //redirect ball from wall
    if(ballPos.y > sceneSize.height - ballSize.height * 0.5) {
        gameBall.velocity.y = -gameSpeed;
    }
    else if(ballPos.y < ballSize.height * 0.5) {
        gameBall.velocity.y = gameSpeed;
    }

    //check if player scored
    if(ballPos.x < ballSize.width * 0.5 || ballPos.x > sceneSize.width - ballSize.width * 0.5) {
        gameBall.velocity = glm::vec2(0.0);
        gameBall.ball.translate(0.5 * sceneSize.width, 0.5 * sceneSize.height, 1.0);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        gameBall.velocity = glm::vec2(gameSpeed, gameSpeed);
        gameBall.hits = 0;
    }

    //move left paddle
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        paddleLeft.translate(paddleLeft.getTranslation() + glm::vec3(0.0, -gameSpeed, 0.0) * delta);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        paddleLeft.translate(paddleLeft.getTranslation() + glm::vec3(0.0, gameSpeed, 0.0) * delta);
    }

    //move right paddle
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        paddleRight.translate(paddleRight.getTranslation() + glm::vec3(0.0, -gameSpeed, 0.0) * delta);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        paddleRight.translate(paddleRight.getTranslation() + glm::vec3(0.0, gameSpeed, 0.0) * delta);
    }

    if(std::signbit(gameBall.velocity.x) && ballHitPaddle(paddleLeft)) {
        gameBall.hits++;
        gameBall.velocity.x = std::min(gameSpeed + gameBall.hits * 1000, 5000.0f);
        printf("Speed %f\n", abs(gameBall.velocity.x));
    }
    else if(!std::signbit(gameBall.velocity.x) && ballHitPaddle(paddleRight)) {
        gameBall.hits++;
        gameBall.velocity.x = -std::min(gameSpeed + gameBall.hits * 1000, 5000.0f);
        printf("Speed %f\n", abs(gameBall.velocity.x));
    }

    paddleLeft.translate(
        paddleLeft.getTranslation().x,
        ballPos.y,
        paddleLeft.getTranslation().z
    );

    paddleRight.translate(
        paddleRight.getTranslation().x,
        ballPos.y,
        paddleRight.getTranslation().z
    );



    //check if ball hit left paddle

    //apply velocity
    gameBall.ball.translate(gameBall.ball.getTranslation() + glm::vec3(gameBall.velocity, 0.0) * delta);



}

bool ballHitPaddle(Mesh &paddle) {
    glm::vec3 paddlePos = paddle.getTranslation();
    glm::vec3 ballPos = gameBall.ball.getTranslation();
    Size paddleSize = paddle.getSize();

    glm::vec2 leftBottom, rightTop;
    leftBottom = glm::vec2(
        paddlePos.x - paddleSize.width * 0.5,
        paddlePos.y + paddleSize.height * 0.5
    );
    rightTop = glm::vec2(
        paddlePos.x + paddleSize.width * 0.5,
        paddlePos.y - paddleSize.height * 0.5
    );

    glm::vec2 nearestRectPoint = glm::vec2(
        std::max(std::min(ballPos.x, rightTop.x), leftBottom.x),
        std::max(std::min(ballPos.y, leftBottom.y), rightTop.y)
    );

    return glm::length(glm::vec3(nearestRectPoint, ballPos.z) - ballPos) < gameBall.ball.getSize().width * 0.5;
}