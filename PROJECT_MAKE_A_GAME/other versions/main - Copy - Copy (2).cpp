//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include <iostream>
#include <assert.h>
#include <time.h>
#include <ctime>
#include <stack> 
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define songname "startsong.wav"
#define songwin "win.wav"
#define songbip "bip.wav"

#pragma warning(disable : 4996)
using namespace std;

#include "SDL2-2.0.8\include\SDL.h"

#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
//copy the SDL2_image.lib from the SDL image folder to your SDL lib folder
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

unsigned char prev_key_state[256];
unsigned char *keys = NULL;





namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 500;
	int screen_height = 550;
}


int random_figure()
{
	
	int random_number = rand() % 7;
	return random_number;

}

void init_position(SDL_Texture *sprite_texture[10])
{
	int pos1_x = 50;
	int pos1_y = 100;
	// drawing the pictures 
	SDL_Rect draw[21];
	for (int i = 0; i < 21; i++)
	{
		draw[i].w = 100;
		draw[i].h = 100;
	}

	int number_of_rows = 4;
	int number_of_columns = 4;
	int n;
	for (int j = 0; j < number_of_rows; j++)
	{
		for (int i = 0; i < number_of_columns; i++)
		{
			draw[i].x = pos1_x;
			draw[i].y = pos1_y;
			n = random_figure();
			SDL_RenderCopyEx(Game::renderer, sprite_texture[n], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
			pos1_x += draw[i].w + 1;
		}
		pos1_y += draw[j].h + 1;
		pos1_x = 50;
	}
}

void print_play(SDL_Texture *font_texture, int number)
{
	/////////////////////////////////////////////////////////////////////////////////////////
	char play_text[30];
	int play_text_size = 25;

	play_text[20] = 0;
	//char tmp[20];
	sprintf(play_text, "SPACEBAR  %d", number);

	int text_x = 2;
	int text_y = play_text_size * 2 + 2;
	for (int i = 0; i < 16; i++)
	{
		SDL_Rect src;
		src.x = 64 * (play_text[i] % 16);//column
		src.y = 64 * (play_text[i] / 16);//row
		src.w = 64;
		src.h = 64;

		SDL_Rect dest;
		dest.x = text_x;
		dest.y = text_y;
		dest.w = play_text_size;
		dest.h = dest.w;

		//set transparency of the texture.
		//0 to 255, 255 is opaque
		SDL_SetTextureAlphaMod(font_texture, 255);

		SDL_RenderCopyEx(Game::renderer, font_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		//increment text_x!
		text_x += dest.w;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



void print_coins(SDL_Texture *font_texture, int number)
{
	/////////////////////////////////////////////////////////////////////////////////////////


	char coins_text[17];
	int coins_text_size = 25;

	coins_text[16] = 0;
//	char tmp[20];
	sprintf(coins_text, "Coins: %d", number);

	int text_x = 2;
	int text_y = 2;
	for (int i = 0; i < 16; i++)
	{
		SDL_Rect src;
		src.x = 64 * (coins_text[i] % 16);//column
		src.y = 64 * (coins_text[i] / 16);//row
		src.w = 64;
		src.h = 64;

		SDL_Rect dest;
		dest.x = text_x;
		dest.y = text_y;
		dest.w = coins_text_size;
		dest.h = dest.w;

		//set transparency of the texture.
		//0 to 255, 255 is opaque
		SDL_SetTextureAlphaMod(font_texture, 255);

		SDL_RenderCopyEx(Game::renderer, font_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		//increment text_x!
		text_x += dest.w;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void print_won(SDL_Texture *font_texture, int number)
{
	char coins_text[17];
	int coins_text_size = 25;
	coins_text[16] = 0;
	sprintf(coins_text, "Won: %d", number);

	int text_x = 2;
	int text_y = coins_text_size+2;
	for (int i = 0; i < 16; i++)
	{
		SDL_Rect src;
		src.x = 64 * (coins_text[i] % 16);//column
		src.y = 64 * (coins_text[i] / 16);//row
		src.w = 64;
		src.h = 64;

		SDL_Rect dest;
		dest.x = text_x;
		dest.y = text_y;
		dest.w = coins_text_size;
		dest.h = dest.w;

		SDL_SetTextureAlphaMod(font_texture, 255);
		SDL_RenderCopyEx(Game::renderer, font_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		//increment text_x!
		text_x += dest.w;
	}
	/////////////////////

}


int spin_rows(SDL_Texture *sprite_texture[10], SDL_Texture *font_texture, int total)
{
	int won=0;
	int pos1_x = 50;
	int pos1_y = 100;
	int array1[7] = { 0, 1, 2, 3, 4, 5, 6 };
	int array2[7] = { 0, 2, 4, 6, 1, 3, 5 };
	int array3[7] = { 6, 3, 0, 4, 1, 5, 2 };
	int array4[7] = { 5, 1, 4, 0, 3, 6, 2 };
	int size = 16;

	int random = random_figure();

	SDL_Rect draw[21];
	for (int i = 0; i < 21; i++)
	{
		draw[i].w = 100;
		draw[i].h = 100;
	}

	int number_of_rows = 4;
	int number_of_columns = 4;
	int n;

	int count = 0;

	int save_winner[16];
	
	n = rand() % 7;
	for (int i = 0; i < number_of_rows; i++)
	{
		draw[i].x = pos1_x;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array1[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array1[n];
		n = n + 1;
		pos1_y += 100+1;
		if (n > 6) n = 0;
	}
	pos1_y = 100;
	n = rand() % 7;

	for (int i = 0; i < number_of_rows; i++)
	{
		draw[i].x = pos1_x+100+1;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array2[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array2[n];
		n = n + 1;
		//printf(" n:%d i:%d", n, i);
		pos1_y += 100+1;
		if (n > 6) n = 0;
	}

	n = rand() % 7;
	pos1_y = 100;
	for (int i = 0; i < number_of_rows; i++)
	{
		draw[i].x = pos1_x + 200+2;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array3[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array3[n];
		n = n + 1;
		//printf(" n:%d i:%d", n, i);
		pos1_y += 100+1;
		if (n > 6) n = 0;
	}

	n = rand() % 7;
	pos1_y = 100;
	for (int i = 0; i < number_of_rows; i++)
	{
		draw[i].x = pos1_x + 300+3;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array4[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array4[n];
		n = n + 1;
		//printf(" n:%d i:%d", n, i);
		pos1_y += 100+1;
		if (n > 6) n = 0;
	}


	for (int i = 0; i < size; i++)
	{
		int countto4 = 0;
		for (int j = i + 1; j < size; j++)
		{
			if (save_winner[i] == save_winner[j])
			{
				countto4++;
				if (countto4 == 3)
				{
					printf("Won: +200 %d\n", save_winner[i]);
					countto4 = 0;
					won += 200;
					
					total += won;
					
				}	
			}

		}
		
	}

	if (won > 100)
	{
		SDL_AudioSpec wavSpec;
		Uint32 wavLength;
		Uint8 *wavBuffer;
		SDL_LoadWAV(songwin, &wavSpec, &wavBuffer, &wavLength);
		SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
		int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
		SDL_FreeWAV(wavBuffer);
		
	}
	print_won(font_texture, won);
	print_coins(font_texture, total);
	return total;
}





int main(int argc, char **argv)
{
	int total_coins = 1000;
		
	SDL_Init(SDL_INIT_AUDIO);// ini audio
	SDL_AudioSpec wavSpec;
	Uint8 *wavBuffer;
	Uint32 wavLength;
	SDL_AudioDeviceID deviceId;


	int sdlresult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlresult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

//	srand(time(0));
	
	prev_key_state[256];
	keys = (unsigned char*)SDL_GetKeyboardState(NULL);
	
	//place the png file next to your main.cpp
	const int n_imagenes = 10;
	const char *imagenes[n_imagenes];
	SDL_Surface *sprite_surface[n_imagenes];
	SDL_Texture *sprite_texture[n_imagenes];
	const char *font_filename = "font_sheet.png";

	imagenes[0] = "CIGARRETE.png";
	imagenes[1] = "heart.jpg";
	imagenes[2] = "cherry2.png";
	imagenes[3] = "WATERMELON.jpg";
	imagenes[4] = "CHARMANDER.jpg";
	imagenes[5] = "fox.png";
	imagenes[6] = "coin.png";

	SDL_Window *window = SDL_CreateWindow("ALE",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);
	Game::renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	for (int i = 0; i <= 6; i++)
	{
		sprite_surface[i] = IMG_Load(imagenes[i]);
		assert(sprite_surface[i]);
		sprite_texture[i] = SDL_CreateTextureFromSurface(Game::renderer, sprite_surface[i]);
		assert(sprite_texture[i]);
		SDL_FreeSurface(sprite_surface[i]);
	}
		
	SDL_Surface *font_surface = IMG_Load(font_filename);
	assert(font_surface);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(Game::renderer, font_surface);
	SDL_FreeSurface(font_surface);
	
	unsigned int last_text_change_time = SDL_GetTicks();
	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	int pos1_x = 50; // columns
	int pos1_y = 100; // rows
	int won = 0;

	unsigned int current_time = SDL_GetTicks();

	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);	// set color to black
	SDL_RenderClear(Game::renderer);	//clear screen with black
	print_coins(font_texture, total_coins);
	print_won(font_texture, 0);
	init_position(sprite_texture);

	//flip buffers
	SDL_RenderPresent(Game::renderer);

	bool done = false;
	while (!done)
	{
	
		
		memcpy(prev_key_state, keys, 256);
		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		//GAME CODE
		int pressed = false;
		int cost_to_play = -100;
		int success;
		// GET MOUSE CLICK
		SDL_PumpEvents();
		if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			SDL_Log("SPIN");
			pressed = true;
			total_coins += cost_to_play;
			
		}

		// GET SPACE BAR 
		if (keys[SDL_SCANCODE_SPACE])
		{
			printf("\n Cost to Play -100\n");
			pressed = true;
			total_coins += cost_to_play;
			
		}
	
		
		Uint32 timeout = SDL_GetTicks() + 3000; // time spining
		
		while (pressed)
		{
			if (SDL_LoadWAV(songname, &wavSpec, &wavBuffer, &wavLength) == NULL) 
			{
				fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
			}
			else 
			{
				deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
				// play audio
				success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
				SDL_FreeWAV(wavBuffer);
			}
			
			while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) 
			{
				SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255); 	
				SDL_RenderClear(Game::renderer);
				init_position(sprite_texture);
				//print_won(font_texture, won);
				print_coins(font_texture, total_coins);
				SDL_RenderPresent(Game::renderer);
				pressed = false;
				//printf("im doing this\n");

				SDL_PumpEvents();
				if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					SDL_Log("STOP PLEASE");
					timeout = SDL_GetTicks();
					if (SDL_LoadWAV(songbip, &wavSpec, &wavBuffer, &wavLength) == NULL) {
						fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
					}
					else {

						deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
						// play audio
						success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
						SDL_PauseAudioDevice(deviceId, 0);
						SDL_FreeWAV(wavBuffer);
					}
				}		
			}	

				
			if (SDL_LoadWAV(songbip, &wavSpec, &wavBuffer, &wavLength) == NULL) {
				fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
			}
			else {
				
				deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
				// play audio
				success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);
				SDL_FreeWAV(wavBuffer);
			}
			SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
			SDL_RenderClear(Game::renderer);
			int total_spin = spin_rows(sprite_texture, font_texture, total_coins);
			total_coins = total_spin;
			//print_won(font_texture, won);
			printf("Total coins: %d\n", total_coins);
			print_coins(font_texture, total_coins);
			print_play(font_texture, cost_to_play);
			SDL_RenderPresent(Game::renderer);
		}
	
			
	
	}	

	
	SDL_CloseAudioDevice(deviceId);
	SDL_Quit();
	return 0;
}
