#ifndef SHOOTER_H_
#define SHOOTER_H_
#include "Core.h"

class Level;
class Object;
class Player;

//================= SHOOTER BASE ==========================
class Shooter
{
public:
	Shooter(Level* level_, const Vector2* origin_, const Vector2& centerOffset_);

	virtual void setLevel(Level* level_);

	virtual void BeginShooting() = 0;
	virtual void StopShooting() = 0;

	virtual void EnterPrecise();
	virtual void QuitPrecise();

	virtual void update(const float frameTime_) = 0;
	virtual void draw();

	virtual bool getPrecise();
	virtual bool getState() = 0;

	virtual ~Shooter();

protected:
	bool m_isPrecise = false;
	Level* m_level;
	const Vector2* m_origin;
	Vector2 m_centerOffset;

};

// ========== SHOOTER BASIC ==============
// Controlled by player
// Normal: 2 rocket launchers
class Shooter_basic : public Shooter
{
public:
	Shooter_basic(Level* level_, const Vector2* origin_, const Vector2& centerOffset_);

	void BeginShooting() final;
	void StopShooting() final;

	void update(const float frameTime_) final;

	bool getState() final;

protected:
	const float m_period = 0.05;
	float m_timePassed;
	enum class IsShooting
	{
		NOSHOOT,
		LEFT,
		RIGHT
	} m_isShooting = IsShooting::NOSHOOT;

};

// ========== SHOOTER LASER ==============
// Controlled by player
// Normal: 1 auto-aim laser
class Shooter_laser : public Shooter
{
public:
	Shooter_laser(Level* level_, const Vector2* origin_, const Vector2& centerOffset_);
	void BeginShooting() final;
	void StopShooting() final;
	void update(const float frameTime_) final;

	bool getState() final;
	void draw() final;

protected:
	const float m_period = 0.15;
	float m_timePassed;
	bool m_isShooting = false;

	std::shared_ptr<Object> m_target = nullptr;
	Texture_t spr_flash;
};

// ========== SHOOTER SPARKLER TARGETED ==============
// Controlled by enemy
// Normal: 1 auto-aim laser
class Shooter_sparkler_targeted : public Shooter
{
public:
	Shooter_sparkler_targeted(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_);

	void BeginShooting() final;
	void StopShooting() final;

	void update(const float frameTime_) final;

	bool getState() final;

protected:
	Player* m_player;
	int m_emitted = 0;
	const int m_sequenceLen = 3;
	const float m_singlePeriod = 0.2;
	const float m_sequencePeriod = 0.6;
	float m_timePassed = 0;
	bool m_isShooting = false;

};

// ========== SHOOTER SPARKLER CIRCULAR ==============
// Controlled by enemy
// Normal: circular waves of projectiles
class Shooter_sparkler_circular : public Shooter
{
public:
	Shooter_sparkler_circular(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_);

	void BeginShooting() final;
	void StopShooting() final;

	void update(const float frameTime_) final;

	bool getState() final;

protected:
	const int density = 23;
	const float m_period = 0.6;
	float m_timePassed = 0;
	bool m_isShooting = false;

};

// ========== SHOOTER SPARKLER SPIRAL ==============
// Controlled by enemy
// Normal: spiral waves of projectiles
class Shooter_sparkler_spiral : public Shooter
{
public:
	Shooter_sparkler_spiral(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_);

	void BeginShooting() final;
	void StopShooting() final;

	void update(const float frameTime_) final;

	bool getState() final;

protected:
	const float m_period = 0.02;
	float m_timePassed = 0;
	const int density = 30;
	int iter = 1;
	bool m_isShooting = false;

};


// ========== SHOOTER HOMING ==============
// Controlled by player
// Normal: 2 homing missles launcher
class Shooter_homing : public Shooter
{
public:
	Shooter_homing(Level* level_, const Vector2* origin_, const Vector2& centerOffset_);

	void BeginShooting() final;
	void StopShooting() final;

	void update(const float frameTime_) final;

	bool getState() final;

protected:
	const float m_period = 0.3;
	int m_emitted = 0;
	const int m_sequenceLen = 2;
	const float m_singlePeriod = 0.1;
	const float m_sequencePeriod = 0.3;
	float m_timePassed;
	bool m_isShooting = false;

};

#endif