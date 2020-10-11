#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}
	AnimationUpdate();
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
	}
	if (Input::GetKeyDown(Key::Space))
	{
		m_moving = false;
		if (m_hasPhysics)
		{
			m_physBody->SetVelocity(vec3());

		}
		m_attacking = true;
		m_locked = true;

	}
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
		activeAnimation = ATTACK;

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
