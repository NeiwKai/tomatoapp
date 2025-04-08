#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600
#define maxINPUT 8

struct pomodoro {
  int tomatoes;
  int PEICE;
  int SMALLREST;
  int BIGREST;
};

struct pomodoro P;

bool FINISH = false;

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

void zawarudo(Rectangle btn) {
  while (true) {
    if (CheckCollisionPointRec(GetMousePosition(), btn)) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        break;
      }
    }
    BeginDrawing();
      DrawRectangleRec(btn, BROWN);
    EndDrawing();
  }
}

bool pomodoro(int tomatoes, int PEICE, int SMALLREST, int BIGREST) {
  bool btnstate = false;
  Rectangle btn = {WIDTH/2.0f, HEIGHT*0.8, 50, 50};
  for (int i=0; i<tomatoes; i++){
    if (WindowShouldClose()) CloseWindow();
    for (int j=0; j<4; j++){
      if (WindowShouldClose()) CloseWindow();
      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(btn, LIGHTGRAY);
        DrawText(TextFormat("Tomato %i/%i, Peice %i/4", i+1, tomatoes, j+1), (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, RED); 
      EndDrawing();
      // button stuff
      if (CheckCollisionPointRec(GetMousePosition(), btn)) {
        printf("yes\n");
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
          printf("hah\n"); 
          btnstate = true;
        }
      }
      while(btnstate==true) {
        if (CheckCollisionPointRec(GetMousePosition(), btn)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
              btnstate = false;
              break;
            }
        }
        BeginDrawing();
          DrawRectangleRec(btn, BROWN);
        EndDrawing();
      }
      // end button stuff
      for (int k=PEICE; k>=0; k--) {
        if (WindowShouldClose()) CloseWindow();
       BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawRectangleRec(btn, LIGHTGRAY);
          DrawText(TextFormat("Tomato %i/%i, Peice %i/4", i+1, tomatoes, j+1), (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, RED); 
          DrawText(TextFormat("%i sec left...", k), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
          WaitTime(1);
        EndDrawing();
        // button stuff
        if (CheckCollisionPointRec(GetMousePosition(), btn)) {
          printf("yes\n");
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            printf("hah\n"); 
            btnstate = true;
          }
        }
        if (btnstate) {
          zawarudo(btn);
        }
        // end button stuff
      }
      if (j==3){ break;}
      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Start small rest...", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
      EndDrawing();
      for (int k=SMALLREST; k>=0; k--) {
        if (WindowShouldClose()) CloseWindow();
        BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawRectangleRec(btn, LIGHTGRAY);
          DrawText("Start small rest...", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
          DrawText(TextFormat("%i sec left...", k), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
          WaitTime(1);
        EndDrawing();
        // button stuff
        if (CheckCollisionPointRec(GetMousePosition(), btn)) {
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            btnstate = true;
          }
        }
        while(btnstate==true) {
          if (CheckCollisionPointRec(GetMousePosition(), btn)) {
              if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                btnstate = false;
                break;
              }
          }
          BeginDrawing();
            DrawRectangleRec(btn, BROWN);
          EndDrawing();
        }
        // end button stuff
      }
    }
    if (i>=tomatoes-1){break;}
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Start big rest :3", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
    EndDrawing();
    for (int k=BIGREST; k>=0; k--) {
      if (WindowShouldClose()) CloseWindow();
     BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(btn, LIGHTGRAY);
        DrawText("Start big rest :3", (WIDTH/2.0f)-20, HEIGHT*0.3f, 20, RED);
        DrawText(TextFormat("%i sec left...", k), (WIDTH/2.0f)-50, HEIGHT*0.5f, 20, ORANGE); 
        WaitTime(1);
      EndDrawing();
      // button stuff
      if (CheckCollisionPointRec(GetMousePosition(), btn)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
          btnstate = true;
        }
      }
      while(btnstate==true) {
        if (CheckCollisionPointRec(GetMousePosition(), btn)) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
              btnstate = false;
              break;
            }
        }
        BeginDrawing();
          DrawRectangleRec(btn, BROWN);
        EndDrawing();
      }
      // end button stuff
    }
  }
  return true;
}

int main(){
  //pomodoro();
  InitWindow(WIDTH, HEIGHT, "tomato");

  bool READY = false;
  
  Rectangle textBox =  {(WIDTH/2.0f)-(255/2), HEIGHT*0.45f, 255, 50};
  char *input = (char *)calloc(sizeof(char), (maxINPUT+1));

  int *peice_time;
  int *smallrest_time;
  int *bigrest_time;

  bool mouseOnText = false;
  int letterCount = 0;
  int enterCount = 0;

  SetTargetFPS(60);

  while (true) {
    if (WindowShouldClose()) CloseWindow();
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
          case 1:
            peice_time = makeitint(input);
            P.PEICE = makeitsec(peice_time);
            free(peice_time);
          case 2:
            smallrest_time = makeitint(input);
            P.SMALLREST = makeitsec(smallrest_time);
            free(smallrest_time);
          case 3:
            bigrest_time = makeitint(input);
            P.BIGREST = makeitsec(bigrest_time);
            free(bigrest_time);
        } 
        for (int i=0; i<maxINPUT+1; i++) {
          input[i] = '\0';
        } 
        letterCount = 0;
        enterCount++;
        if (enterCount >= 4) {
          free(input);
          READY = true;
          break;
        }
      }
    } else {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawRectangleRec(textBox, LIGHTGRAY);
      DrawText("Please Input Your time", (WIDTH/2.0f)-100, HEIGHT*0.3f, 20, GREEN);
      DrawText(input, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
      if (mouseOnText) {
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
      } else {
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
      }
    EndDrawing();
    if (FINISH) break;
  } 
  if (pomodoro(P.tomatoes, P.PEICE, P.SMALLREST, P.BIGREST)) { 
      while (!WindowShouldClose()) {
        BeginDrawing();
          ClearBackground(RAYWHITE);
          DrawText("CONGRATS :D", (WIDTH/2.0f)-10, HEIGHT*0.5f, 50, RED);
        EndDrawing();
      }
      CloseWindow();
  }
  return 0;
}
