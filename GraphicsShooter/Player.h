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

	bool tryFire();
	void reload();
	bool isReloading() const;
	int getAmmo() const;

	void update(float _dt);

	bool isDead() const;
	void dealDamage(float _amount);
	int getHealth() const;

	void addScore(int _amount);
	int getScore() const;


	void infiniteAmmoFor(float _duration);
	void shieldFor(float _duration);

	bool hasInfiniteAmmo() const;
	bool hasShield() const;
};

