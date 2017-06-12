#pragma once
#include "Object.h"

#define MAX_HEALTH 100.f
#define CLIP_SIZE 10u
#define RELOAD_TIME 2.f

class Player :
	public Object
{
private:
	float lifetime = 0.f;
	float infiniteAmmoEndTime = 0.f;
	float shieldEndTime = 0.f;

	float health;
	int ammo = CLIP_SIZE;
	float timeSinceReloadStart = 0.f;
	bool reloading = false;
	int score = 0;


public:
	Player(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	virtual ~Player();

	///
	/// tryFire:
	/// returns whether the player is able to fire a bullet at this instant
	///
	bool tryFire();

	///
	/// reload:
	/// Starts the reload process
	///
	void reload();

	///
	/// isReloading:
	/// returns whether the player is currently reloading
	///
	bool isReloading() const;

	int getAmmo() const;

	///
	/// update:
	/// Updates the player
	/// _dt - the delta time
	///
	void update(float _dt);

	///
	/// isDead:
	/// returns whether the player is dead
	///
	bool isDead() const;

	///
	/// dealDamage:
	/// Adjusts the player's health by a given amount
	/// _amount - the amount
	///
	void dealDamage(float _amount);
	int getHealth() const;

	///
	/// addScore:
	/// Increases the player's score by a given amount
	/// _amount - the amount
	///
	void addScore(int _amount);
	int getScore() const;

	///
	/// infiniteAmmoFor:
	/// Allows the player to fire infinitely for a given duration
	/// _duration - the duration
	///
	void infiniteAmmoFor(float _duration);

	///
	/// shieldFor:
	/// Allows the player a shield for a given duration
	/// _duration - the duration
	///
	void shieldFor(float _duration);

	bool hasInfiniteAmmo() const;
	bool hasShield() const;
};

