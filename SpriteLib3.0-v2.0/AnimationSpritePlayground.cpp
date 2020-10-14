#include "AnimationSpritePlayground.h"
#include "Utilities.h"
#include "Timer.h"
//global variable
int numFootContacts;
class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {
		//std::cout << "this is running" << std::endl;
		////check if fixture A was the foot sensor
		//void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		//std::cout << int(fixtureUserData) << std::endl;
		//if ((int)fixtureUserData == 0)
		//	numFootContacts++;
		////check if fixture B was the foot sensor
		//fixtureUserData = contact->GetFixtureB()->GetUserData();
		//std::cout << int(fixtureUserData) << std::endl;
		//if ((int)fixtureUserData ==0)
		//	numFootContacts++;
		numFootContacts++;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetLocked(false);
	}

	void EndContact(b2Contact* contact) {
		//std::cout << "this is running 2" << std::endl;
		////check if fixture A was the foot sensor
		//void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		//std::cout << int(fixtureUserData) << std::endl;
		//if ((int)fixtureUserData == 0)
		//	numFootContacts--;
		////check if fixture B was the foot sensor
		//fixtureUserData = contact->GetFixtureB()->GetUserData();
		//std::cout << int(fixtureUserData) << std::endl;
		//if ((int)fixtureUserData == 0)
		//	numFootContacts--;
		numFootContacts--;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetLocked(true);
	}
};

MyContactListener myContactListenerInstance;

AnimationSpritePlayground::AnimationSpritePlayground(std::string name)
	:Scene(name)
{
	
	m_gravity = b2Vec2(0.f, -800.f);
	m_physicsWorld->SetGravity(m_gravity);
	
	m_physicsWorld->SetContactListener(&myContactListenerInstance);

}

void AnimationSpritePlayground::InitScene(float windowWidth, float windowHeight)
{
	//dynamically allocates the register
	m_sceneReg = new entt::registry;
	numFootContacts = 0;
	physics = true;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//Scene::CreateCameraEntity();
	//setup main camera entity
	{
		//creates camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);
		//creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));

	}

	//setup end of level Flag
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "flag.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-100.f, 403.f, 0.f));

	}

	//setup sprite guy
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//set up components
		std::string fileName = "spritesheets/guy.png";
		std::string animations = "spritesheet_math2.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 12, 16, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity));


		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));
		//tempDef.angle = Transform::ToRadians(45.f);

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
		////add main fixture
		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->CreateFixture(&myFixtureDef);
		

		//shape definition for main fixture
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(6, 8, b2Vec2(0, -8), 0);
		/*polygonShape.SetAsBox(1, 2);*/

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;

		//add foot sensor fixture
		
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)1);


	}

	//
	//ground
	Scene::BoxMaker(250, 35, 0, -20, 0, 1); 

	//boundaries
	Scene::BoxMaker(10, 2000, -125, -0, 0, 0);
	Scene::BoxMaker(10, 2000, 125, 0, 0, 0);

	float i = 5;

	//platforms
	Scene::BoxMaker(20, 3, -75, 20 + i, 0, 1);
	Scene::BoxMaker(20, 3, -25, 40 + (i * 2), 0, 1);
	Scene::BoxMaker(20, 3, -75, 55 + (i * 3), 0, 1);
	Scene::BoxMaker(20, 3, -25, 70 + (i * 4), 0, 1);
	Scene::BoxMaker(20, 3, 25, 85 + (i * 5), 0, 1);
	Scene::BoxMaker(20, 3, 75, 85 + (i * 6), 0, 1);
	Scene::BoxMaker(20, 3, 105, 105 + (i * 7), 0, 1);
	Scene::BoxMaker(20, 3, 65, 125 + (i * 8), 0, 1);
	Scene::BoxMaker(20, 3, 35, 145 + (i * 9), 0, 1);
	Scene::BoxMaker(20, 3, -20, 145 + (i * 10), 0, 1);
	Scene::BoxMaker(5, 3, -55, 160 + (i * 11), 0, 1);
	Scene::BoxMaker(5, 3, -90, 175 + (i * 12), 0, 1);
	Scene::BoxMaker(20, 3, -40, 195 + (i * 13), 0, 1);
	Scene::BoxMaker(70, 3, 40, 205 + (i * 14), 0, 1);
	Scene::BoxMaker(20, 3, 105, 220 + (i * 15), 0, 1);
	Scene::BoxMaker(20, 3, 70, 240 + (i * 16), 0, 1);
	Scene::BoxMaker(20, 3, 35, 255 + (i * 17), 0, 1);
	Scene::BoxMaker(40, 3, -15, 275 + (i * 18), 325, 1);
	Scene::BoxMaker(60, 3, -80, 300 + (i * 19), 0, 1);
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
//
//
}

void AnimationSpritePlayground::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	Scene::AdjustScrollOffset();
	player.Update();
	
	
	
}

void AnimationSpritePlayground::KeyboardHold()
{


	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float speed = 10.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	//togle to physics movement
	if (Input::GetKey(Key::Q))
	{
		physics = true;
	}
	//togle to direct movement
	if (Input::GetKey(Key::E))
	{
		physics = false;
	}
	
	if (Input::GetKey(Key::Shift))
	{
		speed *= 7.f;
	}

	if (physics)
	{
		
		/*if (Input::GetKey(Key::W))
		{
			vel += b2Vec2(0.f, 1.f);
		}*/
		/*if (Input::GetKey(Key::S))
		{
			vel += b2Vec2(0.f, -1.f);
		}*/

		if (Input::GetKey(Key::A))

		{
			//if (numFootContacts >= 1) 
			{
				vel += b2Vec2(-1.f, 0.f);
				player.GetBody()->SetLinearVelocity(speed * vel);

			}
			//else
			{
				//ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetMoving(false);
			}
		}
		if (Input::GetKey(Key::D))
		{
			//if (numFootContacts >= 1) 
			{
				vel += b2Vec2(1.f, 0.f);
				player.GetBody()->SetLinearVelocity(speed * vel);


			}
			//else
			{
				//ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetMoving(false);

			}
		}
		if (Input::GetKey(Key::Space))
		{
			if (numFootContacts >= 1)
			{
				//force of the jump
				float impulse = player.GetMass() * 1000;
				//each time step the force is applied, gravity gets a chance to push back
				player.GetBody()->ApplyLinearImpulse(b2Vec2(0, impulse), player.GetBody()->GetWorldCenter(), true);

				ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetMoving(false);
				ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetLocked(true);
				ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetJumping(true);
			}


		}
	}
	else
	{
		if (Input::GetKey(Key::A))
		{
			player.GetBody()->SetTransform(b2Vec2((player.GetPosition().x - (speed * Timer::deltaTime)), player.GetPosition().y), 0);
		}
		if (Input::GetKey(Key::D))
		{
			player.GetBody()->SetTransform(b2Vec2((player.GetPosition().x + (speed * Timer::deltaTime)), player.GetPosition().y), 0);
		}
		if (Input::GetKey(Key::W))
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, (player.GetPosition().y + (speed*Timer::deltaTime))), 0);
		}
		if (Input::GetKey(Key::S))
		{
			player.GetBody()->SetTransform(b2Vec2(player.GetPosition().x, (player.GetPosition().y - (speed*Timer::deltaTime))), 0);
		}

	}
	
	if (player.GetPosition().x <= -100.f && player.GetPosition().y >= 403.f)
	{
		MessageBox(NULL, "You completed the level!", "Finish", MB_OK);
		exit(0);
	}
	
}
	

void AnimationSpritePlayground::KeyboardDown()
{
}

void AnimationSpritePlayground::KeyboardUp()
{
}
