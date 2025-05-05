#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600
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
void zawarudo(button btn);
bool playpauseCheck(button btn);
// state
void getUserInput();
void pomodoro();

int main(){
  P.state = 0;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
  InitWindow(WIDTH, HEIGHT, "tomato");
  SetTargetFPS(60);

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
          DrawText("CONGRATS :D", (WIDTH/2.0f)-10, HEIGHT*0.5f, 50, RED);
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
  Rectangle textBox =  {(WIDTH/2.0f)-(255/2), HEIGHT*0.45f, 255, 50};
  char *input = (char *)calloc(sizeof(char), (maxINPUT+1));

  int *peice_time;
  int *smallrest_time;
  int *bigrest_time;

  bool mouseOnText = false;
  int letterCount = 0;
  int enterCount = 0;

  while (true) {
    if (WindowShouldClose()) break;
    if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
    else mouseOnText = false;
    if (mouseOnText) {
      SetMouseCursor(MOUSE_CURSOR_IBEAM);
      int key = GetCharPressed(); 
      while (key>0) {
        if (letterCount < maxINPUT) {
          input[letterCount] = (char)key;
          letterCount++;
        }
        key = GetCharPressed();
      }
      if (IsKeyPressed(KEY_BACKSPACE)) {
        letterCount--;
        if (letterCount < 0) letterCount = 0;
        input[letterCount] = '\0';
      }
      if (IsKeyPressed(KEY_ENTER)) {
        switch(enterCount) {
          case 0:
            P.tomatoes = atoi(input);
            break;
          case 1:
            peice_time = makeitint(input);
            P.PEICE = makeitsec(peice_time);
            free(peice_time);
            break;
          case 2:
            smallrest_time = makeitint(input);
            P.SMALLREST = makeitsec(smallrest_time);
            free(smallrest_time);
            break;
          case 3:
            bigrest_time = makeitint(input);
            P.BIGREST = makeitsec(bigrest_time);
            free(bigrest_time);
            break;
        } 
        for (int i=0; i<maxINPUT+1; i++) {
          input[i] = '\0';
        } 
        letterCount = 0;
        enterCount++;
        if (enterCount >= 4) {
          free(input);
          break;
        }
      }
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawRectangleRec(textBox, LIGHTGRAY);
      switch(enterCount) {
        case 0:
          DrawText("Input your tomato(loop)", (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, GREEN);
          break;
        case 1:
          DrawText("Input your peice(lap) duration(H:M:S)", (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, GREEN);
          break;
        case 2:
          DrawText("Input your short break duration(H:M:S)", (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, GREEN);
          break;
        case 3:
          DrawText("Input your long break duration(H:M:S)", (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, GREEN);
          break;
      }
      DrawText(input, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
      if (mouseOnText) {
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
      } else {
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
      }
    EndDrawing();
  }
}


void zawarudo(button btn) {
  do {
    if (WindowShouldClose()) CloseWindow();
    if (CheckCollisionPointRec(GetMousePosition(), btn.rect)) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) btn.state = !btn.state;
    }
    BeginDrawing();
      DrawRectangleRec(btn.rect, BROWN);
    EndDrawing();
  } while (btn.state); 
}

bool playpauseCheck(button btn) {
  if (CheckCollisionPointRec(GetMousePosition(), btn.rect)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) btn.state = !btn.state;
  }
  return btn.state;
}

void pomodoro() {
  button btn;
  btn.rect = (Rectangle) {(WIDTH/2.0f)-25, HEIGHT*0.8, 50, 50};
  btn.state = false;
  for (int i=0; i<P.tomatoes; i++){
    if (WindowShouldClose()) CloseWindow();
    for (int j=0; j<4; j++){
      if (WindowShouldClose()) CloseWindow();
      if (playpauseCheck(btn)) zawarudo(btn);
      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(btn.rect, LIGHTGRAY);
        DrawText(TextFormat("Tomato %i/%i, Peice %i/4", i+1, P.tomatoes, j+1), (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, RED); 
      EndDrawing();
      for (int k=P.PEICE*60; k>=0; k--) {
        if (WindowShouldClose()) CloseWindow();
        if (playpauseCheck(btn)) zawarudo(btn);
        BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawRectangleRec(btn.rect, LIGHTGRAY);
          DrawText(TextFormat("Tomato %i/%i, Peice %i/4", i+1, P.tomatoes, j+1), (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, RED); 
          DrawText(TextFormat("%i sec left...", k/60), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
        EndDrawing();
      }
      if (j==3){ break;}
      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Start small rest...", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
      EndDrawing();
      for (int k=P.SMALLREST*60; k>=0; k--) {
        if (WindowShouldClose()) CloseWindow();
        BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawRectangleRec(btn.rect, LIGHTGRAY);
          DrawText("Start small rest...", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
          DrawText(TextFormat("%i sec left...", k/60), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
        EndDrawing();
     }
    }
    if (i>=P.tomatoes-1){break;}
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Start big rest :3", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
    EndDrawing();
    for (int k=P.BIGREST*60; k>=0; k--) {
      if (WindowShouldClose()) CloseWindow();
      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(btn.rect, LIGHTGRAY);
        DrawText("Start big rest :3", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
        DrawText(TextFormat("%i sec left...", k/60), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
      EndDrawing();
   }
  }
}


