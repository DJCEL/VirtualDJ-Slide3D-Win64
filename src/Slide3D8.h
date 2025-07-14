#ifndef SLIDE3D8_H
#define SLIDE3D8_H


#include "VdjVideo8.h"
#include <d3d11.h>
#include <atlbase.h> //we use atl for the CComPtr smart pointer, but this is optional

#pragma comment (lib, "D3D11.lib")


//-------------------------------------------------------------------------------------------
class CSlide3D8 : public IVdjPluginVideoTransition8
{
	public:
		CSlide3D8();
		~CSlide3D8();
		HRESULT VDJ_API OnLoad();
		HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *info);
		ULONG   VDJ_API Release();
		HRESULT VDJ_API OnDeviceInit();
		HRESULT VDJ_API OnDeviceClose();
		HRESULT VDJ_API OnDraw(float crossfader);

	private:
		void OnResizeVideo();
		HRESULT Initialize_D3D11(ID3D11Device* pDevice);
		HRESULT Rendering_D3D11(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RenderTargetView* pRenderTargetView, float crossfader);
		HRESULT RenderSurface(int deck, bool bDefault);
		void VideoScaling(int deck);

		ID3D11Device* pD3DDevice;
		ID3D11DeviceContext* pD3DDeviceContext;
		ID3D11RenderTargetView* pD3DRenderTargetView;
		TVertex8 m_DefaultVertices[2][4];
		TVertex8 m_Vertices[2][4];
		bool m_Direct3D_On;
		int m_Width;
		int m_Height;
	
	
		#ifndef SAFE_RELEASE
		#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
		#endif

		#ifndef SAFE_RELEASE_CCOMPTR
		#define SAFE_RELEASE_CCOMPTR(x) { if (x!=nullptr) { x.Release(); x=nullptr; } }
		#endif

		typedef DWORD D3DCOLOR;
		#define D3DCOLOR_RGBA(r,g,b,a) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

};
#endif
