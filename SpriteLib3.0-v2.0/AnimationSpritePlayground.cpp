#include "AnimationSpritePlayground.h"
#include "Utilities.h"
//global variable
int numFootContacts;
class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {
		//std::cout << "this is running" << std::endl;
		//check if fixture A was the foot sensor
		//void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		//if ((int)fixtureUserData == 3)
		//	numFootContacts++;
		////check if fixture B was the foot sensor
		//fixtureUserData = contact->GetFixtureB()->GetUserData();
		//if ((int)fixtureUserData == 3)
		//	numFootContacts++;
		numFootContacts++;
		//ECS::GetComponent<Player>(MainEntities::MainPlayer()).SetLocked(false);
	}

	void EndContact(b2Contact* contact) {
		//std::cout << "this is running 2" << std::endl;
		//check if fixture A was the foot sensor
		//void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		//if ((int)fixtureUserData == 3)
		//	numFootContacts--;
		////check if fixture B was the foot sensor
		//fixtureUserData = contact->GetFixtureB()->GetUserData();
		//if ((int)fixtureUserData == 3)
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

	////setup helloworld sign
	//{
	//	auto entity = ECS::CreateEntity();

	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);

	//	std::string fileName = "HelloWorld.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));

	//}

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
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 25, &ECS::GetComponent<Sprite>(entity),
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

		//shape definition for main fixture
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(1, 2);

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;


		tempBody = m_physicsWorld->CreateBody(&tempDef);
		

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
		//add main fixture
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->CreateFixture(&myFixtureDef);
		

		//add foot sensor fixture
		polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0, -2), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);


	}

	////Setup GROUND (static box 1)
	//{
	//	//Creates entity 
	//	auto entity = ECS::CreateEntity();

	//	//Add components 
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	//ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up components 
	//	std::string fileName = "boxSprite.jpg";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 450, 50);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(30.f), float32(-20.f));

		////shape definition
		//b2PolygonShape polygonShape;
		//polygonShape.SetAsBox(1, 1);
		////fixture definition
		//b2FixtureDef myFixtureDef;
		//myFixtureDef.shape = &polygonShape;
		//myFixtureDef.density = 1;

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

		//add main fixture
		//myFixtureDef.isSensor = true;
		//b2Fixture* fixture = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->CreateFixture(&myFixtureDef);
		//fixture->SetUserData((void*)1);
		//polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0, -2), 0);
	//	tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);



	//Setup LEFT BOUNDARY (static box 2)
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "ground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 2000);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 0.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0;
		float shrinkY = 0();
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-125.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	//Setup RIGHT BOUNDARY (static box 3)
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "ground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 2000);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 0.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(125.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	//}
//
//	//Setup LEFT BOUNDARY (static box 2)
//	{
//		//Creates entity 
//		auto entity = ECS::CreateEntity();
//
//		//Add components 
//		ECS::AttachComponent<Sprite>(entity);
//		ECS::AttachComponent<Transform>(entity);
//		ECS::AttachComponent<PhysicsBody>(entity);
//
//		//Sets up components 
//		std::string fileName = "boxSprite.jpg";
//		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 2000);
//		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 0.f));
//
//		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
//		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
//
//		float shrinkX = 0.f;
//		float shrinkY = 0.f;
//		b2Body* tempBody;
//		b2BodyDef tempDef;
//		tempDef.type = b2_staticBody;
//		tempDef.position.Set(float32(-125.f), float32(0.f));
//
//		tempBody = m_physicsWorld->CreateBody(&tempDef);
//
//		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
//
//	}
//
//	//Setup RIGHT BOUNDARY (static box 3)
//	{
//		//Creates entity 
//		auto entity = ECS::CreateEntity();
//
//		//Add components 
//		ECS::AttachComponent<Sprite>(entity);
//		ECS::AttachComponent<Transform>(entity);
//		ECS::AttachComponent<PhysicsBody>(entity);
//
//		//Sets up components 
//		std::string fileName = "boxSprite.jpg";
//		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 2000);
//		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 0.f));
//
//		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
//		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
//
//		float shrinkX = 0.f;
//		float shrinkY = 0.f;
//		b2Body* tempBody;
//		b2BodyDef tempDef;
//		tempDef.type = b2_staticBody;
//		tempDef.position.Set(float32(125.f), float32(0.f));
//
//		tempBody = m_physicsWorld->CreateBody(&tempDef);
//
//		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
//
//	}
//
	//Setup PLATFORM 1 (static box 4)
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 65, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, 10.f, 0.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

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

	
		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
		}

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
	

void AnimationSpritePlayground::KeyboardDown()
{
}

void AnimationSpritePlayground::KeyboardUp()
{
}
