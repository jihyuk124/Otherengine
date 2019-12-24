﻿#include "Game.h"
#include "SDL.h"
#include "Exception.h"
#include "StringUtils.h"

NEG_BEGIN

static constexpr auto screen_w = 1024;
static constexpr auto screen_h = 768;

SDL_Window* create_window()
{	
	const auto window = SDL_CreateWindow("No Engine Game", 100, 100, screen_w, screen_h, 0);
	if (!window) throw sdl_error{fmt("Failed to create window: ", SDL_GetError())};
	return window;
}

SDL_Renderer* create_renderer(SDL_Window* const window)
{
	const auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) throw sdl_error{fmt("Failed to create renderer: ", SDL_GetError())};
	return renderer;
}

game::game()
	:window_{create_window(), SDL_DestroyWindow},
	renderer_{create_renderer(window_.get()), SDL_DestroyRenderer},
	paddle_pos_{20, screen_h/2},
	ball_pos_{screen_w/2, screen_h/2},
	is_running_{true}
{
}

game::~game() = default;

void game::run_loop()
{
	while (is_running_)
	{
		process_input();
		update_game();
		generate_output();
	}
}

void game::shutdown()
{
	is_running_ = false;
}

void game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			shutdown();
			break;
		default:
			break;
		}
	}

	const auto keyboard = SDL_GetKeyboardState(nullptr);
	if (keyboard[SDL_SCANCODE_ESCAPE]) shutdown();
}

void game::update_game()
{
}

void game::generate_output()
{
	SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 255, 255);
	SDL_RenderClear(renderer_.get());

	SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
	constexpr auto thickness = 15;
	const SDL_Rect rects[]
	{
		{0, 0, screen_w, thickness},
		{0, screen_h - thickness, screen_w, thickness},
		{screen_w - thickness, 0, thickness, screen_w},
		{ball_pos_.x - thickness/2, ball_pos_.y - thickness/2, thickness, thickness},
		{static_cast<int>(paddle_pos_.x - thickness/2.f), static_cast<int>(paddle_pos_.y - 50), thickness, 100}
	};
	for (auto&& rect : rects)
	{
		SDL_RenderFillRect(renderer_.get(), &rect);
	}
	
	SDL_RenderPresent(renderer_.get());
}

game::sdl_raii::sdl_raii()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw sdl_error{fmt("Unable to initialize SDL: ", SDL_GetError())};
}

game::sdl_raii::~sdl_raii()
{
	SDL_Quit();
}

NEG_END
