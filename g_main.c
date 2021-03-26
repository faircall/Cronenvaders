/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
    


#define true 1
#define false 0

Vector2 Vector2Set(float x, float y) 
{
    Vector2 result;
    result.x = x;
    result.y = y;
    return result;
}

//typedef unsigned int bool;

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

Vector2 wrap_player(Vector2 position, int *counter, float width, float height)
{
    Vector2 result = Vector2Set(position.x, position.y);
    bool wrapped = false;
    if (result.x > SCREENWIDTH + width/2.0f) {
	result.x = -width/2.0;
	wrapped = true;
    } else if (result.x < -width/2.0f) {
	result.x = SCREENWIDTH + width/2.0f;
	wrapped = true;
    }
    if (result.y > SCREENHEIGHT + height/2.0f) {
	result.y = -height/2.0f;
	wrapped = true;
    } else if (result.y < -height/2.0f) {
	result.y = SCREENHEIGHT + height/2.0f;
	wrapped = true;
    }
    if (wrapped) {
	//check dis
	*counter = (*counter) + 1;
    }
    return result;
}

int main(void)
{
    // Initializationn
    //--------------------------------------------------------------------------------------


    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Cronenvaders");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    struct Vector2 player_position = {10.0f, 0.0f};
    
    struct Vector2 player_velocity = {0.0f, 0.0f};
    
    float inv_mass = 1.0f/10.0f;
    
    //struct Vector2 player_acceleration = {0.0f, 0.0f};
    
    struct Vector2 player_heading = {0.0f, 0.0f};  
    
    float player_acceleration = 800.0f;

    float player_top_speed = 300.0f;
    
    float player_angle = 0.0f;

    float friction = 300.0f;

    float player_width = 10.0f;
    float player_height = 10.0f;

    float player_rotate_speed = 5.0f;


    int enemy_count = 5;
    float enemy_width = 8.0f;
    float enemy_height = 8.0f;
    Vector2 *enemy_positions = (Vector2*)malloc(sizeof(Vector2) * enemy_count);
    for (int i = 0; i < enemy_count; i++) {
	enemy_positions[i] = Vector2Set((float)(i*SCREENWIDTH)/(float)enemy_count + 50.0f, 40.0f);
    }

    //we will set this at zero
    //and increment it each time the player wraps the border...
    //and give them a surprise after they go off too many times ;)
    int wrap_counter = 0;

    float targeting_angle = 0.0f;

    //we'll implement a sort of focusing aspect
    // to targeting
    // Main game loop
    int mouse_x, mouse_y;
    Color mouse_color = BLACK;
    HideCursor();
    
    while (!WindowShouldClose()) {
	// Detect window close button or ESC key
    
	//to make this creepy,
	//we can use a sort of tank control
	//rather than allowing for strafing

	//Done...
	//something cool would be to maybe start off
	//visually simple, and then
	//add the complexity visually
   
    
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        float dt = GetFrameTime();
        
        player_heading = Vector2Set(0.0f, 0.0f);
        float acceleration = 0.0f;

	mouse_x = GetMouseX();
	mouse_y = GetMouseY();
	mouse_color = BLACK;
	bool shooting = false;
	bool aiming = false;

	if (IsMouseButtonDown(1)) {
	    mouse_color = RAYWHITE;
	    aiming = true;
	}
	if (IsMouseButtonDown(0)) {
	    if (aiming)
		shooting = true;
	}
	
        if (IsKeyDown(KEY_W))
        {
            acceleration = player_acceleration;
        }
        if (IsKeyDown(KEY_S))
        {
            acceleration = -player_acceleration*0.5f;
        }

        if (IsKeyDown(KEY_A))
        {
	    player_angle -= (player_rotate_speed*dt);
            
        }       
        if (IsKeyDown(KEY_D))
        {
	    player_angle += (player_rotate_speed*dt);
            
        }

	if (shooting) {
	    mouse_color = RED;
	}
	
        float fp_epsilon = 0.001f;

	player_heading.x = cos(player_angle);
	player_heading.y = sin(player_angle);

	//this can all be abstracted under 'update simulation'
        if (Vector2Length(player_heading) > fp_epsilon)
            player_heading = Vector2Normalize(player_heading);
	
        player_velocity = Vector2Add(player_velocity, Vector2Scale(player_heading, dt * acceleration));
	if (Vector2Length(player_velocity) > fp_epsilon*1000.0f) {
	    Vector2 player_friction = Vector2Scale(Vector2Normalize(player_velocity), -friction);
	    player_velocity = Vector2Add(player_velocity, Vector2Scale(player_friction, dt));}
	if (Vector2Length(player_velocity) > player_top_speed) {
	    player_velocity = Vector2Normalize(player_velocity);
	    player_velocity = Vector2Scale(player_velocity, player_top_speed);
	}

	if (Vector2Length(player_velocity) > fp_epsilon*1000.0f) {
	    player_position = Vector2Add(player_position, Vector2Scale(player_velocity, dt));
	}

	player_position = wrap_player(player_position, &wrap_counter, player_width, player_height);



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

	if (wrap_counter < 5) {
	    
	    ClearBackground(MAGENTA);
	} else {
	    ClearBackground(BLACK);
	}



	for (int i = 0; i < enemy_count; i++) {
	    Vector2 pos = enemy_positions[i];
	    
	    DrawRectangle(pos.x - enemy_width/2.0f, pos.y - enemy_height/2.0f, enemy_width, enemy_height, RED);
	}
        
        DrawRectangle(player_position.x - player_width/2.0f, player_position.y - player_height/2.0f, player_width, player_height, RAYWHITE);
	DrawRectangle((player_position.x  - player_width/4.0f) + player_heading.x*10.0f, player_position.y - player_height/4.0f + player_heading.y*10.0f, 4.0f, 4.0f, RAYWHITE);
	DrawRectangle(mouse_x, mouse_y, 4.0f, 4.0f, mouse_color);

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
