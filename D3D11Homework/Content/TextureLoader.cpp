#include "pch.h"
#include"TextureLoder.h"  

#include "..\Common\DirectXHelper.h"

TextureLoder::TextureLoder()
{
	mTexture = NULL;
}

TextureLoder::TextureLoder(const TextureLoder&other)
{

}

TextureLoder::~TextureLoder()
{

}

bool TextureLoder::Initilize(ID3D11Device* d3dDevice, WCHAR* TextureFilename)
{
	HR(D3DX11CreateShaderResourceViewFromFileW(d3dDevice, TextureFilename, NULL, NULL, &mTexture, NULL));
	return true;
}

void TextureLoder::Shutdown()
{
	ReleaseCOM(mTexture);
}

ID3D11ShaderResourceView* TextureLoder::GetTexture()
{
	return mTexture;
}