/*
 *	EvenShooter
 *	ゲームオブジェクト
 *	2012 / 04 / 08
 *	jskny
*/


#pragma once

#ifndef __EVENSHOOTER_GAMEOBJECT_INCLUDE_GUARD_37819794691982_142_121_143__
#define __EVENSHOOTER_GAMEOBJECT_INCLUDE_GUARD_37819794691982_142_121_143__


#include	<boost/shared_ptr.hpp>


// ゲームオブジェクト
class ObjectGame;
// ゲームパラメータ
class ActorParam;


typedef boost::shared_ptr<ObjectGame> ObjectGamePtr;


class ObjectGame
{
public:
	float		x, y;


	ObjectGame() :
		x(0.0f), y(0.0f)
	{
	}


};


// ゲームパラメータ
class ActorParam
{
public:
	float		x, y;
	int		life;
	int		r;
unsigned	int		index;
unsigned	int		type;


	ActorParam() :
		x(0.0f), y(0.0f),
		life(0), r(0),
		index(0), type(0)
	{
	}


};


#endif // __EVENSHOOTER_GAMEOBJECT_INCLUDE_GUARD_37819794691982_142_121_143__

