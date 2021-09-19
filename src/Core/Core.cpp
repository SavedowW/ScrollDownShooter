#include "Core.h"

bool Core::init()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization error: " << SDL_GetError() << std::endl;
		return false;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cout << "IMG initialization error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_window = SDL_CreateWindow(m_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, targetResolution.x, targetResolution.y, SDL_WINDOW_SHOWN);
	if (m_window == NULL)
	{
		std::cout << "Window creation error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (m_renderer == nullptr) {
		std::cout << "Renderer creation error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 0, 512))
	{
		std::cout << "MIX ititialization error: " << Mix_GetError() << std::endl;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "TTF initialization error: " << TTF_GetError() << std::endl;
	}

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	m_fullScreenMode = 0;

	textureManager = new TextureManager(this);
	animationManager = new AnimationManager(this);
	soundManager = new SoundManager(this);
	textManager = new TextManager(this);
	settings = new DataSaver("settings.cfg");

	if (settings->load())
	{
		Logger::print("Loading settings from settings.cfg\n");
		//[video]
		Vector2 resolution;
		resolution.x = toType<float>(settings->m_variables["Video"]["Screen_width"]);
		resolution.y = toType<float>(settings->m_variables["Video"]["Screen_height"]);
		int resolutionMode = toType<int>(settings->m_variables["Video"]["Fullscreen_mode"]);
		setResolution(resolution);
		setFS(resolutionMode);
	}
	else
	{
		Logger::print("Cannot find settings.cfg, using default settings\n");
		setResolution(Vector2(1280, 720));
		setFS(0);
	}

	return true;
}

void Core::close()
{
	settings->m_variables.clear();
	settings->m_variables["Video"]["Screen_width"] = toString<float>(m_currentResolution.x);
	settings->m_variables["Video"]["Screen_height"] = toString<float>(m_currentResolution.y);
	settings->m_variables["Video"]["Fullscreen_mode"] = toString<float>(m_fullScreenMode);
	settings->save();

	delete textureManager;
	delete animationManager;
	//delete soundManager; - for some reason, it breaks everything
	delete textManager;
	delete settings;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Core::drawFilledRect(const Vector2& pos_, const Vector2& size_, const SDL_Color& col_)
{
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_Rect rect = { pos_.x, pos_.y, size_.x, size_.y };
	SDL_RenderFillRect(m_renderer, &rect);
}

void Core::drawPoint(const Vector2& pos_, float size_, const SDL_Color& col_)
{
	drawFilledRect({ pos_.x - size_ / 2, pos_.y - size_ / 2 }, { size_, size_ }, col_);
}

void Core::drawLine(const Vector2& p1_, const Vector2& p2_, const SDL_Color& col_)
{
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_RenderDrawLine(m_renderer, p1_.x, p1_.y, p2_.x, p2_.y);
}

void Core::drawOutRect(const Vector2& pos_, const Vector2& size_, const SDL_Color& col_)
{
	drawLine(pos_, pos_ + Vector2(size_.x, 0), col_);
	drawLine(pos_, pos_ + Vector2(0, size_.y), col_);
	drawLine(pos_ + size_, pos_ + Vector2(size_.x, 0), col_);
	drawLine(pos_ + size_, pos_ + Vector2(0, size_.y), col_);
}

void Core::drawBezierCurve(const Vector2& pos1_, const Vector2& subPoint_, const Vector2& pos2_, int s_, const SDL_Color &col_)
{
	Sint16 vx[3] = { pos1_.x, subPoint_.x, pos2_.x};
	Sint16 vy[3] = { pos1_.y, subPoint_.y, pos2_.y};
	bezierColor(m_renderer, vx, vy, 3, s_, *(Uint32*)(&col_));
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

SDL_Texture* Core::createTexture(int w_, int h_)
{
	return SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w_, h_);
}

SDL_Texture* Core::createTexture(const Vector2& size_)
{
	return SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size_.x, size_.y);
}

SDL_Texture* Core::loadTexture(const char* file_)
{
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, file_);
	if (!texture) {
		std::cout << "Texture loading problem: " << file_ << "  |  " << IMG_GetError() << std::endl;
	}
	return texture;
}

SDL_Texture* Core::createTextureFromSurface(SDL_Surface* sur_)
{
	return SDL_CreateTextureFromSurface(m_renderer, sur_);
}

void Core::renderTexture(SDL_Texture* tex_, int x_, int y_, int w_, int h_, float angle_, SDL_Point* center_, SDL_RendererFlip flip_)
{
	SDL_Rect dst;

	dst.x = x_;
	dst.y = y_;
	dst.w = w_;
	dst.h = h_;

	SDL_RenderCopyEx(m_renderer, tex_, NULL, &dst, angle_, center_, flip_);
}

void Core::renderTexture(SDL_Texture* tex_, int x_, int y_, float angle_, SDL_Point* center_, SDL_RendererFlip flip_)
{
	int w, h;
	SDL_QueryTexture(tex_, NULL, NULL, &w, &h);

	renderTexture(tex_, x_, y_, w, h, angle_, center_, flip_);
}

void Core::renderTexture(SDL_Texture* tex_, int x_, int y_, int w_, int h_)
{
	SDL_Rect dst;

	dst.x = x_;
	dst.y = y_;
	dst.w = w_;
	dst.h = h_;

	SDL_RenderCopy(m_renderer, tex_, NULL, &dst);
}

void Core::renderTexture(SDL_Texture* tex_, int x_, int y_)
{
	int w, h;
	SDL_QueryTexture(tex_, NULL, NULL, &w, &h);
	renderTexture(tex_, x_, y_, w, h);
}

void Core::fillRenderer(const SDL_Color& col_)
{
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_RenderClear(m_renderer);
}

void Core::updateScreen()
{
	SDL_RenderPresent(m_renderer);
}

void Core::setFS(int mode_)
{
	m_fullScreenMode = mode_;

	switch (mode_)
	{
	case (1):
			setResolutionForced(targetResolution);
			SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			setResolutionForced(m_currentResolution);
			break;

	case (2):
			setResolutionForced(targetResolution);
			SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
			setResolutionForced(m_currentResolution);
			break;

	default:
			SDL_SetWindowFullscreen(m_window, 0);
			break;
		
	}
}

//Properly changes resolution and saves the changes
void Core::setResolution(const Vector2& res_)
{
	int mode = m_fullScreenMode;

	setFS(0);

	m_currentResolution = res_;
	setResolutionForced(res_);

	setFS(mode);
}

//Changes render scaling and window size - only for internal use
void Core::setResolutionForced(const Vector2& res_)
{
	SDL_RenderSetScale(m_renderer, res_.x / targetResolution.x, res_.y / targetResolution.y);
	SDL_SetWindowSize(m_window, res_.x, res_.y);
}

void Core::quickDrawText(const char* s, const Vector2& pos, FONTS font_, ALIGN_VER vAlign, ALIGN_HOR hAlign, int hAmp, int vAmp)
{
	//Fix size calculation
	float w = 0;
	float h = 0;
	Font* font = (*textManager)[font_];
	for (int i = 0; s[i] != '\0' && s[i] != '\n' && s[i] != '\r'; i++)
	{
		w += float((*font)[s[i]]->m_w);
	}
	h = float(font->m_h);

	Vector2 offset;
	if (vAlign == ALIGN_VER::TOP)
		offset.y = 0;
	else if (vAlign == ALIGN_VER::CENTER)
		offset.y = h / 2;
	else if (vAlign == ALIGN_VER::BOTTOM)
		offset.y = h;

	if (hAlign == ALIGN_HOR::LEFT)
		offset.x = 0;
	else if (hAlign == ALIGN_HOR::CENTER)
		offset.x = w / 2;
	else if (hAlign == ALIGN_HOR::RIGHT)
		offset.x = w;

	Vector2 curPos = pos - offset;

	for (int i = 0; s[i] != '\0' && s[i] != '\r'; i++)
	{
		if (s[i] == '\n')
		{
			curPos.x = pos.x - offset.x;
			curPos.y += h;
			continue;
		}
		Vector2 shakeOffset;
		if (hAmp != 0 && vAmp != 0)
			shakeOffset = Vector2(rand() % hAmp - hAmp / 2, rand() % vAmp - vAmp / 2);
		else if (hAmp != 0 && vAmp == 0)
			shakeOffset = Vector2(rand() % hAmp - hAmp / 2, 0);
		else if (hAmp == 0 && vAmp != 0)
			shakeOffset = Vector2(0, rand() % vAmp - vAmp / 2);

		Vector2 temp = curPos + shakeOffset;

		renderTexture((*font)[s[i]]->getTexture(), temp.x, temp.y);
		curPos += Vector2{ float((*font)[s[i]]->m_w), 0 };
	}
}

void Core::setRenderTarget(SDL_Texture* tex_)
{
	int i = SDL_SetRenderTarget(m_renderer, tex_);
	if (i != 0)
		std::cout << i << std::endl;
}

TTF_Font* Core::createFont(const char* s_, int fHeight_)
{
	return TTF_OpenFont(s_, fHeight_);
}

SDL_Texture* Core::createText(const char* textureText_, SDL_Color textColor_, TTF_Font* font_)
{
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Blended(font_, textureText_, textColor_);
	if (textSurface == NULL)
	{
		std::cout << "textSurface creating error | " << TTF_GetError() << std::endl;
		return NULL;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
	if (texture == NULL)
	{
		std::cout << "Text texture creating error | " << SDL_GetError() << std::endl;
		return NULL;
	}
	SDL_FreeSurface(textSurface);
	return texture;
}
