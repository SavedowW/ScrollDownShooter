#ifndef PARTICLE_H_
#define PARTICLE_H_
#include "Object.h"

class Particle : public Object
{
public:
	Particle(Level* level_, float angle_, const Vector2& position_ = Vector2{ 0, 0 }, const SDL_Point &center_ = {0, 0});
	virtual void update(const float frameTime_) override;
	virtual void draw() override;

	virtual ~Particle();

protected:
	Animation_t m_anim;
	SDL_Point m_center;
	float m_angle;

};

//======== HIT 1 =============
//Emitted by destroyed Missle1 in the same direction
class Particle_hit1 : Particle
{
public:
	Particle_hit1(Level* level_, float angle_, const Vector2& position_ = Vector2{ 0, 0 });

private:

};

//======== Explosion 1 =============
//Emitted by destroyed swarmer
class Particle_expl1 : Particle
{
public:
	Particle_expl1(Level* level_, const Vector2& position_ = Vector2{ 0, 0 });

private:

};

//======== Explosion 2 =============
//Emitted by destroyed health icon
class Particle_expl2 : Particle
{
public:
	Particle_expl2(Level* level_, const Vector2& position_ = Vector2{ 0, 0 });

private:

};

//======== Explosion 3 =============
//Emitted by destroyed player
class Particle_expl3 : Particle
{
public:
	Particle_expl3(Level* level_, const Vector2& position_ = Vector2{ 0, 0 });

private:

};

//======== Explosion 4 =============
//Emitted by destroyed homing missle
class Particle_expl4 : Particle
{
public:
	Particle_expl4(Level* level_, const Vector2& position_ = Vector2{ 0, 0 });

private:

};

#endif