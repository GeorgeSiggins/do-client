// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "do_download_property_internal.h"

#include <cassert>
#include <vector>
#include <string>

#include "do_errors.h"

using namespace microsoft::deliveryoptimization::details;

int32_t UTF8toWstr(const char* str, std::wstring& wstr)
{
    size_t cch = strlen(str);
    if (cch == 0)
    {
        wstr = std::wstring();
    }

    std::vector<wchar_t> dest(cch * 4);
    const uint32_t result = MultiByteToWideChar(CP_UTF8, 0, str, static_cast<int>(cch), dest.data(), static_cast<int>(dest.size()));
    if (result == 0)
    {
        return E_FAIL;
    }
    wstr = std::wstring(dest.data(), result);
    return S_OK;
}

int32_t CDownloadPropertyValueInternal::Init(const std::string& val) noexcept
{
    V_VT(&_var) = VT_BSTR;

    std::wstring wval;
    auto hr = UTF8toWstr(val.c_str(), wval);
    RETURN_IF_FAILED(hr);

    BSTR bstr = SysAllocString(wval.c_str());
    if (bstr == nullptr)
    {
        hr = DO_ERROR_FROM_STD_ERROR(std::errc::not_enough_memory);
    }
    V_BSTR(&_var) = bstr;

    return S_OK;
};

int32_t CDownloadPropertyValueInternal::Init(uint32_t val) noexcept
{
    V_VT(&_var) = VT_UI4;
    V_UI4(&_var) = val;
    return S_OK;
};

int32_t CDownloadPropertyValueInternal::Init(uint64_t val) noexcept
{
    V_VT(&_var) = VT_UI8;
    V_UI8(&_var) = val;
    return S_OK;
};

int32_t CDownloadPropertyValueInternal::Init(bool val) noexcept
{
    V_VT(&_var) = VT_BOOL;
    V_BOOL(&_var) = val ? VARIANT_TRUE : VARIANT_FALSE;
    return S_OK;
};

int32_t CDownloadPropertyValueInternal::Init(std::vector<unsigned char>& val) noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
};

int32_t CDownloadPropertyValueInternal::Init(const download_property_value::status_callback_t& val) noexcept
{
    V_VT(&_var) = VT_EMPTY;
    _callback = val;
    return S_OK;
}

CDownloadPropertyValueInternal::~CDownloadPropertyValueInternal()
{
#ifdef DEBUG
    assert(SUCCEEDED(VariantClear(&_var)));
#else
    (void)VariantClear(&_var);
#endif
};

CDownloadPropertyValueInternal::CDownloadPropertyValueInternal(const CDownloadPropertyValueInternal& rhs)
{
#ifdef DEBUG
    assert(SUCCEEDED(VariantCopy(&_var, &rhs._var)));
#else:
    (void)VariantCopy(&_var, &rhs._var);
#endif
    _callback = rhs._callback;
};

CDownloadPropertyValueInternal& CDownloadPropertyValueInternal::operator=(CDownloadPropertyValueInternal copy)
{
    swap(*this, copy);
    return *this;
};

CDownloadPropertyValueInternal::CDownloadPropertyValueInternal(CDownloadPropertyValueInternal&& rhs) noexcept
{
    _var = rhs._var;
    rhs._var = {};
    V_VT(&rhs._var) = VT_EMPTY;
    _callback = std::move(rhs._callback);
};

const CDownloadPropertyValueInternal::native_type& CDownloadPropertyValueInternal::native_value() const noexcept
{
    return _var;
};

int32_t CDownloadPropertyValueInternal::As(bool& val) const noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
};

int32_t CDownloadPropertyValueInternal::As(uint32_t& val) const noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
};

int32_t CDownloadPropertyValueInternal::As(uint64_t& val) const noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
};

int32_t CDownloadPropertyValueInternal::As(std::string& val) const noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
};

int32_t CDownloadPropertyValueInternal::As(std::vector<unsigned char>& val) const noexcept
{
    return static_cast<int32_t>(errc::e_not_impl);
}

int32_t CDownloadPropertyValueInternal::As(download_property_value::status_callback_t& val) const noexcept
{
    val = _callback;
    return S_OK;
};
