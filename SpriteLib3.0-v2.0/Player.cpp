#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform)
{
	//store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;

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




void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}
	AnimationUpdate();
	//trigger to check if level is finished
	//if (m_transform->GetPositionX() == 0.f && m_transform->GetPositionY() == 0.f)
	//{
	//	MessageBox(NULL, "You completed the level!", "Finish", MB_OK);
	//	exit(0);
	//}
}
bool Player::IsLocked()
{
	return m_locked;
}

void Player::MovementUpdate()
{
	m_moving = false;
	if (m_hasPhysics)
	{
		float speed = 10.f;
		vec3 vel = vec3(0.f, 0.f, 0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
		}
#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			vel = vel + vec3(0.f, 1.f, 0.f);
			m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			vel = vel + vec3(0.f, -1.f, 0.f);
			m_facing = DOWN;
			m_moving = true;
		}
#endif
		if (Input::GetKey(Key::A))
		{
			vel = vel + vec3(-1.f, 0.f, 0.f);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			vel = vel + vec3(1.f, 0.f, 0.f);
			m_facing = RIGHT;
			m_moving = true;
		}

		m_physBody->SetVelocity(vel * speed);
	}
	else
	{
		//regular movement
		float speed = 15.f;
#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() - (speed * Timer::deltaTime));
			m_facing = DOWN;
			m_moving = true;
		}
#endif
		if (Input::GetKey(Key::A))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
			
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
			m_facing = RIGHT;
			
			m_moving = true;

		}

		if (Input::GetKeyDown(Key::Space))
		{
			////might have to improve this-> will def need to improve
			//m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			//m_moving = false;
			///*if (m_hasPhysics)
			//{
			//	m_physBody->SetVelocity(vec3());

			//}*/
			////m_attacking = true;
			//m_locked = true;
			//m_jumping = true;


		}
	}
	
}
void Player::SetMoving(bool move)
{
	m_moving = move;
}
void Player::SetLocked(bool lock)
{
	m_locked = lock;
}
void Player::SetJumping(bool jump)
{
	m_jumping = jump;
}


void Player::AnimationUpdate()
{
	int activeAnimation = 0;
	if (m_moving)
	{
		//puts it into walk category
		activeAnimation = WALK;

	}
	else if (m_attacking)
	{
		//activeAnimation = ATTACK;

		//check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//will auto set to idle
			m_locked = false;
			m_attacking = false;
			//resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	//might need to change this too
	else if (m_jumping)
	{
		activeAnimation = JUMPUP;
		//check if the jump animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//will auto set to idle
			m_locked = false;
			m_jumping = false;
			//resets the jump animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}

	}
	else
	{
		activeAnimation = IDLE;
	}
	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	//goes through the array from "addanimations", the order added corresponds to a number

	m_animController->SetActiveAnim(anim);
}
