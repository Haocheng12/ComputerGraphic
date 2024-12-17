#pragma once

#include "DXCore.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>
#include <map>
#include <string>


class Texture {
public:
    ID3D11Texture2D* texture = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;
    ID3D11SamplerState* state = nullptr;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    std::map<std::string, int> textureBindPoints;

    void initTexture(const std::string& filename, ID3D11Device* device) {
        int width = 0, height = 0, channels = 0;
        unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!texels) {
            throw std::runtime_error("Failed to load texture: " + filename);
        }

        unsigned char* texelsWithAlpha = nullptr;
        if (channels == 3) {
            channels = 4;
            texelsWithAlpha = new unsigned char[width * height * channels];
            for (int i = 0; i < width * height; i++) {
                texelsWithAlpha[i * 4] = texels[i * 3];
                texelsWithAlpha[i * 4 + 1] = texels[i * 3 + 1];
                texelsWithAlpha[i * 4 + 2] = texels[i * 3 + 2];
                texelsWithAlpha[i * 4 + 3] = 255;
            }
        }

        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = format;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
       
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = (channels == 4 && texelsWithAlpha) ? texelsWithAlpha : texels;
        initData.SysMemPitch = width * channels;

        HRESULT hr = device->CreateTexture2D(&texDesc, &initData, &texture);
        if (FAILED(hr)) {
            stbi_image_free(texels);
            delete[] texelsWithAlpha;
            throw std::runtime_error("Failed to create texture.");
        }

        stbi_image_free(texels);
        delete[] texelsWithAlpha;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create Shader Resource View.");
        }

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        hr = device->CreateSamplerState(&samplerDesc, &state);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create Sampler State.");
        }
    }

    void free() {
        if (srv) srv->Release();
        if (texture) texture->Release();
        if (state) state->Release();
        srv = nullptr;
        texture = nullptr;
        state = nullptr;
    }
};

class Textures {
public:
    std::map<std::string, Texture> textures;

    void load(ID3D11Device* device, const std::string& filename) {
        if (textures.find(filename) == textures.end()) {
            Texture tex;
            tex.initTexture(filename, device);
            textures[filename] = tex;
        }
    }

    void bindTextureToPS(ID3D11DeviceContext* devicecontext, const std::string& filename, UINT slot) {
        if (textures.find(filename) != textures.end()) {
            devicecontext->PSSetShaderResources(slot, 1, &textures[filename].srv);
            devicecontext->PSSetSamplers(slot, 1, &textures[filename].state);
        }
    }

    void cleanup() {
        for (auto& pair : textures) {
            pair.second.free();  // Access the Texture object using pair.second
        }
        textures.clear();
    }
};

