#pragma once

#include "Rasterizer.h"
#include "../SubPic/SubPicProviderImpl.h"
#include "HdmvSub.h"
#include "BaseSub.h"

class HdmvSubtitleProviderImpl
{
public:
    HdmvSubtitleProviderImpl (CBaseSub* pSub);
    ~HdmvSubtitleProviderImpl(void);

    STDMETHODIMP Connect     (IXyOptions *consumer);
    STDMETHODIMP RequestFrame(IXySubRenderFrame**subRenderFrame, REFERENCE_TIME now);
    STDMETHODIMP Invalidate  (REFERENCE_TIME rtInvalidate = -1);
private:
    HRESULT ResetAllocator();
    HRESULT Render(REFERENCE_TIME now, POSITION pos);
private :
    CBaseSub*                   m_pSub;

    IXyOptions                 *m_consumer;
    CSize                       m_cur_output_size;
    bool m_use_dst_alpha;
    CComPtr<ISubPicExAllocator> m_allocator;
    CComPtr<ISubPicEx>          m_pSubPic;
    CComPtr<IXySubRenderFrame>  m_xy_sub_render_frame;
};

//This is for embedded subtitles
class __declspec(uuid("F316F98F-BDB5-42AA-A253-8D6C8AA20754"))
HdmvSubtitleProvider : public CUnknown, public IXySubRenderProvider, public ISubStream
{
public:
    HdmvSubtitleProvider(CCritSec* pLock, SUBTITLE_TYPE nType, const CString& name, LCID lcid);
    ~HdmvSubtitleProvider(void);

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

    // IXySubRenderProvider
    STDMETHODIMP Connect     (IXyOptions *consumer);
    STDMETHODIMP RequestFrame(IXySubRenderFrame**subRenderFrame, REFERENCE_TIME now);
    STDMETHODIMP Invalidate  (REFERENCE_TIME rtInvalidate = -1);

    // IPersist
    STDMETHODIMP GetClassID(CLSID* pClassID);

    // ISubStream
    STDMETHODIMP_(int) GetStreamCount();
    STDMETHODIMP GetStreamInfo(int i, WCHAR** ppName, LCID* pLCID);
    STDMETHODIMP_(int) GetStream();
    STDMETHODIMP SetStream(int iStream);
    STDMETHODIMP Reload();

    //fix me: add to an interface
    HRESULT ParseSample(IMediaSample* pSample);
    HRESULT NewSegment (REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
    HRESULT EndOfStream(void);

    HRESULT SetYuvType(CBaseSub::ColorType colorType, CBaseSub::YuvRangeType yuvRangeType);
private :
    CString         m_name;
    LCID            m_lcid;
    REFERENCE_TIME  m_rtStart;

    CBaseSub*       m_pSub;
    CCritSec        m_csCritSec;
    HdmvSubtitleProviderImpl *m_helper;
};

//This is for external subtitles
class __declspec(uuid("B5C880E5-A702-4F1B-B79D-410149874726"))
SupFileSubtitleProvider : public CUnknown, public IXySubRenderProvider, public ISubStream, protected CAMThread
{
public:
    SupFileSubtitleProvider();
    virtual ~SupFileSubtitleProvider();

    bool Open(CString fn, CString subName = _T(""));

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

    // IXySubRenderProvider
    STDMETHODIMP Connect     (IXyOptions *consumer);
    STDMETHODIMP RequestFrame(IXySubRenderFrame**subRenderFrame, REFERENCE_TIME now);
    STDMETHODIMP Invalidate  (REFERENCE_TIME rtInvalidate = -1);

    // IPersist
    STDMETHODIMP GetClassID(CLSID* pClassID);

    // ISubStream
    STDMETHODIMP_(int) GetStreamCount();
    STDMETHODIMP GetStreamInfo(int i, WCHAR** ppName, LCID* pLCID);
    STDMETHODIMP_(int) GetStream();
    STDMETHODIMP SetStream(int iStream);
    STDMETHODIMP Reload();

    HRESULT SetYuvType(CBaseSub::ColorType colorType, CBaseSub::YuvRangeType yuvRangeType);
protected:
    DWORD ThreadProc();
private:
    CString     m_fname, m_subname;
    CHdmvSub*   m_pSub;
    CCritSec    m_csCritSec;
    HdmvSubtitleProviderImpl *m_helper;
};

//This is for embedded subtitles -- pk
class __declspec(uuid("131980A7-FDAC-4543-95A8-C99EEDBD77DD"))
EmbeddedPGSSubtitleProvider : public CSubPicProviderImpl, public ISubStream
{
public:
    EmbeddedPGSSubtitleProvider();
    virtual ~EmbeddedPGSSubtitleProvider();

    bool LoadBytes(BYTE* buf, int bufSize);

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);
    
    // ISubPicProvider
    STDMETHODIMP_(POSITION) GetStartPosition(REFERENCE_TIME rt, double fps);
    STDMETHODIMP_(POSITION) GetNext(POSITION pos);
    STDMETHODIMP_(REFERENCE_TIME) GetStart(POSITION pos, double fps);
    STDMETHODIMP_(REFERENCE_TIME) GetStop(POSITION pos, double fps);
    STDMETHODIMP_(bool) IsAnimated(POSITION pos);
    STDMETHODIMP Render(SubPicDesc& spd, REFERENCE_TIME rt, double fps, RECT& bbox);
    STDMETHODIMP GetTextureSize(POSITION pos, SIZE& MaxTextureSize, SIZE& VirtualSize, POINT& VirtualTopLeft);

    // ISubPicProviderEx
    STDMETHODIMP_(bool) IsColorTypeSupported(int type);

    // IPersist
    STDMETHODIMP GetClassID(CLSID* pClassID);

    // ISubStream
    STDMETHODIMP_(int) GetStreamCount();
    STDMETHODIMP GetStreamInfo(int i, WCHAR** ppName, LCID* pLCID);
    STDMETHODIMP_(int) GetStream();
    STDMETHODIMP SetStream(int iStream);
    STDMETHODIMP Reload();

    HRESULT SetYuvType(CBaseSub::ColorType colorType, CBaseSub::YuvRangeType yuvRangeType);

private:
    CString     m_fname, m_subname;
    CHdmvSub*   m_pSub;
    CCritSec    m_csCritSec;
    REFERENCE_TIME	m_rtStart;

    HdmvSubtitleProviderImpl *m_helper;
};

