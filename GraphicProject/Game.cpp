#include"Window.h"
#include"Shaders.h"
#include "Camera.h"
#include <DirectXMath.h>
#include"GamesEngineeringBase.h"
#include"Mesh.h"
#include"AnimationController.h"
int  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXcore core;
	Shader shader;
	GamesEngineeringBase::Timer timer;
	Camera camera(1.0f);
	win.create(800, 600, "Game");
	core.init(win);
	shader.init("VertexShader.txt", "GrassShader.txt","PineShader.txt", "TrexShader.txt","SkyPShader.txt","TrexPShader.txt" ,& core);
	
	std::vector<AABB> Colliders;
	Textures textures;

	Plane p;
	p.init(core.device);
	AnimationModel trex;
	trex.init("Source/TRex.gem",core.device,&textures);
	
	TextureModel tree1;
	tree1.init("Source/Pine/pine.gem", core.device, &textures);
	TextureModel tree2;
	tree2.init("Source/Pine/pine.gem", core.device, &textures);

	Skybox skybox;
	skybox.init(&core, "Sky.png", &textures);
	
	AnimationController animationController;

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX skyWorld =XMMatrixTranslation(0.0f, -50.0f, 0.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);  // Adjust scale as needed
	DirectX::XMMATRIX translate1 = DirectX::XMMatrixTranslation(5.0f, 0.0f, 5.0f);
	DirectX::XMMATRIX translate2 = DirectX::XMMatrixTranslation(2.0f, 0.0f, 5.0f);
	DirectX::XMMATRIX tree1World = scale * translate1;
	DirectX::XMMATRIX tree2World = scale * translate2;

	// Extract position by directly accessing the matrix elements
	
	AABB tree1Collider;
	tree1Collider.setFromCenterAndSize(XMFLOAT3(5.0f, 0.0f, 5.0f));  // Add the collider for tree1
	Colliders.push_back(tree1Collider);

	AABB tree2Collider;
	tree2Collider.setFromCenterAndSize(XMFLOAT3(2.0f, 0.0f, 5.0f));  // Add the collider for tree2
	Colliders.push_back(tree2Collider);

	AABB trexCollider;
	trexCollider.setFromCenterAndSize(XMFLOAT3(30.0f, 0.0f, 30.0f),5.0f);
	Colliders.push_back(trexCollider);

	DirectX::XMMATRIX TrexWorld = DirectX::XMMatrixTranslation(30.0f, 0.0f, 30.0f);
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 1.0f, 0.1f, 500.0f);

	while (true) {
		float dt = timer.dt();
		core.clear();
		win.processMessages();
		camera.Update(dt, win.hwnd,Colliders); 

		
		

		DirectX::XMMATRIX view = camera.GetViewMatrix();
		DirectX::XMMATRIX vp = view * projection;

		
		skybox.draw(&core, &shader, &textures, world, vp);


		shader.updateConstantVS("staticMeshBuffer", "W", &world);
		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		core.devicecontext->IASetInputLayout(shader.staticLayout);
		core.devicecontext->VSSetShader(shader.staticVertexShader, NULL, 0); 
		core.devicecontext->PSSetShader(shader.pixelShader, NULL, 0); 
		shader.apply(core.devicecontext);
		p.mesh.draw(core.devicecontext);
		

		
		
		tree1.draw(core.devicecontext,textures,&shader, tree1World,vp);  // Draw the pine model
		tree2.draw(core.devicecontext, textures, &shader, tree2World, vp);

		// Inside WinMain or wherever you update your game logic
		

		// Update the animation based on camera distance
		animationController.update(trex, camera, TrexWorld, dt, Colliders);
		trex.draw(core.devicecontext, textures, &shader, TrexWorld, vp);
		
		core.present();
	}
	return 0;

 }


