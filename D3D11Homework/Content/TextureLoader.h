#pragma once  
#ifndef _TEX_CLASS_H  
#define _TEX_CLASS_H  

#include<d3d11.h>  
#include"Macro.h"  
#include<D3DX11.h> //����������Դ��API  

namespace D3D11Homework
{
	class TextureLoder
	{
	private:
		ID3D11ShaderResourceView * mTexture;

	public:
		TextureLoder();
		TextureLoder(const TexClass&);
		~TextureLoder();

	public:
		bool Initilize(ID3D11Device* d3dDevice, WCHAR* TextureFilename);
		void Shutdown();
		ID3D11ShaderResourceView* GetTexture();
	};
}