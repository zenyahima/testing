#include "AnimationSpritePlayground.h"
#include "Utilities.h"

AnimationSpritePlayground::AnimationSpritePlayground(std::string name)
	:Scene(name)
{

	m_gravity = b2Vec2(0.f, -800.f);
	m_physicsWorld->SetGravity(m_gravity);

}

void AnimationSpritePlayground::InitScene(float windowWidth, float windowHeight)
{
	//dynamically allocates the register
	m_sceneReg = new entt::registry;

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

	//setup helloworld sign
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "flag.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-100.f, 307.f, 0.f));

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
				

	}

	//
	//ground
	Scene::BoxMaker(450, 35, 30, -20, 0, 1); 

	//boundaries
	Scene::BoxMaker(10, 2000, -125, -0, 0, 0);
	Scene::BoxMaker(10, 2000, 125, 0, 0, 0);

	//platforms
	Scene::BoxMaker(20, 3, -75, 25, 0, 1);
	Scene::BoxMaker(20, 3, -25, 40, 0, 1);
	Scene::BoxMaker(20, 3, -75, 55, 0, 1);
	Scene::BoxMaker(20, 3, -25, 70, 0, 1);
	Scene::BoxMaker(20, 3, 25, 85, 0, 1);
	Scene::BoxMaker(20, 3, 75, 85, 0, 1);
	Scene::BoxMaker(20, 3, 105, 105, 0, 1);
	Scene::BoxMaker(20, 3, 65, 125, 0, 1);
	Scene::BoxMaker(20, 3, 35, 145, 0, 1);
	Scene::BoxMaker(20, 3, -20, 145, 0, 1);
	Scene::BoxMaker(5, 3, -55, 160, 0, 1);
	Scene::BoxMaker(5, 3, -90, 175, 0, 1);
	Scene::BoxMaker(20, 3, -40, 190, 0, 1);
	Scene::BoxMaker(70, 3, 40, 205, 0, 1);
	Scene::BoxMaker(20, 3, 105, 220, 0, 1);
	Scene::BoxMaker(20, 3, 70, 240, 0, 1);
	Scene::BoxMaker(20, 3, 35, 255, 0, 1);
	Scene::BoxMaker(40, 3, -15, 275, 325, 1);
	Scene::BoxMaker(60, 3, -80, 300, 0, 1);
	
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
		vel += b2Vec2(-1.f, 0.f);
	}
	if (Input::GetKey(Key::D))
	{
		vel += b2Vec2(1.f, 0.f);
	}
	if (Input::GetKey(Key::Space))
	{
		vel += b2Vec2(0.f, 50.f);
	}
	

	player.GetBody()->SetLinearVelocity(speed * vel);
}

void AnimationSpritePlayground::KeyboardDown()
{
}

void AnimationSpritePlayground::KeyboardUp()
{
}
