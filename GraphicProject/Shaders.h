#pragma once
#include"DXCore.h"
#include<string>
#include<fstream>
#include <sstream>
#include<D3D11.h>
#include<D3Dcompiler.h>
#include<d3d11shader.h>
#include"ShaderReflection.h"

using namespace std;

class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* layout;

    vector<ConstantBuffer> psConstantBuffers;
    vector<ConstantBuffer> vsConstantBuffers;
    map<std::string, int> textureBindPointsVS;
    map<std::string, int> textureBindPointsPS;
    bool hasLayout = true;

	void init(string vs,string ps,DXcore *core ) {
        string vertexShaderHLSL = readFile(vs);
        string pixelShaderHLSL = readFile(ps);


        loadVS(core, vertexShaderHLSL);
        loadPS(core, pixelShaderHLSL);
        

        
    

	}


    void loadPS(DXcore* core, std::string hlsl)
    {
        ID3DBlob* shader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
        if (FAILED(hr)) {
            MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel shader Error", 0);
            exit(0);
        }
        core->device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &pixelShader);
        ConstantBufferReflection reflection;
        reflection.build(core, shader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
    }
    

    void loadVS(DXcore* core, std::string hlsl)
    {
        ID3DBlob* shader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &shader, &status);
        if (FAILED(hr))
        {
            MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex shader Error", 0);
            exit(0);
        }
        core->device->CreateVertexShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &vertexShader);

        ConstantBufferReflection reflection;
        reflection.build(core, shader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

        D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
        {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 }

        };
        core->device->CreateInputLayout(layoutDesc, 6, shader->GetBufferPointer(), shader->GetBufferSize(), &layout);

        
    }
    
    


    string readFile(string fileName) {
        ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open the file." << std::endl;
        }
        std::stringstream buffer;
        buffer << file.rdbuf(); 
        file.close(); 
        return buffer.str();
	}

    
   

    void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
    {
        for (int i = 0; i < buffers.size(); i++)
        {
            if (buffers[i].name == constantBufferName)
            {
                buffers[i].update(variableName, data);
                
                return;
            }
        }
    }
    void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
    {
        updateConstant(constantBufferName, variableName, data, vsConstantBuffers);


    }

    void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
    {
        updateConstant(constantBufferName, variableName, data, psConstantBuffers);
    }
   /* void updaetPSShader(DXcore *core,) {
        core->devicecontext->PSSetShaderResources(textureBindPoints[name], 1, &srv);

    }*/

    void apply(DXcore* core) {

        
        core->devicecontext->IASetInputLayout(layout);

        core->devicecontext->VSSetShader(vertexShader, NULL, 0);
        core->devicecontext->PSSetShader(pixelShader, NULL, 0);
        //core->devicecontext->PSSetShaderResources(0, 1, &srv);


        for (int i = 0; i < vsConstantBuffers.size(); i++) {
            vsConstantBuffers[i].upload(core);
            core->devicecontext->VSSetConstantBuffers(i, 1, &vsConstantBuffers[i].cb); 
            
        }

        for (int i = 0; i < psConstantBuffers.size(); i++) {
            core->devicecontext->PSSetConstantBuffers(i, 1, &psConstantBuffers[i].cb); 
        }
        

    }
};