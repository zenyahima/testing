#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BackEnd.h"

//makes it topdown
//#define TOPDOWN


enum AnimEnums
{
	IDLELEFT,
	IDLERIGHT,
	
	//Only in Top down
#ifdef TOPDOWN
	IDLEUP,
	IDLEDOWN,
#endif

	WALKLEFT,
	WALKRIGHT,

	//Only in Top down
#ifdef TOPDOWN
	WALKUP,
	WALKDOWN,
#endif
	
	ATTACKLEFT,
	ATTACKRIGHT,

	//Only in Top down
#ifdef TOPDOWN
	ATTACKUP,
	ATTACKDOWN
#endif
};

enum AnimTypes
{
#ifdef TOPDOWN
	IDLE = 0,
	WALK = 4,
	ATTACK = 8
#endif
#ifndef TOPDOWN
	IDLE = 0,
	WALK = 2,
	JUMPUP = 4
#endif
};

enum AnimDir
{
	LEFT,
	RIGHT,
	//Only in Top Down
#ifdef TOPDOWN
	UP,
	DOWN
#endif
};

class Player
{
public:
	Player();
	Player(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);

	void InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr)
	{
		//store references to the components
		m_sprite = sprite;
		m_animController = controller;
		m_transform = transform;
		m_hasPhysics = hasPhys;
		if (hasPhys)
		{
			m_physBody = body;
		}
		//initialize UVs
		m_animController->InitUVs(fileName);

		//loads the texture and sets width and height
		m_sprite->LoadSprite(fileName, width, height, true, m_animController);
		m_animController->SetVAO(m_sprite->GetVAO());
		m_animController->SetTextureSize(m_sprite->GetTextureSize());

		//loads in the animations
		nlohmann::json animations = File::LoadJSON(animationJSON);

		//idle animations\\

		/*for (int i = 0; i < animationsNames.size(); i++)
		{
		m_animController->AddAnimation(animations[animationsNames[i]].get<Animation>());
		}*/\
		
		m_animController->AddAnimation(animations["IdleLeft"].get<Animation>()); //value of 0
		m_animController->AddAnimation(animations["IdleRight"].get<Animation>()); //value of 1
		m_animController->AddAnimation(animations["RunLeft"].get<Animation>()); //value of 2+0 = 2
		m_animController->AddAnimation(animations["RunRight"].get<Animation>()); //value of 2+1 = 3
		m_animController->AddAnimation(animations["JumpUpLeft"].get<Animation>());//value of 4+0 = 4
		m_animController->AddAnimation(animations["JumpUpRight"].get<Animation>()); // value of 4+1 = 5
		m_animController->AddAnimation(animations["JumpDownLeft"].get<Animation>());
		m_animController->AddAnimation(animations["JumpDownRight"].get<Animation>());

//
//		//set default animation
		m_animController->SetActiveAnim(IDLELEFT);
//
		//m_animController->SetActiveAnim(0);

	}

	void Update();
	void MovementUpdate();
	void AnimationUpdate();

private:
	void SetActiveAnimation(int anim);

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//are you currently jumping
	bool m_jumping = false;
	//Are you currently attacking?????
	bool m_attacking = false;
	//Have we locked the player from moving during this animation?
	bool m_locked = false;

	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;

	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;
	//Does this player have physics?
	bool m_hasPhysics = false;

	//Default animation direction (feel free to change this to suit your game. If you're making a side-scroller, left or right would be better
	AnimDir m_facing = RIGHT;
};

#endif // !__PLAYER_H__