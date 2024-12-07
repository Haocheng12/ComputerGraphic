//#pragma once
//
//#include"DXCore.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//class Texture {
//    ID3D11Texture2D* texture;
//    ID3D11ShaderResourceView* srv;
//    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//    
//
//
//    void initTexture() {
//        int width = 0;
//        int height = 0;
//        int channels = 0;
//        unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
//        if (channels == 3) {
//            channels = 4;
//            unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
//            for (int i = 0; i < (width * height); i++) {
//                texelsWithAlpha[i * 4] = texels[i * 3];
//                texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
//                texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
//                texelsWithAlpha[(i * 4) + 3] = 255;
//            }
//            // Initialize texture using width, height, channels, and texelsWithAlpha
//            delete[] texelsWithAlpha;
//        }
//        else {
//            // Initialize texture using width, height, channels, and texels
//        }
//        stbi_image_free(texels);
//
//        
//
//    }
//    void init(DXCore* core, int width, int height, int channels, DXGI_FORMAT format) {
//        D3D11_TEXTURE2D_DESC texDesc;
//        memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
//        texDesc.Width = width;
//        texDesc.Height = height;
//        texDesc.MipLevels = 1;
//        texDesc.ArraySize = 1;
//        texDesc.Format = format;
//        texDesc.SampleDesc.Count = 1;
//        texDesc.Usage = D3D11_USAGE_DEFAULT;
//        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//        texDesc.CPUAccessFlags = 0;
//
//     
//
//        D3D11_SUBRESOURCE_DATA initData;
//        memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
//        initData.pSysMem = data;
//        initData.SysMemPitch = width * channels;
//        core->device->CreateTexture2D(&texDesc, &initData, &texture);
//
//
//        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//        srvDesc.Format = format;
//        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//        srvDesc.Texture2D.MostDetailedMip = 0;
//        srvDesc.Texture2D.MipLevels = 1;
//        core->device->CreateShaderResourceView(texture, &srvDesc, &srv);
//
//    }
//    void free() {
//        srv->Release();
//        texture->Release();
//    }
//};
//
//