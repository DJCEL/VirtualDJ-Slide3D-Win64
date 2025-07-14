#include "Slide3D8.h"


//------------------------------------------------------------------------------------------
CSlide3D8::CSlide3D8()
{
	pD3DDevice = nullptr;
	ZeroMemory(m_DefaultVertices, 2 * 4 * sizeof(TVertex8));
	ZeroMemory(m_Vertices, 2 * 4 * sizeof(TVertex8));
	m_Direct3D_On = false;
	m_Width = 0;
	m_Height = 0;
}
//------------------------------------------------------------------------------------------
CSlide3D8::~CSlide3D8()
{

}
//---------------------------------------------------------------------------------------------
HRESULT VDJ_API CSlide3D8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->Author = "DJ CEL";
	infos->PluginName = "Slide3D";
	infos->Description = " A slide effect in 3D";
	infos->Flags = VDJFLAG_VIDEOTRANSITION_CONTINOUS;
	infos->Version     = "2.0 (64-bit)";
	
	return S_OK;
}
//---------------------------------------------------------------------------------------------
HRESULT VDJ_API CSlide3D8::OnLoad()
{
	
	return S_OK;
}
//---------------------------------------------------------------------------------------------
ULONG VDJ_API CSlide3D8::Release()
{
	delete this;
	return 0;
}
//---------------------------------------------------------------------------------------------
HRESULT VDJ_API CSlide3D8::OnDeviceInit()
{
	HRESULT hr = S_FALSE;

	m_Direct3D_On = true;
	m_Width = width;
	m_Height = height;

	hr = GetDevice(VDJVIDEOENGINE, (void**)&pD3DDevice);
	if (hr != S_OK || pD3DDevice == nullptr) return S_FALSE;


	return S_OK;
}
//---------------------------------------------------------------------------------------------
HRESULT VDJ_API CSlide3D8::OnDeviceClose()
{
	pD3DDevice = nullptr; //can no longer be used when device closed
	m_Direct3D_On = false;
	return S_OK;
}
//---------------------------------------------------------------------------------------------
HRESULT VDJ_API CSlide3D8::OnDraw(float crossfader)
{
	HRESULT hr = S_FALSE;

	if (width != m_Width || height != m_Height)
	{
		OnResizeVideo();
	}


	memcpy(m_Vertices[0],GetVertices(1),4*sizeof(TVertex8));
	memcpy(m_Vertices[1],GetVertices(2),4*sizeof(TVertex8));

	hr = Compose(crossfader);

	return hr;
}
//---------------------------------------------------------------------------------------------
void CSlide3D8::OnResizeVideo()
{
	m_Width = width;
	m_Height = height;
}
//---------------------------------------------------------------------------------------------
HRESULT CSlide3D8::Compose(float crossfader)
{
	HRESULT hr=S_FALSE;

	float compressor_rate = 0.0f;
	int alpha = 0;


	if (crossfader<=0.5f)
	{
		compressor_rate = crossfader/0.5f;
		alpha = (int) (compressor_rate * 255.0f);

		// Video 1
		m_Vertices[0][0].position.x = 0.0f * m_Width;
		m_Vertices[0][0].position.y = (0.0f + compressor_rate * 0.25f ) * m_Height; // de 0 à 0.25;

		m_Vertices[0][1].position.x = (1.0f - compressor_rate * 0.5f) * m_Width; // de 1 à 0.5
		m_Vertices[0][1].position.y = (0.0f + compressor_rate * 0.25f) * m_Height; // de 0 à 0.25

		m_Vertices[0][2].position.x = (1.0f - compressor_rate * 0.5f) * m_Width; // de 1 à 0.5
		m_Vertices[0][2].position.y = (1.0f - compressor_rate * 0.25f) * m_Height; // de 1 à 0.75;

		m_Vertices[0][3].position.x = 0.0f * m_Width ;
		m_Vertices[0][3].position.y = (1.0f - compressor_rate * 0.25f) * m_Height; // de 1 à 0.75;

		//Video 2
		m_Vertices[1][0].position.x = 0.5f * m_Width;
		m_Vertices[1][0].position.y = 0.25f * m_Height;
        m_Vertices[1][0].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[1][1].position.x = 1.0f * m_Width;
		m_Vertices[1][1].position.y = 0.25f * m_Height;
		m_Vertices[1][1].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[1][2].position.x = 1.0f * m_Width;
		m_Vertices[1][2].position.y = 0.75f * m_Height;
		m_Vertices[1][2].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[1][3].position.x = 0.5f * m_Width;
		m_Vertices[1][3].position.y = 0.75f * m_Height;
		m_Vertices[1][3].color = D3DCOLOR_RGBA(255,255,255,alpha);

		hr = RenderSurface(2,false);
		hr = RenderSurface(1,false);

	}
	else 
	{
		compressor_rate=(1.0f-crossfader)/0.5f;
		alpha = (int) (compressor_rate * 255.0f);

		//Video 1
		m_Vertices[0][0].position.x=0.0f * m_Width;
		m_Vertices[0][0].position.y=0.25f * m_Height;
		m_Vertices[0][0].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[0][1].position.x=0.5f * m_Width;
		m_Vertices[0][1].position.y=0.25f * m_Height;
		m_Vertices[0][1].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[0][2].position.x=0.5f * m_Width;
		m_Vertices[0][2].position.y=0.75f * m_Height;
		m_Vertices[0][2].color = D3DCOLOR_RGBA(255,255,255,alpha);

		m_Vertices[0][3].position.x=0.0f * m_Width;
		m_Vertices[0][3].position.y=0.75f * m_Height;
		m_Vertices[0][3].color = D3DCOLOR_RGBA(255,255,255,alpha);

		// Video 2
		m_Vertices[1][0].position.x=(0.0f + compressor_rate * 0.5f) * m_Width; // de 0.5 à 0.0
		m_Vertices[1][0].position.y=(0.0f + compressor_rate * 0.25f) * m_Height; // de 0.25 à 0.0

		m_Vertices[1][1].position.x=1.0f * m_Width;
		m_Vertices[1][1].position.y=(0.0f + compressor_rate * 0.25f) * m_Height; // de 0.25 à 0.0

		m_Vertices[1][2].position.x=1.f * m_Width;
		m_Vertices[1][2].position.y=(1.f - compressor_rate * 0.25f) * m_Height; // de 0.75 à 1.0

		m_Vertices[1][3].position.x=(0.f + compressor_rate * 0.5f) * m_Width; // de 0.5 à 0.0
		m_Vertices[1][3].position.y=(1.f - compressor_rate * 0.25f) * m_Height; // de 0.75 à 1.0

		hr = RenderSurface(1,false);
		hr = RenderSurface(2,false);
	}
	
	return S_OK;
}
//---------------------------------------------------------------------------------------------
HRESULT CSlide3D8::RenderSurface(int deck, bool bDefault)
{	
	HRESULT hr = S_OK;

	VideoScaling(deck);

	if (bDefault==true) 
	{
		hr = DrawDeck(deck, NULL); // (pass NULL to DrawDeck() to use the default vertices)
	}
	else 
	{
		hr = DrawDeck(deck, m_Vertices[deck-1]);
	}

	return hr;
}
//---------------------------------------------------------------------------------------------
void CSlide3D8::VideoScaling(int deck)
{
	float WidthOriginalVideo, HeightOriginalVideo;
	float WidthVideo, HeightVideo;
	float NewWidthVideo, NewHeightVideo;
	float RatioOriginalVideo;
	bool b_CropVideoW,b_CropVideoH;
	float dx,dy;

	memcpy(m_DefaultVertices[deck-1],GetVertices(deck),4*sizeof(TVertex8));

	WidthOriginalVideo = m_DefaultVertices[deck-1][1].position.x - m_DefaultVertices[deck-1][0].position.x;
	HeightOriginalVideo = m_DefaultVertices[deck-1][3].position.y - m_DefaultVertices[deck-1][0].position.y;

    b_CropVideoW = (WidthOriginalVideo !=  float(m_Width));
	b_CropVideoH  = (HeightOriginalVideo !=  float(m_Height));

	RatioOriginalVideo = HeightOriginalVideo / WidthOriginalVideo;

	WidthVideo = m_Vertices[deck-1][1].position.x - m_Vertices[deck-1][0].position.x;
	HeightVideo = m_Vertices[deck-1][3].position.y - m_Vertices[deck-1][0].position.y;

	if (b_CropVideoW)
	{
		NewWidthVideo = HeightVideo / RatioOriginalVideo;
		dx = (WidthVideo - NewWidthVideo) * 0.5f;

		m_Vertices[deck-1][0].position.x += dx;
		m_Vertices[deck-1][1].position.x -= dx;
		m_Vertices[deck-1][2].position.x -= dx;
		m_Vertices[deck-1][3].position.x += dx;
	}
	else if (b_CropVideoH)
	{
		NewHeightVideo = WidthVideo * RatioOriginalVideo;
		dy = (HeightVideo - NewHeightVideo) * 0.5f;
	
		m_Vertices[deck-1][0].position.y += dy;
		m_Vertices[deck-1][1].position.y += dy;
		m_Vertices[deck-1][2].position.y -= dy;
		m_Vertices[deck-1][3].position.y -= dy;
	}
}
