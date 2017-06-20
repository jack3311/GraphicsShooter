#include "Player.h"



Player::Player(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation) : Object(_position, _scale, _rotation), health(MAX_HEALTH)
{
}


Player::~Player()
{
}

bool Player::tryFire()
{
	if (hasInfiniteAmmo())
		return true;

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

bool Player::isReloading() const
{
	return reloading;
}

int Player::getAmmo() const
{
	return ammo;
}

void Player::resetAmmo()
{
	reloading = false;
	ammo = CLIP_SIZE;
}

void Player::update(float _dt)
{
	lifetime += _dt;

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

void Player::dealDamage(float _amount)
{
	health -= _amount;
	health = clamp(0.f, health, MAX_HEALTH);
}

int Player::getHealth() const
{
	return static_cast<int>(health);
}

void Player::setHealth(float _amount)
{
	health = _amount;
}

void Player::addScore(int _amount)
{
	score += _amount;
}

int Player::getScore() const
{
	return score;
}

void Player::infiniteAmmoFor(float _duration)
{
	if (infiniteAmmoEndTime < lifetime)
	{
		infiniteAmmoEndTime = lifetime + _duration;
	}
	else
	{
		infiniteAmmoEndTime += _duration;
	}
}

void Player::shieldFor(float _duration)
{
	if (shieldEndTime < lifetime)
	{
		shieldEndTime = lifetime + _duration;
	}
	else
	{
		shieldEndTime += _duration;
	}
}

bool Player::hasInfiniteAmmo() const
{
	return infiniteAmmoEndTime > lifetime;
}

bool Player::hasShield() const
{
	return shieldEndTime > lifetime;
}

void Player::serialiseSpecific(std::ostringstream & _oss)
{
	//Lifetime
	_oss << lifetime << " ";
	
	//End times
	_oss << infiniteAmmoEndTime << " ";
	_oss << shieldEndTime << " ";

	//Health
	_oss << health << " ";

	//Ammo
	_oss << ammo << " ";

	//Reloading
	_oss << reloading << " ";

	//Score
	_oss << score << " ";
}

void Player::deserialiseSpecific(std::istringstream & _iss)
{
	_iss >> lifetime;
	_iss >> infiniteAmmoEndTime;
	_iss >> shieldEndTime;
	_iss >> health;
	_iss >> ammo;
	_iss >> reloading;
	_iss >> score;
}
