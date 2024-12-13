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
	win.create(800, 600, "Game");
	core.init(win);
	shader.init("VertexShader.txt", "PixelShader.txt", "TrexShader.txt", &core);
	


	Plane p;
	p.init(core.device);
	AnimationModel trex;
	trex.init("Source/TRex.gem",core.device);
	Model pine;
	pine.init("Source/Pine/pine.gem", core.device);
	

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity(); // No translation; cube is already centered
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);  // Adjust scale as needed
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(5.0f, 0.0f, 5.0f);
	DirectX::XMMATRIX pineWorld = scale * translate;
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1.0f, 0.1f, 100.0f);

	while (true) {
		float dt = timer.dt();
		core.clear();
		win.processMessages();
		camera.Update(dt, win.hwnd); 

		
		trex.instance.update("Run", dt);

		DirectX::XMMATRIX view = camera.GetViewMatrix();
		DirectX::XMMATRIX vp = view * projection;

		
		shader.updateConstantVS("staticMeshBuffer", "W", &world);
		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		core.devicecontext->IASetInputLayout(shader.staticLayout);
		core.devicecontext->VSSetShader(shader.staticVertexShader, NULL, 0); 
		core.devicecontext->PSSetShader(shader.pixelShader, NULL, 0); 
		shader.apply(&core);
		p.mesh.draw(core.devicecontext);
		
		shader.updateConstantVS("staticMeshBuffer", "W", &pineWorld);
		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		core.devicecontext->IASetInputLayout(shader.staticLayout);  // Use the same input layout
		core.devicecontext->VSSetShader(shader.staticVertexShader, NULL, 0);
		core.devicecontext->PSSetShader(shader.pixelShader, NULL, 0);
		shader.apply(&core);
		pine.draw(core.devicecontext);  // Draw the pine model


		shader.updateConstantVS("animatedMeshBuffer", "W", &world);
		shader.updateConstantVS("animatedMeshBuffer", "VP", &vp);
		shader.updateConstantVS("animatedMeshBuffer", "bones", trex.instance.matrices);

		
		core.devicecontext->IASetInputLayout(shader.animationLayout); 
		core.devicecontext->VSSetShader(shader.animationVertexShader, NULL, 0); 
		core.devicecontext->PSSetShader(shader.pixelShader, NULL, 0);
		shader.apply(&core);
		trex.draw(core.devicecontext);
		
		core.present();
	}
	return 0;

 }
 