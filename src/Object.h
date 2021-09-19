#ifndef OBJECT_H_
#define OBJECT_H_
#include "Core.h"
#define SHOWHITBOXES

class Level;

enum class OBJ_TAGS
{
	OBJECT,
	BULLET,
	PARTICLE
};

/*
* Basic interface for all objects in the project,
* used mainly by levels
*/
class Object
{
public:
	Object(Level* level_, const Hitbox &hitbox_, OBJ_TAGS tag_, const Vector2& position_ = Vector2{0, 0}, const Vector2& velocity_ = Vector2{ 0, 0 });

	//Called from level loop
	virtual void update(const float frameTime_);
	virtual void draw();
	virtual void drawHbox(const SDL_Color& color_ = SDL_Color{ 0, 255, 0, 100 }); // - from draw()

	//Interface stuff
	virtual Hitbox getHitbox() const;
	const virtual Vector2 &getPosition() const;
	virtual void setVelocity(const Vector2& velocity_);
	const virtual Vector2 &getVelocity();
	virtual void setLevel(Level* level_);

	static Core* m_core;

	virtual ~Object();

	OBJ_TAGS m_tag;
	bool m_destroyed = false;

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	Level *m_level;
	Hitbox m_hitbox;
};

#endif