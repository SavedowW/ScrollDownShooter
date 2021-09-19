#ifndef INPUT_H_
#define INPUT_H_
#include <SDL.h>
#include <SDL_Image.h>
#include "Vector2.h"
#include <map>
#include <vector>
#include <functional>

//List of possible events
//Anything that is inherited from InputReactor can subscribe to them
enum class EVENTS
{
	QUIT = 0,
	LEFT, //A, left
	RIGHT, //D, right
	UP, //W, up
	DOWN, //S, down
	AXISUPD,
	OK, //Z, enter
	CANCEL, //X, esc
	SHIFT,
	CTRL,
	NONE
};

//Struct for current directions
struct QDir
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
};

class InputReactor;

typedef InputReactor* subscriber;

class Input
{
public:
	Input();
	void handleInput();
	void subscribe(EVENTS ev_, subscriber sub_);
	void unsubscribe(EVENTS ev_, subscriber sub_);
	Vector2 getAxis();

private:
	void updateAxis(EVENTS ev_, float scale_);
	void send(EVENTS ev_, float val_);
	std::vector<subscriber> m_subscribers[(int)EVENTS::NONE];
	const std::map<SDL_Keycode, EVENTS> KEY_EVENTS = {
		{SDLK_LEFT, EVENTS::LEFT},
		{SDLK_a, EVENTS::LEFT},
		{SDLK_RIGHT, EVENTS::RIGHT},
		{SDLK_d, EVENTS::RIGHT},
		{SDLK_UP, EVENTS::UP},
		{SDLK_w, EVENTS::UP},
		{SDLK_DOWN, EVENTS::DOWN},
		{SDLK_s, EVENTS::DOWN},
		{SDLK_RETURN, EVENTS::OK},
		{SDLK_z, EVENTS::OK},
		{SDLK_ESCAPE, EVENTS::CANCEL},
		{SDLK_x, EVENTS::CANCEL},
		{SDLK_LSHIFT, EVENTS::SHIFT},
		{SDLK_LCTRL, EVENTS::CTRL}
	};

	QDir m_qDir;
	Vector2 m_axis;
};


class InputReactor
{
public:
	//Input reactor needs pointer to  Input
	InputReactor(Input* input_) :
		m_input(input_)
	{
	}

	//That could be a meme
	virtual void input_quit(const float scale_);
	virtual void input_left(const float scale_);
	virtual void input_right(const float scale_);
	virtual void input_up(const float scale_);
	virtual void input_down(const float scale_);
	virtual void input_axisupd(const float scale_);
	virtual void input_ok(const float scale_);
	virtual void input_cancel(const float scale_);
	virtual void input_shift(const float scale_);
	virtual void input_ctrl(const float scale_);

	//All events reactor is subscribed at
	std::vector<EVENTS> subscribed_at;

	void setInputEnabled(bool inputEnabled_);
	bool isInputEnabled();

	//InputReactor automatically removes itself from subscribers
	virtual ~InputReactor()
	{
		for (EVENTS ev : subscribed_at)
		{
			m_input->unsubscribe(ev, this);
		}
	}

private:
	Input* m_input;
	bool m_inputEnabled = false;

};

#endif