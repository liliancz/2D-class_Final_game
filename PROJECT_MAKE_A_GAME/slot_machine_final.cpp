#pragma warning(disable : 4996)

#include <iostream>
#include <assert.h>
#include <time.h>
#include <ctime>
#include <stack> 
#include <stdio.h>
#include <stdlib.h>    
#include <time.h>      
#include "SDL2_mixer-2.0.2\include\SDL_mixer.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2_mixer-2.0.2\\lib\\x86\\SDL2_mixer.lib")
#pragma comment(linker,"/subsystem:console")

unsigned char prev_key_state[256];
unsigned char *keys = NULL;


namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 550;
	int screen_height = 650;
}


int random_figure()
{
	int random_number = rand() % 7;
	return random_number;
}

void play_songs(int n)
{

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	const char *star = "startsong.wav";
	const char* win = "win.wav";
	const char* end = "bip.wav";

	const char* song = 0;
	if (n == 1)song = star;
	if (n == 2)song = win;
	if (n == 3)song = end;

	SDL_LoadWAV(song, &wavSpec, &wavBuffer, &wavLength);

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
	SDL_FreeWAV(wavBuffer);
}

void draw_images(SDL_Texture *sprite_texture[12], int save_winner[12])
{
	int pos1_x = 50;
	int pos1_y = 200;
	int update_pos_y = 200;

	int array1[7] = { 0, 1, 2, 3, 4, 5, 6 };
	int array2[7] = { 0, 2, 4, 6, 1, 3, 5 };
	int array3[7] = { 6, 3, 0, 4, 1, 5, 2 };

	int random = rand() % 7;
	int n_rows = 4;
	int n_cols = 3;
	int n = 0;
	int count = 0;


	SDL_Rect draw[16];
	for (int i = 0; i < 16; i++)
	{
		draw[i].w = 100;
		draw[i].h = 100;
	}

	n = rand() % 7;
	for (int i = 0; i < n_rows; i++)
	{
		draw[i].x = pos1_x;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array1[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array1[n];
		n = n + 1;
		pos1_y += 100 + 1;
		if (n > 6) n = 0;
	}
	pos1_y = update_pos_y;
	n = rand() % 7;

	for (int i = 0; i < n_rows; i++)
	{
		draw[i].x = pos1_x + 100 + 1;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array2[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array2[n];
		n = n + 1;
		pos1_y += 100 + 1;
		if (n > 6) n = 0;
	}

	n = rand() % 7;
	pos1_y = update_pos_y;
	for (int i = 0; i < n_rows; i++)
	{
		draw[i].x = pos1_x + 200 + 2;
		draw[i].y = pos1_y;
		SDL_RenderCopyEx(Game::renderer, sprite_texture[array3[n]], NULL, (const SDL_Rect*)&draw[i], 0, NULL, SDL_FLIP_NONE);
		save_winner[count++] = array3[n];
		n = n + 1;
		pos1_y += 100 + 1;
		if (n > 6) n = 0;
	}

}



void print_text(SDL_Texture *font_texture, char text[100], float text_x, float text_y, int random_text_size)
{
	for (int i = 0; i < 16; i++)
	{
		SDL_Rect src;
		src.x = 64 * (text[i] % 16);//column
		src.y = 64 * (text[i] / 16);//row
		src.w = 64;
		src.h = 64;

		SDL_Rect dest;
		dest.x = text_x;
		dest.y = text_y;
		dest.w = random_text_size;
		dest.h = dest.w;

		SDL_SetTextureAlphaMod(font_texture, 255);
		SDL_RenderCopyEx(Game::renderer, font_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
		text_x += dest.w;
	}
}

void text_screen(SDL_Texture *font_texture, int cost_to_play, int total_coins, int coins_earned)
{
	float text_x = 10;
	float text_y = 10;
	char random_text[100];
	int random_text_size = 32;

	sprintf(random_text, "> SLOT MACHINE <  ");
	print_text(font_texture, random_text, text_x, text_y, random_text_size);
	text_y = 50;
	sprintf(random_text, "-Play for: $%d ", cost_to_play);
	print_text(font_texture, random_text, text_x, text_y, random_text_size);
	text_y = 80;
	sprintf(random_text, "Coins: %d          ", total_coins);
	print_text(font_texture, random_text, text_x, text_y, random_text_size);
	text_y = 110;
	sprintf(random_text, "Won :  %d          ", coins_earned);
	print_text(font_texture, random_text, text_x, text_y, random_text_size);
	text_y = 140;
	sprintf(random_text, "A:Add 1000 coins");
	print_text(font_texture, random_text, text_x, text_y, random_text_size);
}


int draw_line(int position[3])
{

	int pos_x[5] = { 25, 100, 200, 300, 375 };
	int pos_y[4] = { 250, 350, 450, 550 };
	int  y1 = 0, y2 = 0, y3 = 0;

	SDL_SetRenderDrawColor(Game::renderer, rand() % 255, rand() % 255, rand() % 255, 255);

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			if (position[0] == 0)
			{
				y1 = pos_y[0];
				break;
			}
			if (position[0] == 1)
			{
				y1 = pos_y[1];
				break;
			}
			if (position[0] == 2)
			{
				y1 = pos_y[2];
				break;
			}
			if (position[0] == 3)
			{
				y1 = pos_y[3];
				break;
			}

		case 1:
			if (position[1] == 4 && (position[0] == 0 || position[0] == 1))
			{
				y2 = pos_y[0];
				break;
			}
			if (position[1] == 5 && (position[0] == 0 || position[0] == 1 || position[0] == 2))
			{
				y2 = pos_y[1];
				break;
			}
			if (position[1] == 6 && (position[0] == 1 || position[0] == 2 || position[0] == 3))
			{
				y2 = pos_y[2];
				break;
			}
			if (position[1] == 7 && (position[0] == 2 || position[0] == 3))
			{
				y2 = pos_y[3];
				break;
			}
			else
			{
				return 0;
			}

		case 2:

			if (position[2] == 8 && (position[1] == 4 || position[1] == 5))
			{
				y3 = pos_y[0];
				SDL_RenderDrawLine(Game::renderer, pos_x[0], y1, pos_x[1], y1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1], y1, pos_x[2], y2);
				SDL_RenderDrawLine(Game::renderer, pos_x[2], y2, pos_x[3], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[3], y3, pos_x[4], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[0] + 1, y1 + 1, pos_x[1] + 1, y1 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1] + 1, y1 + 1, pos_x[2] + 1, y2 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[2] + 1, y2 + 1, pos_x[3] + 1, y3 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[3] + 1, y3 + 1, pos_x[4] + 1, y3 + 1);
				return 1;
				break;

			}
			if (position[2] == 9 && (position[1] == 4 || position[1] == 5 || position[1] == 6))
			{
				y3 = pos_y[1];
				SDL_RenderDrawLine(Game::renderer, pos_x[0], y1, pos_x[1], y1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1], y1, pos_x[2], y2);
				SDL_RenderDrawLine(Game::renderer, pos_x[2], y2, pos_x[3], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[3], y3, pos_x[4], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[0] + 1, y1 + 1, pos_x[1] + 1, y1 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1] + 1, y1 + 1, pos_x[2] + 1, y2 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[2] + 1, y2 + 1, pos_x[3] + 1, y3 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[3] + 1, y3 + 1, pos_x[4] + 1, y3 + 1);
				return 1;
				break;

			}
			if (position[2] == 10 && (position[1] == 5 || position[1] == 6 || position[1] == 7))
			{
				y3 = pos_y[2];
				SDL_RenderDrawLine(Game::renderer, pos_x[0], y1, pos_x[1], y1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1], y1, pos_x[2], y2);
				SDL_RenderDrawLine(Game::renderer, pos_x[2], y2, pos_x[3], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[3], y3, pos_x[4], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[0] + 1, y1 + 1, pos_x[1] + 1, y1 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1] + 1, y1 + 1, pos_x[2] + 1, y2 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[2] + 1, y2 + 1, pos_x[3] + 1, y3 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[3] + 1, y3 + 1, pos_x[4] + 1, y3 + 1);
				return 1;
				break;

			}
			if (position[2] == 11 && (position[1] == 6 || position[1] == 7))
			{
				y3 = pos_y[3];
				SDL_RenderDrawLine(Game::renderer, pos_x[0], y1, pos_x[1], y1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1], y1, pos_x[2], y2);
				SDL_RenderDrawLine(Game::renderer, pos_x[2], y2, pos_x[3], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[3], y3, pos_x[4], y3);
				SDL_RenderDrawLine(Game::renderer, pos_x[0] + 1, y1 + 1, pos_x[1] + 1, y1 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[1] + 1, y1 + 1, pos_x[2] + 1, y2 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[2] + 1, y2 + 1, pos_x[3] + 1, y3 + 1);
				SDL_RenderDrawLine(Game::renderer, pos_x[3] + 1, y3 + 1, pos_x[4] + 1, y3 + 1);
				return 1;
				break;

			}
		}
	}
	return 0;
}


void draw_winner(int save_winner[12], int save_pos[3], int n_winner)
{
	SDL_Surface *surface = IMG_Load("winner.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);


	int current_frame = 0;
	unsigned int last_time_frame_changed = 0;
	bool play = true;
	int count = 0;
	while (play)
	{
		unsigned int current_time = SDL_GetTicks();

		if (current_time - last_time_frame_changed > 66)
		{
			last_time_frame_changed = current_time;
			current_frame++;
			if (current_frame >= 4)
			{
				current_frame = 0;
				count++;
			}
		}
		if (count > 5) play = false;
		int x_frame = current_frame * 32;
		SDL_Rect src = { x_frame,0,32,32 };
		SDL_Rect dest = { 360,300,150,150 };
		SDL_RenderCopyEx(Game::renderer, texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(Game::renderer);

	}
}




int find_pos(int save_winner[12], int n_winner)
{
	int save_pos[3];
	int n = 0;

	for (int i = 0; i < 12; i++)
	{
		if (save_winner[i] == n_winner) save_pos[n++] = i;

	}

	int r = draw_line(save_pos);
	if (r == 1)
	{
		play_songs(2);
		draw_winner(save_winner, save_pos, n_winner);


		if (save_pos[0] == 0 && save_pos[1] == 4 && save_pos[2] == 8) return 1000;
		if (save_pos[0] == 1 && save_pos[1] == 5 && save_pos[2] == 9) return 1000;
		if (save_pos[0] == 2 && save_pos[1] == 6 && save_pos[2] == 10) return 1000;
		if (save_pos[0] == 3 && save_pos[1] == 7 && save_pos[2] == 11) return 1000;

		return 200;
	}
	if (r == 0) return 0;

	return 200;
}


int who_won(SDL_Texture *sprite_texture, int save_winner[12])
{
	int n_images = 12;
	int size_array = 7;
	int counter[7] = { 0, 0, 0, 0, 0, 0, 0 };
	int n_winner = 0;
	int n = 0;
	int save_pos[3];
	int r = 0;

	for (int i = 0; i < n_images; i++)
	{
		if (save_winner[i] == 0) { counter[0]++; }
		if (save_winner[i] == 1) { counter[1]++; }
		if (save_winner[i] == 2) { counter[2]++; }
		if (save_winner[i] == 3) { counter[3]++; }
		if (save_winner[i] == 4) { counter[4]++; }
		if (save_winner[i] == 5) { counter[5]++; }
		if (save_winner[i] == 6) { counter[6]++; }

		for (int j = 0; j < size_array; j++)
		{
			if (counter[j] == 3)
			{
				n_winner = save_winner[i];
				r = find_pos(save_winner, n_winner);
				return r;
			}

		}
	}


}



int main(int argc, char **argv)
{

	int total_coins = 100;
	int coins_earned = 0;
	int cost_to_play = -100;
	int winner_array[12];

	const int n_imagenes = 7;
	const char *imagenes[n_imagenes];
	const char *imagenes_1[n_imagenes];
	SDL_Surface *sprite_surface[n_imagenes];
	SDL_Surface *sprite_surface_1[n_imagenes];
	SDL_Texture *sprite_texture[n_imagenes];
	SDL_Texture *sprite_texture_1[n_imagenes];

	imagenes[0] = "CIGARRETE.png";
	imagenes[1] = "heart_1.jpg";
	imagenes[2] = "cherry2.png";
	imagenes[3] = "WATERMELON.jpg";
	imagenes[4] = "CHARMANDER.jpg";
	imagenes[5] = "fox.png";
	imagenes[6] = "coin.png";


	const char *image_filename = "cherry2.png";
	const char *font_filename = "font_sheet.png";

	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));

	SDL_Window *window = SDL_CreateWindow("SLOT MACHINE SIMULATOR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//load image into a surface.
	for (int i = 0; i < 7; i++)
	{
		sprite_surface[i] = IMG_Load(imagenes[i]);
		assert(sprite_surface[i]);
		sprite_texture[i] = SDL_CreateTextureFromSurface(Game::renderer, sprite_surface[i]);
		assert(sprite_texture[i]);
		SDL_FreeSurface(sprite_surface[i]);
	}

	//load font into a surface.
	SDL_Surface *font_surface = IMG_Load(font_filename);
	assert(font_surface);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(Game::renderer, font_surface);
	SDL_FreeSurface(font_surface);


	SDL_RenderClear(Game::renderer);
	draw_images(sprite_texture, winner_array);
	text_screen(font_texture, cost_to_play, total_coins, coins_earned);
	SDL_RenderPresent(Game::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	unsigned int last_text_change_time = SDL_GetTicks();


	bool press_bar = false;
	bool press_mousebotton = false;
	bool press_A = false;
	bool prev_press_mousebotton = true;
	bool prev_press_A = true;

	SDL_Log("Play with space bar or mouse click");
	SDL_Log("Buy 1000 coins pressing A");

	bool done = false;
	while (!done)
	{

		SDL_Init(SDL_INIT_AUDIO);// ini audio


		unsigned int current_time = SDL_GetTicks();
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

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			done = true;
		}


		if (prev_press_mousebotton == true) press_mousebotton = SDL_GetMouseState(NULL, NULL);
		if (press_mousebotton & SDL_BUTTON(SDL_BUTTON_LEFT))
		{

			total_coins += cost_to_play;
			press_bar = true;
			prev_press_mousebotton = false;
		}

		// GET SPACE BAR 
		if (keys[SDL_SCANCODE_SPACE])
		{

			press_bar = true;
			total_coins += cost_to_play;

		}
		if (prev_press_A == true) press_A = keys[SDL_SCANCODE_A];
		if (press_A == true && prev_press_A == true)
		{

			SDL_Log("You added 1000 coins");
			total_coins += 1000;
			prev_press_A = false;
		}

		text_screen(font_texture, cost_to_play, total_coins, coins_earned);

		Uint32 timeout = SDL_GetTicks() + 3000; // time spining
		while (press_bar)
		{

			SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
			//SDL_RenderClear(Game::renderer);
			SDL_RenderClear(Game::renderer);
			play_songs(1);

			while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
			{

				last_text_change_time = current_time;
				draw_images(sprite_texture, winner_array);
				text_screen(font_texture, cost_to_play, total_coins, coins_earned);
				SDL_RenderPresent(Game::renderer);

			}

			SDL_Log("PLAY AGAIN ");
			play_songs(3);
			press_bar = false;
			press_mousebotton = false;
			press_A = false;
			prev_press_mousebotton = true;
			prev_press_A = true;

			coins_earned = who_won(*sprite_texture_1, winner_array);

			total_coins += coins_earned;

		}




		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}