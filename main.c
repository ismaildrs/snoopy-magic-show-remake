//---------------------This code was created by: Ismail Drissi-------------------------



//-------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
//
#include "raylib.h"

//
#define CELLS 8

//
const int screen_size = 800;

//
const int cell_size = screen_size/CELLS;

//!Structures:

//--Position-
typedef struct Pos{
    int i;
    int j;
}Pos;

//--Game--
typedef struct Game{
    int grid[CELLS][CELLS];
    Pos snoopy;
    int snoopy_dir;
    Pos ball;
    int ball_dir_x;
    int ball_dir_y;
    int score;
    char* password;
}Game;

//!Enum:
enum caracters {Blank, Snoopy, Ball, Bloc1, Bloc2, Bird};

enum dir {Left,_, Right, __, Up, ___, Down};

//!Functions Prototype:

void initLevel();
int validMove(Pos move);
void userInput();
void snoopyInput();
void showBord(Texture2D*, Texture2D, Texture2D, Texture2D, int, int);
void moveBall();
void showBall(Texture2D);
int snoopyAndBall();
void snoopyAndBird(Pos);
void gameState();

//!Golbal Var:
Game game;

int game_start=0;
int gameOver=0;
int win=0;
int pause = 0;
int nbr_of_games = 0;

//
int main(){
    
    int a  = 0, b=0;
    double startTime = GetTime();
    double elapsedTime = 0.0;
    //
    InitWindow(screen_size, screen_size, "Snoopy: Magic Show - Ismail Drissi");

    //
    InitAudioDevice();

    //
    Music game_music = LoadMusicStream("music/game_music.mp3");
    Music menu_music = LoadMusicStream("music/menu_music.mp3");
    Sound losing_game = LoadSound("music/losing_sound.ogg");
    Sound win_game = LoadSound("music/win_sound.ogg");

    //
    Font customFont = LoadFont("fonts/PixelGameFont.ttf");
    //
    Image ball_image = LoadImage("assets/ball.png");
    Image snoopy_image1 = LoadImage("assets/snoopy_walk1_l.png");
    Image snoopy_image2 = LoadImage("assets/snoopy_walk2_l.png");
    Image snoopy_image3 = LoadImage("assets/snoopy_walk1_r.png");
    Image snoopy_image4 = LoadImage("assets/snoopy_walk2_r.png");
    Image snoopy_image5 = LoadImage("assets/snoopy_up1.png");
    Image snoopy_image6 = LoadImage("assets/snoopy_up2.png");
    Image bird_image = LoadImage("assets/bird.png");
    Image bloc1_image = LoadImage("assets/obstacle1.png");
    Image bloc2_image = LoadImage("assets/obstacle2.png");

    ImageResize(&snoopy_image1, cell_size, cell_size);
    ImageResize(&snoopy_image2, cell_size, cell_size);
    ImageResize(&snoopy_image3, cell_size, cell_size);
    ImageResize(&snoopy_image4, cell_size, cell_size);
    ImageResize(&snoopy_image5, cell_size, cell_size);
    ImageResize(&snoopy_image6, cell_size, cell_size);
    ImageResize(&bird_image, cell_size, cell_size);
    ImageResize(&bloc1_image, cell_size, cell_size);
    ImageResize(&bloc2_image, cell_size, cell_size);
    ImageResize(&ball_image, cell_size*0.45, cell_size*0.45);

    Texture2D snoopy_txts[] = { 
                                LoadTextureFromImage(snoopy_image1), 
                                LoadTextureFromImage(snoopy_image2), 
                                LoadTextureFromImage(snoopy_image3),  
                                LoadTextureFromImage(snoopy_image4),
                                LoadTextureFromImage(snoopy_image5),
                                LoadTextureFromImage(snoopy_image6),
                            };
    Texture2D bird_txt = LoadTextureFromImage(bird_image);
    Texture2D bloc1_txt = LoadTextureFromImage(bloc1_image);
    Texture2D bloc2_txt = LoadTextureFromImage(bloc2_image);
    Texture2D ball_txt = LoadTextureFromImage(ball_image);
    
    UnloadImage(snoopy_image1);
    UnloadImage(snoopy_image2);
    UnloadImage(snoopy_image3);
    UnloadImage(snoopy_image4);
    UnloadImage(snoopy_image5);
    UnloadImage(snoopy_image6);
    UnloadImage(bird_image);
    UnloadImage(bloc1_image);
    UnloadImage(bloc2_image);

    //
    PlayMusicStream(game_music);
    PlayMusicStream(menu_music);
    //
    SetTargetFPS(120);

    //
    initLevel();

    //game loop
    while(!WindowShouldClose()){

        elapsedTime = GetTime()-startTime;
        //
        //
        BeginDrawing();

            //
            ClearBackground(RAYWHITE);
            //
            userInput();
            if(game_start){
                for(int i=0; i<CELLS; i++){
                for(int j =0; j<CELLS; j++){
                    DrawRectangleLines(i*cell_size, j*cell_size, cell_size, cell_size, BLACK);
                    showBord(snoopy_txts, bloc1_txt, bloc2_txt, bird_txt, game.snoopy_dir, a);
                    // DrawTexture(game.grid[i][j].element.sprite, game.grid[i][j].j* cell_size, game.grid[i][j].i * cell_size, WHITE);
                    }
                }
                if(!gameOver && !win && !pause){
                    gameState();
                    UpdateMusicStream(game_music);
                    if(elapsedTime>0.15){
                        snoopyInput();
                        startTime = GetTime();
                        //

                        moveBall();
                        a = !a;
                    }
                }
                else if (gameOver == 1 && b==0){
                    game_start = 0;
                    PlaySound(losing_game);
                    b=1;
                }
                else if(win == 1 && b==0){
                    game_start = 0;
                    PlaySound(win_game);
                    b=1;
                }
                if((gameOver || win )&& elapsedTime>5) game_start = 0;
                showBall(ball_txt);
            }
            else{
                if(!nbr_of_games) UpdateMusicStream(menu_music);
                ClearBackground(RAYWHITE);
                DrawTextEx(customFont, "SNOOPY", (Vector2){130, 100}, 150, 2, GRAY);
                DrawTextEx(customFont, "SNOOPY", (Vector2){130, 90}, 150, 2, BLACK);
                DrawTextEx(customFont, "-MAGIC  SHOW-", (Vector2){210, 255}, 50, 2, GRAY);
                DrawTextEx(customFont, "-MAGIC  SHOW-", (Vector2){210, 250}, 50, 2, BLACK);
                DrawTextEx(customFont, "GAME START", (Vector2){220, 600}, 50, 2, GRAY);
                DrawTextEx(customFont, "GAME START", (Vector2){220, 605}, 50, 2, BLACK);
                DrawTextEx(customFont, "ISMAIL DRISSI", (Vector2){290, 700}, 30, 2, BLUE);
                DrawTextEx(customFont, "ISMAIL DRISSI", (Vector2){290, 702}, 30, 2, DARKBLUE);
                Rectangle rect = {0,0,800,800};
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                // Perform actions when the left mouse button is pressed
                    if (CheckCollisionPointRec(GetMousePosition(), rect)){
                        initLevel();
                        game_start = 1;
                        b=0;
                        nbr_of_games++;
                }
            }
            }
        //
        EndDrawing();
        
        
    }
    //
    StopMusicStream(game_music);

    //
    CloseWindow();

    return 0;
}

//!Functions:

//initializer le niveau;
void initLevel(){
    gameOver = 0;
    win = 0;
    game.score = 0;

    Pos bird[2][2] = {(Pos){.i = 0, .j = 0}, (Pos){.i = CELLS-1, .j = CELLS-1}, (Pos){.i = 0, .j = CELLS - 1}, (Pos){.i = CELLS-1, .j = 0}};
    
    Pos blocs1[2][2] = {(Pos){.i = 4, .j = 2}, (Pos){.i = 4, .j = 3}, (Pos){.i = 4, .j = 4}, (Pos){.i = 4, .j = 5}};

    Pos blocs2[2][2] = {(Pos){.i = 1, .j = 1}, (Pos){.i = CELLS-2, .j = CELLS-2}, (Pos){.i = 1, .j = CELLS - 2}, (Pos){.i = CELLS-2, .j = 1}};

    game.ball = (Pos){.i = 4*cell_size,.j = 0};

    game.snoopy = (Pos){.i = 3, .j =4};

    game.grid[game.snoopy.i][game.snoopy.j] = Snoopy;

    game.grid[game.ball.i][game.ball.j] = Ball;

    game.snoopy_dir = Left;

    game.ball_dir_x = Right;

    game.ball_dir_y = Down;

    for(int i =0; i< 2; i++){
        for(int j=0; j<2; j++){
            game.grid[bird[i][j].i][bird[i][j].j] = Bird;
            game.grid[blocs1[i][j].i][blocs1[i][j].j] = Bloc1;
            game.grid[blocs2[i][j].i][blocs2[i][j].j] = Bloc2;
        }
    }
}

//show game board
void showBord(Texture2D* snoopy_txt, Texture2D bloc1_txt, Texture2D bloc2_txt, Texture2D bird_txt, int dir, int a){
    Texture2D needed_img;
    for(int i=0; i<CELLS; i++){
        for(int j =0; j<CELLS; j++){
            switch (game.grid[i][j])
            {
            case Snoopy:
                needed_img = snoopy_txt[dir+a];
                break;
            case Bloc1:
                needed_img = bloc1_txt;
                break;
            case Bloc2:
                needed_img = bloc2_txt;
                break;
            case Bird:
                needed_img = bird_txt;
                break;
            default:
                needed_img = LoadTexture("");
                break;
            }
            DrawTexture(needed_img,i*cell_size, j*cell_size, WHITE);
        }
    }
}

//check if the move is valid
int validMove(Pos move){
    if(game.grid[move.i][move.j] == Bloc1 || game.grid[move.i][move.j] == Bloc2) 
        return 0;
    return 1;
}

//check the user input and apply it to snoopy
void snoopyInput(){
    Pos wanted_move;
    if(IsKeyDown(KEY_RIGHT)){
        if(game.snoopy.i<7){
            wanted_move = (Pos){.i = game.snoopy.i +1, game.snoopy.j}; 
            if(validMove(wanted_move)){
                snoopyAndBird(wanted_move);
                game.grid[game.snoopy.i][game.snoopy.j] = Blank;
                game.grid[++game.snoopy.i][game.snoopy.j] = Snoopy;
                game.snoopy_dir = Right;
            }
        }
    }
    else if(IsKeyDown(KEY_LEFT)){
        if(game.snoopy.i>0){
            wanted_move = (Pos){.i = game.snoopy.i -1, game.snoopy.j}; 
            if(validMove(wanted_move)){
                snoopyAndBird(wanted_move);
                game.grid[game.snoopy.i][game.snoopy.j] = Blank;
                game.grid[--game.snoopy.i][game.snoopy.j] = Snoopy;
                game.snoopy_dir = Left;
            }
        }
    }
    else if(IsKeyDown(KEY_UP)){
        if(game.snoopy.j>0){
            wanted_move = (Pos){.i = game.snoopy.i, game.snoopy.j-1}; 
            if(validMove(wanted_move)){
                snoopyAndBird(wanted_move);
                game.grid[game.snoopy.i][game.snoopy.j] = Blank;
                game.grid[game.snoopy.i][--game.snoopy.j] = Snoopy;
                game.snoopy_dir = Up;
            }
        }
    }
    else if(IsKeyDown(KEY_DOWN)){
        if(game.snoopy.j<7){
            wanted_move = (Pos){.i = game.snoopy.i, game.snoopy.j+1}; 
            if(validMove(wanted_move)){
                snoopyAndBird(wanted_move);
                game.grid[game.snoopy.i][game.snoopy.j] = Blank;
                game.grid[game.snoopy.i][++game.snoopy.j] = Snoopy;
                game.snoopy_dir = Left;
            }
        }
    }
}


//Moves the ball around
void moveBall(){
    Pos move1 = (Pos){.i = (game.ball.i +(cell_size*0.45)/2)/cell_size, .j = (game.ball.j + (cell_size*0.45) +25)/cell_size};
    Pos move2 = (Pos){.i = (game.ball.i +(cell_size*0.45)/2)/cell_size, .j = (game.ball.j -25)/cell_size};
    Pos move3 = (Pos){.i = (game.ball.i +cell_size*0.45+25)/cell_size, .j = (game.ball.j+(cell_size*0.45)/2)/cell_size};
    Pos move4 = (Pos){.i = (game.ball.i-25)/cell_size, .j = (game.ball.j + (cell_size*0.45)/2)/cell_size};
    if(game.ball_dir_y == Down){
        if(game.ball.j<screen_size-cell_size*0.45 && validMove(move1)) game.ball.j += 40;
        else game.ball_dir_y = Up;
    }
    else if(game.ball_dir_y == Up ){
        if(game.ball.j>0&&validMove(move2)) game.ball.j -= 40;
        else game.ball_dir_y = Down;
    }
    if(game.ball_dir_x == Right){
        if(game.ball.i<screen_size-cell_size*0.45 && validMove(move3)) game.ball.i+= 40;
        else game.ball_dir_x = Left;
    }
    else if(game.ball_dir_x == Left){
        if(game.ball.i>0 && validMove(move4)) game.ball.i-= 40;
        else game.ball_dir_x = Right;
    }
}

//Show the ball
void showBall(Texture2D ball_txt){
    DrawTexture(ball_txt, game.ball.i, game.ball.j, WHITE);
}

//Detect if the ball and snoopy collide
int snoopyAndBall(){
    if(game.snoopy.i == (game.ball.i)/cell_size && game.snoopy.j == (game.ball.j)/cell_size)
        return 1;
    return 0;
}

//Whenever Snoopy and the birds collide we increase the score
void snoopyAndBird(Pos move){
    if(game.grid[move.i][move.j] == Bird) game.score++;
}

//check wheter the game is over or its a win;
void gameState(){
    if(snoopyAndBall()) gameOver = 1;
    if(game.score==4) win=1;
}

//
void userInput(){
    if(IsKeyDown(KEY_SPACE)){
        pause = !pause;
    }
}