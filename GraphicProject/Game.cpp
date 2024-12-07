#include"Window.h"
#include"DXCore.h"
#include"Shaders.h"
#include "Camera.h"
#include <DirectXMath.h>
#include"GamesEngineeringBase.h"


int  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXcore core;
	Shader shader;
	GamesEngineeringBase::Timer timer;
	Camera camera(1.0f);
	win.create(800, 600, "My Window");
	core.init(win);
	shader.init("VertexShader.txt", "PixelShader.txt", &core);


	/*Plane p;
	p.init(core.device);*/
	/*Cube c;
	c.init(core.device);*/

	Model bunny;
	bunny.init("bunny.gem",core.device);

	Matrix planeWorld;

	Matrix p = Matrix::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	Matrix v = Matrix::lookAt(Vec3(0,1, -1), Vec3(0, 0, 0), Vec3(0, 1, 0));
	//Matrix vp =  view*projection;
	DirectX::XMMATRIX view = camera.GetViewMatrix();
	// Create World, View, Projection Matrices
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity(); // No translation; cube is already centered
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),    // 45-degree FOV
		1.0f, // Aspect ratio
		0.1f,                    // Near plane
		100.0f                   // Far plane
	);




	while (true) {
		float dt = timer.dt();
		core.clear();
		win.processMessages();
		camera.Update(dt, win.hwnd); // Pass time step and window handle

		// Get the updated view matrix
		DirectX::XMMATRIX view = camera.GetViewMatrix();
		DirectX::XMMATRIX vp = view * projection;
		shader.updateConstantVS("staticMeshBuffer", "W", &world);
		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);
		shader.apply(&core);
		// Apply shader and draw the plane
		
		bunny.draw(core.devicecontext);
		
		
		core.present();
	}
	return 0;

 }
 