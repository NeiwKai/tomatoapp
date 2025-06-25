#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <raylib.h>
#include <raygui.h>

#define WIDTH 400
#define HEIGHT 400
#define maxINPUT 8

typedef struct POMODORO {
  int tomatoes;
  int PEICE;
  int SMALLREST;
  int BIGREST;
  int state;
} POMODORO;

typedef struct button {
  Rectangle rect;
  bool state;
} button;

POMODORO P;

bool FINISH = false;

int* makeitint(char input[]);
int makeitsec(int input[]);
// state
void getUserInput();
void pomodoro();

int main(){
  P.state = 0;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
  SetWindowMinSize(WIDTH, HEIGHT);
  InitWindow(WIDTH, HEIGHT, "tomato");
  SetTargetFPS(60);
  char textInput[20];

  while (!WindowShouldClose()) {
   switch(P.state) {
      case 0: {
        getUserInput();
        P.state++;
        break;
      }
      case 1: {
        pomodoro();
        P.state++;
        break;
      }
      case 2: {
        BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawText("CONGRATS :D", (GetScreenWidth()/2.0f)-10, GetScreenWidth()*0.5f, 50, RED);
        EndDrawing();
        break;
      }
    }
  }
  CloseWindow(); 
  return 0;
}

int* makeitint(char input[]){ 
  int *buffer = (int *) malloc(sizeof(int));
  char *token = strtok(input, ":"); 
  int i=0;
  while (token != NULL){
    buffer[i] = atoi(token);
    token = strtok(NULL, ":");
    i++;
  }
  free(token);
  return buffer;
}

int makeitsec(int input[]) {
  int h = input[0];
  int m = input[1];
  int s = input[2]; 
  return h*3600 + m*60 + s;
}

// Save the stat
void CloseThatWindow() {
  CloseWindow();
}

void getUserInput() {
  Rectangle textBox =  {(GetScreenWidth()/2.0f)-(255/2), GetScreenHeight()*0.45f, 255, 50};
  char *input = (char *)calloc(sizeof(char), (maxINPUT+1));
  char *text = (char *)calloc(sizeof(char), 255);
  text = "Please input your loop";

  int *peice_time;
  int *smallrest_time;
  int *bigrest_time;

  int enterCount = 0;

  while (!WindowShouldClose()) {

    BeginDrawing();
      ClearBackground(RAYWHITE);
      GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
      int result = GuiTextInputBox((Rectangle){(GetScreenWidth()/2.0f)-(255/2), GetScreenHeight()*0.5f-(255/2), 255, 255}, "Input", text, "Ok;Cancel", input, 255, NULL);
      if (result == 1) {
        switch (enterCount) {
          case 0:
            P.tomatoes = atoi(input);
            enterCount++;
            text = "Please input your lap duration";
            break;
          case 1:
            peice_time = makeitint(input);
            P.PEICE = makeitsec(peice_time);
            enterCount++;
            text = "Please input your short break duration";
            break;
          case 2:
            smallrest_time = makeitint(input);
            P.SMALLREST = makeitsec(smallrest_time);
            enterCount++;
            text = "Pease input your long break duration";
            break;
          case 3:
            bigrest_time = makeitint(input);
            P.BIGREST = makeitsec(bigrest_time);
            enterCount++;
            break;
        }
      }
    EndDrawing();
    if (enterCount >= 4) {
      break;
    }
  }
  free(input);
  free(peice_time);
  free(smallrest_time);
  free(bigrest_time);
}

void pomodoro() {
  int state = 0;
  int loopC = 0;
  int lapC = 0;
  int countdown = P.PEICE*60;

  bool isPause = false;
  char *icon = (char *)calloc(sizeof(char), 5);
  icon = "#132#";
  while (!WindowShouldClose() && state != 69) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
      GuiToggle((Rectangle){(GetScreenWidth()/2.0f)-25, GetScreenWidth()*0.8, 50, 50}, icon, &isPause);
      if (isPause) {
        icon = "#131#";
      } else {
        icon = "#132#";
        switch (state) {
          case 0: {
            if (countdown >= 0) {
              DrawText(TextFormat("Tomato %i/%i, Peice %i/4", loopC+1, P.tomatoes, lapC+1), (GetScreenWidth()/2.0f)-100, GetScreenWidth()*0.3f, 20, RED); 
              DrawText(TextFormat("%i sec left...", countdown/60), (GetScreenWidth()/2.0f)-50, GetScreenWidth()*0.5f, 20, ORANGE); 
              countdown--;
            } else {
              if (lapC < 3) {
                lapC++;
                countdown = P.SMALLREST*60;
                state = 1;
              } else {
                lapC = 0;
                loopC++;
                countdown = P.BIGREST*60;
                state = 2;
              }
              if (loopC >= P.tomatoes) {
                state = 69;
              }
            }
            break;
          }
          case 1: {
            if (countdown >= 0) {
              DrawText("Start small rest...", (GetScreenWidth()/2.0f)-20, GetScreenWidth()*0.3f, 20, RED);
              DrawText(TextFormat("%i sec left...", countdown/60), (GetScreenWidth()/2.0f)-50, GetScreenWidth()*0.5f, 20, ORANGE); 
              countdown--;
            } else {
              countdown = P.PEICE*60;
              state = 0;
            }
            break;
          }
          case 2: {
            if (countdown >= 0) {
              DrawText("Start big rest :3", (GetScreenWidth()/2.0f)-20, GetScreenWidth()*0.3f, 20, RED);
              DrawText(TextFormat("%i sec left...", countdown/60), (GetScreenWidth()/2.0f)-50, GetScreenWidth()*0.5f, 20, ORANGE); 
              countdown--;
            } else {
              countdown = P.PEICE*60;
              state = 0;
            }
            break;
          }
        }
      }
    EndDrawing();
  }
  free(icon);
}


