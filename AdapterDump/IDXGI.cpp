#include "pch.h"

using namespace Microsoft::WRL;

IDXGI::IDXGI()
{
    ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&m_pFactory)));

    if (CheckFactory<IDXGIFactory7>()) {
        m_version = DXGIVersion::V_7;
        m_caps = DXGICaps::FLIP_BOTH;
        return;
    }

    if (CheckFactory<IDXGIFactory6>()) {
        m_version = DXGIVersion::V_6;
        m_caps = DXGICaps::FLIP_BOTH;
        return;
    }

    if (CheckFactory<IDXGIFactory5>()) {
        m_version = DXGIVersion::V_5;
        m_caps = DXGICaps::FLIP_BOTH;
        return;
    }

    if (CheckFactory<IDXGIFactory4>()) {
        m_version = DXGIVersion::V_4;
        m_caps = DXGICaps::FLIP_BOTH;
        return;
    }

    if (CheckFactory<IDXGIFactory3>()) {
        m_version = DXGIVersion::V_3;
        m_caps = DXGICaps::FLIP_SEQUENTIAL;
        return;
    }

    m_caps = DXGICaps::NONE;

    if (CheckFactory<IDXGIFactory2>()) {
        m_version = DXGIVersion::V_2;
        return;
    }

    if (CheckFactory<IDXGIFactory1>()) {
        m_version = DXGIVersion::V_1;
        return;
    }

    m_version = DXGIVersion::V_0;
}

DXGIVersion IDXGI::GetVersion() const noexcept
{
    return m_version;
}

void IDXGI::GetAdapters(AdapterList& out) const
{
    for (UINT i = 0;; ++i)
    {
        ComPtr<IDXGIAdapter> pAdapter;
        HRESULT res = m_pFactory->EnumAdapters(i, &pAdapter);

        if (res == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        ThrowIfFailed(res);

        out.push_back(IAdapter(pAdapter, i));
    }
}

bool IDXGI::CheckFeatureSupport(DXGI_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) const noexcept
{
    ComPtr<IDXGIFactory5> tmp;
    if (FAILED(m_pFactory.As(&tmp))) {
        return false;
    }

    return SUCCEEDED(tmp->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize));
}

bool IDXGI::HasCapability(DXGICaps cap) const noexcept
{
    return (m_caps & cap) == cap;
}

DXGICaps IDXGI::GetCapabilities() const noexcept
{
    return m_caps;
}
