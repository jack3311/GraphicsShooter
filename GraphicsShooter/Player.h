#pragma once
#include "Object.h"

#define MAX_HEALTH 100
#define CLIP_SIZE 10u
#define RELOAD_TIME 2.f

class Player :
	public Object
{
private:
	int health;
	int ammo = CLIP_SIZE;
	float timeSinceReloadStart = 0.f;
	bool reloading = false;


public:
	Player(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	virtual ~Player();

	bool tryFire();
	void reload();

	void update(float _dt);

	bool isDead() const;
	void dealDamage(int amount);
};

