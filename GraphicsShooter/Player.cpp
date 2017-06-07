#include "Player.h"



Player::Player(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation) : Object(_position, _scale, _rotation), health(MAX_HEALTH)
{
}


Player::~Player()
{
}

bool Player::tryFire()
{
	--ammo;
	return ammo >= 0;
}

void Player::reload()
{
	if (!reloading)
	{
		reloading = true;
		timeSinceReloadStart = 0.f;
	}
}

void Player::update(float _dt)
{
	if (reloading)
	{
		timeSinceReloadStart += _dt;

		if (timeSinceReloadStart >= RELOAD_TIME)
		{
			//Replenish ammo
			ammo = CLIP_SIZE;
			reloading = false;
		}
	}
}

bool Player::isDead() const
{
	return health <= 0;
}

void Player::dealDamage(int amount)
{
	health -= amount;
	health = clamp(0, health, MAX_HEALTH);
}
