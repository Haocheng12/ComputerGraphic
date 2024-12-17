#pragma once


#include"GEMLoader.h"
#include"Animation.h"
#include"mathlib.h"
#include"Texture.h"
#include"Shaders.h"
#include"Collison.h"
struct Vertex {
	Vec3 position;
	Colour colour;
};


class Triangle {
	Vertex vertices[3];
	ID3D11Buffer* vertexBuffer;
public:
	void init(ID3D11Device* &device) {
		Vertex vertices[3];
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		device->CreateBuffer(&bd, &uploadData, &vertexBuffer);

	}
	
	void draw(ID3D11DeviceContext* &devicecontext) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		devicecontext->Draw(3, 0);

	}
};

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

class Mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;


	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, ID3D11Device* device) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}

	void init(vector<STATIC_VERTEX> vertices,vector<unsigned int> indices, ID3D11Device* device)
	{
		init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(),device);
	}
	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, ID3D11Device* device)
	{
		init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(),device);
	}


	void draw(ID3D11DeviceContext* devicecontext) {
		UINT offsets = 0;
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devicecontext->DrawIndexed(indicesSize, 0, 0);


	}

};

class Plane {
	
public:
	Mesh mesh;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame;
		//frame.fromVector(n);
		//v.tangent = frame.u; // For now
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(ID3D11Device* device) {
		
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-100, 0, -100), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(100, 0, -100), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-100, 0, 100), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(100, 0, 100), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(vertices, indices,device);


	}
	
};

class Cube {
public:
	Mesh mesh;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame;
		//frame.fromVector(n);
		//v.tangent = frame.u; // For now
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(ID3D11Device* device) {

		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);

		mesh.init(vertices, indices, device);


	}
};

class Model {
public:

	std::vector<Mesh> meshes;

	void init(string filename, ID3D11Device* device) {

		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			mesh.init(vertices, gemmeshes[i].indices,device);
			meshes.push_back(mesh);
		}


	}
	void draw(ID3D11DeviceContext* devicecontext) {
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(devicecontext);
		}
			
	}
};

class AnimationModel {

public:
	std::vector<Mesh> meshes;
	std::vector<std::string> textureNames;
	std::vector<std::string> normalNames;
	std::vector<std::string> roughnessNames;
	AnimationInstance instance;
	Animation animation;


	void init(string filename, ID3D11Device* device,Textures* textures) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
			}

			string diffuse = gemmeshes[i].material.find("diffuse").getValue();
			textureNames.push_back(diffuse);
			textures->load(device, diffuse);

			string normal = gemmeshes[i].material.find("normals").getValue();
			normalNames.push_back(normal);
			textures->load(device, normal);

			/*string roughness = gemmeshes[i].material.find("roughness").getValue();
			roughnessNames.push_back(roughness);
			textures->load(device, roughness);*/


			mesh.init(vertices, gemmeshes[i].indices, device);
			meshes.push_back(mesh);
		}


		// Set up the skeleton bones
		for (const auto& gemBone : gemanimation.bones) {
			Bone bone;
			bone.name = gemBone.name;
			bone.offset = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&gemBone.offset));
			//ThresholdMatrixValues(bone.offset);
			bone.parentIndex = gemBone.parentIndex;
			animation.skeleton.bones.push_back(bone);  // Safe because animation is now initialized
		}

		// Set up the animations
		for (const auto& gemAnim : gemanimation.animations) {
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemAnim.ticksPerSecond;
			for (const auto& gemFrame : gemAnim.frames) {
				AnimationFrame frame;

				for (size_t index = 0; index < gemFrame.positions.size(); ++index) {
					frame.positions.push_back(ConvertToXMFLOAT3(gemFrame.positions[index]));
					frame.rotations.push_back(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&gemFrame.rotations[index])));
					frame.scales.push_back(ConvertToXMFLOAT3(gemFrame.scales[index]));
				}

				aseq.frames.push_back(frame);
			}

			animation.animations.insert({ gemAnim.name, aseq });
		}
		animation.skeleton.globalInverse = ConvertToXMMATRIX(gemanimation.globalInverse);
		instance.animation = &animation;
		// Set global inverse matrix
		//instance.animation->skeleton.globalInverse = ConvertToXMMATRIX(gemanimation.globalInverse);
	}

	// Convert methods
	XMFLOAT3 ConvertToXMFLOAT3(const GEMLoader::GEMVec3& vec) {
		return XMFLOAT3(vec.x, vec.y, vec.z);
	}

	XMMATRIX ConvertToXMMATRIX(const GEMLoader::GEMMatrix& matrix) {
		return XMMATRIX(
			matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
			matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
			matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
			matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]
		);
	}
	void ThresholdMatrixValues(XMMATRIX& matrix, float threshold = 1e-6f) {
		// Iterate over each element in the matrix (4x4)
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				float value = matrix.r[row].m128_f32[col];  // Get matrix element

				// If the absolute value is below the threshold, set it to 0
				if (fabs(value) < threshold) {
					matrix.r[row].m128_f32[col] = matrix.r[row].m128_f32[col]*1000000;
				}
			}
		}
	}
	void draw(ID3D11DeviceContext* devicecontext, Textures textures, Shader* shader, DirectX::XMMATRIX world, DirectX::XMMATRIX vp) {
		// Extract position from the world matrix (translation part)
		
		shader->updateConstantVS("animatedMeshBuffer", "W", &world);
		shader->updateConstantVS("animatedMeshBuffer", "VP", &vp);
		shader->updateConstantVS("animatedMeshBuffer", "bones", instance.matrices);

		devicecontext->IASetInputLayout(shader->animationLayout);
		devicecontext->VSSetShader(shader->animationVertexShader, NULL, 0);
		devicecontext->PSSetShader(shader->trexPixelShader, NULL, 0);
		shader->apply(devicecontext, textures);

		for (int i = 0; i < meshes.size(); i++) {
			textures.bindTextureToPS(devicecontext, textureNames[i], 0);
			textures.bindTextureToPS(devicecontext, normalNames[i], 1);
			

			meshes[i].draw(devicecontext);
		}
	}
	
};

class TextureModel {
public:
	std::vector<Mesh> meshes;
	std::vector<std::string> textureNames;
	std::vector<std::string> normalNames;
	std::vector<std::string> roughnessNames;



	void init(string filename, ID3D11Device* device, Textures* textures) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}

			// Load textures as usual
			string diffuse = gemmeshes[i].material.find("diffuse").getValue();
			textureNames.push_back(diffuse);
			textures->load(device, diffuse);

			string normal = gemmeshes[i].material.find("normals").getValue();
			normalNames.push_back(normal);
			textures->load(device, normal);

			string roughness = gemmeshes[i].material.find("roughness").getValue();
			roughnessNames.push_back(roughness);
			textures->load(device, roughness);

			mesh.init(vertices, gemmeshes[i].indices, device);
			meshes.push_back(mesh);
		}
	}

	void draw(ID3D11DeviceContext* devicecontext, Textures textures, Shader* shader, DirectX::XMMATRIX world, DirectX::XMMATRIX vp) {
		// Extract position from the world matrix (translation part)
		

		shader->updateConstantVS("staticMeshBuffer", "W", &world);
		shader->updateConstantVS("staticMeshBuffer", "VP", &vp);

		devicecontext->IASetInputLayout(shader->staticLayout);
		devicecontext->VSSetShader(shader->staticVertexShader, NULL, 0);
		devicecontext->PSSetShader(shader->texturePixelShader, NULL, 0);
		shader->apply(devicecontext, textures);

		for (int i = 0; i < meshes.size(); i++) {
			textures.bindTextureToPS(devicecontext, textureNames[i], 0);
			textures.bindTextureToPS(devicecontext, normalNames[i], 1);
			textures.bindTextureToPS(devicecontext, roughnessNames[i], 2);

			meshes[i].draw(devicecontext);
		}
	}


};


class Skybox {
public:
	std::string textureName;
	Mesh skyboxMesh;

	void init(DXcore* core, const std::string& textureFile, Textures* textures) {
		// Load the cube map texture using Textures manager
		

		
		

		textures->load(core->device, textureFile);  // Assuming Textures now supports cube maps
		textureName = textureFile;


		// Initialize the skybox mesh (cube)
		cubeMeshInit(core->device);
	}

	void draw(DXcore* core, Shader* shader, Textures* textures, XMMATRIX world, XMMATRIX vp) {
		shader->updateConstantVS("staticMeshBuffer", "W", &world);
		shader->updateConstantVS("staticMeshBuffer", "VP", &vp);

		core->devicecontext->IASetInputLayout(shader->staticLayout);
		core->devicecontext->VSSetShader(shader->staticVertexShader, NULL, 0);
		core->devicecontext->PSSetShader(shader->skyPixelShader, NULL, 0);

		// Bind the cube map texture
		textures->bindTextureToPS(core->devicecontext, textureName, 0);

		// Apply shader and draw skybox mesh
		shader->apply(core->devicecontext, *textures);
		skyboxMesh.draw(core->devicecontext);
	}



	void cubeMeshInit(ID3D11Device* device) {
		float scale = 200.0f;
		std::vector<STATIC_VERTEX> vertices;

		// Define the 8 corner points of the skybox cube
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f) * scale;
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f) * scale;
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f) * scale;
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f) * scale;
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f) * scale;
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f) * scale;
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f) * scale;
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f) * scale;

		// Front (+Z)
		vertices.push_back(addVertex(p0, Vec3(0, 0, 1), 0.25f, 0.6666f));
		vertices.push_back(addVertex(p1, Vec3(0, 0, 1), 0.5f, 0.6666f));
		vertices.push_back(addVertex(p2, Vec3(0, 0, 1), 0.5f, 0.3333f));
		vertices.push_back(addVertex(p3, Vec3(0, 0, 1), 0.25f, 0.3333f));

		// Back (-Z)
		vertices.push_back(addVertex(p5, Vec3(0, 0, -1), 0.75f, 0.6665f));
		vertices.push_back(addVertex(p4, Vec3(0, 0, -1), 1.0f, 0.6665f));
		vertices.push_back(addVertex(p7, Vec3(0, 0, -1), 1.0f, 0.3334f));
		vertices.push_back(addVertex(p6, Vec3(0, 0, -1), 0.75f, 0.3334f));

		// Left (-X)
		vertices.push_back(addVertex(p4, Vec3(1, 0, 0), 0.0f, 0.6665f));
		vertices.push_back(addVertex(p0, Vec3(1, 0, 0), 0.25f, 0.6665f));
		vertices.push_back(addVertex(p3, Vec3(1, 0, 0), 0.25f, 0.3334f));
		vertices.push_back(addVertex(p7, Vec3(1, 0, 0), 0.0f, 0.3334f));

		// Right (+X)
		vertices.push_back(addVertex(p1, Vec3(-1, 0, 0), 0.5f, 0.6665f));
		vertices.push_back(addVertex(p5, Vec3(-1, 0, 0), 0.75f, 0.6665f));
		vertices.push_back(addVertex(p6, Vec3(-1, 0, 0), 0.75f, 0.3334f));
		vertices.push_back(addVertex(p2, Vec3(-1, 0, 0), 0.5f, 0.3334f));

		// Top (+Y) [Rotated 360 degrees from original (180 degrees from mirrored)]
		vertices.push_back(addVertex(p3, Vec3(0, -1, 0), 0.2499f, 0.3334f)); // (0.5f, 0.0f)
		vertices.push_back(addVertex(p2, Vec3(0, -1, 0), 0.4999f, 0.3334f));  // (0.25f, 0.0f)
		vertices.push_back(addVertex(p6, Vec3(0, -1, 0), 0.4999f, 0.0001f));    // (0.25f, 0.333f)
		vertices.push_back(addVertex(p7, Vec3(0, -1, 0), 0.2499f, 0.0001f));   // (0.5f, 0.333f)






		// Bottom (-Y) [Reverse TV for bottom face]
		vertices.push_back(addVertex(p4, Vec3(0, 1, 0), 0.25f, 1.0f));  // tv reversed
		vertices.push_back(addVertex(p5, Vec3(0, 1, 0), 0.5f, 1.0f));   // tv reversed
		vertices.push_back(addVertex(p1, Vec3(0, 1, 0), 0.5f, 0.6675f)); // tv reversed
		vertices.push_back(addVertex(p0, Vec3(0, 1, 0), 0.25f, 0.6675f)); // tv reversed

		// Create indices
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0); // Reversed
		indices.push_back(3); indices.push_back(2); indices.push_back(0); // Reversed

		indices.push_back(6); indices.push_back(5); indices.push_back(4); // Reversed
		indices.push_back(7); indices.push_back(6); indices.push_back(4); // Reversed

		indices.push_back(10); indices.push_back(9); indices.push_back(8); // Reversed
		indices.push_back(11); indices.push_back(10); indices.push_back(8); // Reversed

		indices.push_back(14); indices.push_back(13); indices.push_back(12); // Reversed
		indices.push_back(15); indices.push_back(14); indices.push_back(12); // Reversed

		indices.push_back(18); indices.push_back(17); indices.push_back(16); // Reversed
		indices.push_back(19); indices.push_back(18); indices.push_back(16); // Reversed

		indices.push_back(22); indices.push_back(21); indices.push_back(20); // Reversed
		indices.push_back(23); indices.push_back(22); indices.push_back(20); // Reversed


		// Initialize mesh
		skyboxMesh.init(vertices, indices, device);
	}


	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);  // Skybox doesn't need tangent vectors
		v.tu = tu;
		v.tv = tv;
		return v;
	}


};