//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    MessageType.h
//  Description  :    Represents a type of game update packet
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

enum MessageType : unsigned __int8
{
	ENEMY_POSITION_UPDATE,
	RESIZE_ENEMIES,
	ENEMY_TURRET_FIRED,
	ENEMY_FIRED_BULLET,

	BULLET_POSITION_UPDATE,
	RESIZE_BULLETS,

	POWERUP_POSITION_UPDATE,
	RESIZE_POWERUPS,

	PLAYER_POSITION_UPDATE,
	PLAYER_SPECIFIC_UPDATE,
	PLAYER_TRY_FIRE,
	PLAYER_HIT_BY_BULLET,
	PLAYER_COLLECTED_POWERUP,
	PLAYER_HIT_BY_DRILL,

	PLAYER_LEAVE,

	GAME_OVER,

	MESSAGETYPE_NUM_ITEMS
};