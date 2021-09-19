/**************************
* 
*	Core is responsible for basic drawing stuff
*	And holding another systems (Input, Texture manager, etc)
* 
**************************/
#ifndef CORE_H_
#define CORE_H_
#include "TextureManager.h"
#include "AnimationManager.h"
#include "SoundManager.h"
#include "Input.h"
#include "Logger.h"
#include <sstream>
#include "SDL2_framerate.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL2_gfxPrimitives_font.h"
#include "SDL2_imageFilter.h"
#include "SDL2_rotozoom.h"
#include "TextManager.h"
#include "DataSaver.h"


class Core
{
public:
	const Vector2 targetResolution = { 1600, 900 };

	//System
	bool init();
	void close();

	//Primitives
	void drawFilledRect(const Vector2 &pos_, const Vector2 &size_, const SDL_Color &col_);
	void drawPoint(const Vector2 &pos_, float size_, const SDL_Color &col_);
	void drawLine(const Vector2 &p1_, const Vector2 &p2_, const SDL_Color &col_);
	void drawOutRect(const Vector2 &pos_, const Vector2 &size_, const SDL_Color &col_);
	void drawBezierCurve(const Vector2 &pos1_, const Vector2 &subPoint_, const Vector2 &pos2_, int s_, const SDL_Color &col_);

	//Texture creation
	SDL_Texture* createTexture(int w_, int h_);
	SDL_Texture* createTexture(const Vector2 &size_);
	SDL_Texture* loadTexture(const char* file_);
	SDL_Texture* createTextureFromSurface(SDL_Surface* sur_);

	//Texture render
	void renderTexture(SDL_Texture* tex_, int x_, int y_, int w_, int h_);
	void renderTexture(SDL_Texture* tex_, int x_, int y_);
	void renderTexture(SDL_Texture* tex_, int x_, int y_, int w_, int h_, float angle_, SDL_Point* center_, SDL_RendererFlip flip_);
	void renderTexture(SDL_Texture* tex_, int x_, int y_, float angle_, SDL_Point* center_, SDL_RendererFlip flip_);

	//Global render
	void fillRenderer(const SDL_Color &col_);
	void updateScreen();
	void setFS(int mode_);
	void setResolution(const Vector2 &res_);
	void setResolutionForced(const Vector2 &res_);

	//Text render
	enum class ALIGN_VER { TOP, CENTER, BOTTOM };
	enum class ALIGN_HOR { LEFT, CENTER, RIGHT };
	void quickDrawText(const char* s_, const Vector2& pos_, FONTS font_, ALIGN_VER vAlign_ = ALIGN_VER::TOP, ALIGN_HOR hAlign_ = ALIGN_HOR::LEFT, int hAmp_ = 0, int vAmp_ = 0);

	//Render target settings
	void setRenderTarget(SDL_Texture* tex_ = NULL);

	//Text functions
	TTF_Font* createFont(const char* s_, int fHeight_);
	SDL_Texture* createText(const char* textureText_, SDL_Color textColor_, TTF_Font* font_);

	//Sub systems
	Input input;
	TextureManager *textureManager = nullptr;
	AnimationManager *animationManager = nullptr;
	SoundManager* soundManager = nullptr;
	TextManager* textManager = nullptr;
	DataSaver *settings = nullptr;

private:
	int m_fullScreenMode = 0;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;
	Vector2 m_currentResolution = targetResolution;
	std::string m_windowName = "SDL core test";
};

template<class T>
std::string toString(T val)
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}

template<class T>
T toType(const std::string& val_)
{
	std::stringstream ss;
	ss << val_;
	T typed;
	ss >> typed;
	return typed;
}

#endif