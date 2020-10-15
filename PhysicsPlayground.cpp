#include "PhysicsPlayground.h"
#include "Utilities.h"
#include <cmath> 
PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}
	
	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));
		tempDef.angle = Transform::ToRadians(45.f);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
	}

	//Setup BOX
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-30.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	}

	//Setup static BOX
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2000, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	Scene::AdjustScrollOffset();
}
static bool leap = false;
static int speedDelay = 0;
static float HorizVelMod = 0.0, gravity = -4.0, PlayerVertVel = 0.0,runMode=1.0;
void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float baseSpeed = 3.f;
	bool sprint = false;
	
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	if (runMode==1.0){
		if (Input::GetKey(Key::Shift))
		{
		sprint = true;
		}

		if (Input::GetKey(Key::A))
		{
			if ((sprint == true)&&(HorizVelMod >-3.0)) {
				HorizVelMod -= 0.01;
				if (HorizVelMod > -2.0) {
				HorizVelMod -= 0.02;
				}
		}
		else if ((sprint != true) && (HorizVelMod < -2.0)) { HorizVelMod += 0.01; }
			if (HorizVelMod > -2.0) {
			HorizVelMod -= 0.01;
			}
		}
		else if (Input::GetKey(Key::D) == false) {
			if (HorizVelMod < 0) {
			HorizVelMod += 0.01;
			}
		}



		if (Input::GetKey(Key::D))
		{
		if ((sprint == true) && (HorizVelMod <3.0)) {
			HorizVelMod += 0.01;
			if (HorizVelMod < 2.0) {
				HorizVelMod += 0.02;
			}
		}
		else if ((sprint!=true)&&(HorizVelMod >2.0)){ HorizVelMod -= 0.01; }
			if (HorizVelMod < 2.0) {
			HorizVelMod += 0.01;
			}
		}
		else if (Input::GetKey(Key::A) == false) {
			if (HorizVelMod > 0) {
			HorizVelMod -= 0.01;
			}
		}
	}
	if (runMode == -1.0) {
		if (Input::GetKey(Key::D))
		{
			if (HorizVelMod < 1.0) {
				HorizVelMod += (1.0 - HorizVelMod) / 8;
			}
			if ((HorizVelMod > 0.999999)&&(speedDelay!=25)) {
				speedDelay++;
				if (speedDelay == 25) {
					HorizVelMod += 0.00001;
				}
			}
			if ((HorizVelMod > 1.0)&&(HorizVelMod<2.0)) {
				HorizVelMod += (HorizVelMod-1) / 8;
			}			
		}
		else if (Input::GetKey(Key::A) == false) {
			if (HorizVelMod > 0) {
				HorizVelMod -= 0.01;
				speedDelay = 0;
			}
		}
		if (Input::GetKey(Key::A))
		{
			
			if (HorizVelMod >-1.0) {
				HorizVelMod += (-1.0 - HorizVelMod) / 8;
			}
			if ((HorizVelMod < -0.999999) && (speedDelay != 25)) {
				speedDelay++;
				if (speedDelay == 25) {
					HorizVelMod -= 0.00001;
				}
			}
			if ((HorizVelMod < -1.0) && (HorizVelMod > -2.0)) {
				HorizVelMod -= (-1.0-HorizVelMod ) / 8;
			}
		}
		else if (Input::GetKey(Key::D) == false) {
			if (HorizVelMod <0) {
				HorizVelMod += 0.01;
					speedDelay = 0;				
			}
		}
	}
	std::cout << speedDelay <<" ";
	std::cout << HorizVelMod << std::endl;
	//code is static so that it doesn't reset every frame, stopping the jump you 
	//adjusts vertical velocity while in the jump 
	//resets jump once touching ground 
	if (leap == true) {
		PlayerVertVel += gravity / 50;
	}
	if ((PlayerVertVel < -18) && (leap == true)) {
		leap = false;
		PlayerVertVel = 0.0;
	}
	//adjust velocity in both X and Y creating a velocity vector then apply vector to players velocity 
	vel += b2Vec2(HorizVelMod* baseSpeed,gravity+ PlayerVertVel);
	player.GetBody()->SetLinearVelocity(vel);
	
}
   // jumping mechanic 
void PhysicsPlayground::KeyboardDown()
{
	if (((Input::GetKey(Key::W)) && (leap == false)) || (Input::GetKey(Key::Space))&&(leap==false))
	{
		leap = true;
		PlayerVertVel = 25.f;
	}
	if (Input::GetKey(Key::M)) {
		runMode *= -1;
		HorizVelMod = 0.0;
	}
}

void PhysicsPlayground::KeyboardUp()
{
}
