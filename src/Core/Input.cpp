#include "Input.h"

Input::Input()
{
}

void Input::handleInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case (SDL_QUIT):
			send(EVENTS::QUIT, 1);
			break;

		case (SDL_KEYDOWN):
		{
			if (e.key.repeat)
				break;
			auto res = KEY_EVENTS.find(e.key.keysym.sym);
			if (res != KEY_EVENTS.end())
			{
				send(res->second, 1);
				updateAxis(res->second, 1);
			}
			
		}
			break;

		case (SDL_KEYUP):
		{
			auto res = KEY_EVENTS.find(e.key.keysym.sym);
			if (res != KEY_EVENTS.end())
			{
				send(res->second, -1);
				updateAxis(res->second, -1);
			}
		}
		break;
		}
	}
}

//must be called by InputReactor
void Input::subscribe(EVENTS ev_, subscriber sub_)
{
	if (!sub_)
		throw std::runtime_error("Input reactor is nullptr");

	m_subscribers[(int)ev_].push_back(sub_);
	sub_->subscribed_at.push_back(ev_);
}

//Automatically called when InputReactor is destroyed
void Input::unsubscribe(EVENTS ev_, subscriber sub_)
{
	std::vector<subscriber>& v = m_subscribers[(int)ev_];
	v.erase(remove(v.begin(), v.end(), sub_), v.end());
}

Vector2 Input::getAxis()
{
	return m_axis;
}

void Input::updateAxis(EVENTS ev_, float scale_)
{
	bool required = true;

	switch (ev_)
	{
	case (EVENTS::UP):
		m_qDir.up = scale_ + 1;
		break;

	case (EVENTS::DOWN):
		m_qDir.down = scale_ + 1;
		break;

	case (EVENTS::LEFT):
		m_qDir.left = scale_ + 1;
		break;

	case (EVENTS::RIGHT):
		m_qDir.right = scale_ + 1;
		break;

	default:
		required = false;
		break;
	}

	if (!required)
		return;

	if (m_qDir.right)
		m_axis.x = 1;
	else if (m_qDir.left)
		m_axis.x = -1;
	else
		m_axis.x = 0;

	if (m_qDir.down)
		m_axis.y = 1;
	else if (m_qDir.up)
		m_axis.y = -1;
	else
		m_axis.y = 0;

	m_axis.normalise();

	send(EVENTS::AXISUPD, 1);
}

void Input::send(EVENTS ev_, float val_)
{
	for (subscriber& sub : m_subscribers[(int)ev_])
	{
		if (sub->isInputEnabled())
		{
			switch (ev_)
			{
			case (EVENTS::QUIT):
				sub->input_quit(val_);
				break;

			case (EVENTS::LEFT):
				sub->input_left(val_);
				break;

			case (EVENTS::RIGHT):
				sub->input_right(val_);
				break;

			case (EVENTS::UP):
				sub->input_up(val_);
				break;

			case (EVENTS::DOWN):
				sub->input_down(val_);
				break;

			case (EVENTS::OK):
				sub->input_ok(val_);
				break;

			case (EVENTS::CANCEL):
				sub->input_cancel(val_);
				break;

			case (EVENTS::AXISUPD):
				sub->input_axisupd(val_);
				break;

			case (EVENTS::SHIFT):
				sub->input_shift(val_);
				break;

			case (EVENTS::CTRL):
				sub->input_ctrl(val_);
				break;

			default:
				throw std::runtime_error("Unhandled input event!\n");
				break;
			}
		}
	}
}

void InputReactor::input_quit(const float scale_)
{
}

void InputReactor::input_left(const float scale_)
{
}

void InputReactor::input_right(const float scale_)
{
}

void InputReactor::input_up(const float scale_)
{
}

void InputReactor::input_down(const float scale_)
{
}

void InputReactor::input_axisupd(const float scale_)
{
}

void InputReactor::input_ok(const float scale_)
{
}

void InputReactor::input_cancel(const float scale_)
{
}

void InputReactor::input_shift(const float scale_)
{
}

void InputReactor::input_ctrl(const float scale_)
{
}

void InputReactor::setInputEnabled(bool inputEnabled_)
{
	m_inputEnabled = inputEnabled_;
}

bool InputReactor::isInputEnabled()
{
	return m_inputEnabled;
}
