#include "pch.h"

using namespace Microsoft::WRL;

IOutput::IOutput(ComPtr<IDXGIOutput>& pOutput)
    : m_pOutput(pOutput)
{
}

bool IOutput::QueryHardwareCompositionSupport(UINT* pFlags)
{
    UINT flags;
    ComPtr<IDXGIOutput6> output6;

    if (FAILED(m_pOutput.As(&output6))) {
        return false;
    }

    if (FAILED(output6->CheckHardwareCompositionSupport(&flags))) {
        return false;
    }

    *pFlags = flags;

    return true;
}

bool IOutput::GetDesc(DXGI_OUTPUT_DESC& out)
{
    return SUCCEEDED(m_pOutput->GetDesc(&out));
}


IAdapter::IAdapter(ComPtr<IDXGIAdapter>& pAdapter, UINT index)
    : m_pAdapter(pAdapter), m_index(index)
{
}

void IAdapter::GetOutputs(OutputList& out)
{
    for (UINT i = 0;; ++i)
    {
        ComPtr<IDXGIOutput> output;
        HRESULT res = m_pAdapter->EnumOutputs(i, &output);

        if (res == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        ThrowIfFailed(res);

        out.push_back(IOutput(output));
    }
}

bool IAdapter::GetDesc(DXGI_ADAPTER_DESC& out)
{
    return SUCCEEDED(m_pAdapter->GetDesc(&out));
}

UINT IAdapter::GetIndex() const
{
    return m_index;
}

IAdapters::IAdapters()
{
    ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory));
}

void IAdapters::Get(AdapterList& out)
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