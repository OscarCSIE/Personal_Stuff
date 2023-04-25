#include<iostream>
#include<cstdlib>
#include<ctime>
#include<chrono>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>

// The Ball class represents the shuttlecock in the game
class Ball{
private:
    float x; // x position of the ball
    float y; // y position of the ball
    float vx; // x velocity of the ball
    float vy; // y velocity of the ball
    float gravity; // gravitational acceleration
public:
    Ball(float x0, float y0, float vx0, float vy0, float g){
        x = x0;
        y = y0;
        vx = vx0;
        vy = vy0;
        gravity = g;
    }
    void update(float dt){
        x += vx * dt;
        y += vy * dt;
        vy += gravity * dt;
    }
    void bounceOffWall(float width){
        if (x < 0 || x > width){
            vx = -vx;
        }
    }
    void bounceOffGround(float groundHeight){
        if (y < groundHeight){
            y = groundHeight;
            vy = -vy;
        }
    }
    float getX() const{
        return x;
    }
    float getY() const{
        return y;
    }
};


// The Player class represents a player in the game
class Player{
private:
    float x; // x position of the player
    float y; // y position of the player
    float speed; // movement speed of the player
public:
    Player(float x0, float y0, float s){
        x = x0;
        y = y0;
        speed = s;
    }
    void moveLeft(float dt){
        x -= speed * dt;
    }
    void moveRight(float dt){
        x += speed * dt;
    }
    void jump(float vy0){
        y += vy0;
    }
    float getX() const{
        return x;
    }
    float getY() const{
        return y;
    }
};

// The Game class represents the overall game state
class Game{
private:
    Ball ball; // the shuttlecock
    Player player1; // the first player
    Player player2; // the second player
    float width; // width of the game screen
    float groundHeight; // height of the ground
    Mix_Chunk* hitSound; // sound effect for hitting the ball
    Mix_Chunk* missSound; // sound effect for missing the ball
    Mix_Chunk* scoreSound; // sound effect for scoring
public:
    Game(float w, float gh, Mix_Chunk* hit, Mix_Chunk* miss, Mix_Chunk* score) : 
        ball(w / 2, gh + 10, 0, 0, 9.8), player1(w / 4, gh + 10, 100), player2(w * 3 / 4, gh + 10, 100){
        width = w;
        groundHeight = gh;
        hitSound = hit;
        missSound = miss;
        scoreSound = score;
    }
    void update(float dt){
        ball.update(dt);
        ball.bounceOffWall(width);
        ball.bounceOffGround(groundHeight);
        // check for collisions with players here
        // update player positions here
    }
    void playHitSound(){
        Mix_PlayChannel(-1, hitSound, 0);
    }
    void playMissSound(){
        Mix_PlayChannel(-1, missSound, 0);
    }
    void playScoreSound(){
        Mix_PlayChannel(-1, scoreSound, 0);
    }
};


float getTime(){
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0f;
}

int main(int argc, char* argv[]){
    const float SCREEN_WIDTH = 800;
    const float GROUND_HEIGHT = 50;
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Chunk* hitSound = Mix_LoadWAV("hit.wav");
    Mix_Chunk* missSound = Mix_LoadWAV("miss.wav");
    Mix_Chunk* scoreSound = Mix_LoadWAV("score.wav");

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Badminton Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    Game game(SCREEN_WIDTH, GROUND_HEIGHT);

    float lastTime = getTime();
    float totalTime = 0;
    while(totalTime < 30){
        float currentTime = getTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        game.update(deltaTime);

        // clear the screen with black color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw game graphics
        // ...
        // use SDL_Renderer functions to draw the game graphics

        SDL_RenderPresent(renderer);

        totalTime += deltaTime;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
//Download the SDL development libraries from the official SDL website : https://www.libsdl.org/download-2.0.php. 
//Make sure to choose the correct version for your platform and Visual Studio version.
//Extract the contents of the downloaded archive to a folder on your computer(e.g.C:\SDL2).
//In Visual Studio, create a new C++ project.
//Right - click on the project in the Solution Explorer, and select "Properties".
//In the properties window, go to "VC++ Directories" and add the SDL include directory(e.g.C:\SDL2\include) 
// to "Include Directories", and the SDL lib directory(e.g.C:\SDL2\lib) to "Library Directories".
//Still in the properties window, go to "Linker > Input" and add the following libraries 
//to "Additional Dependencies" : SDL2.lib, SDL2main.lib.