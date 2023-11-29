#pragma once

#ifndef _CONSTANT_VALUES
#define _CONSTANT_VALUES

#include <chrono>
#include <thread>

/*-----------[General Profiler Constants]-----------*/
#define D3D11_METHOD_COUNT   205  
#define MEASURE_FRAME_COUNT  500
#define METHOD_ID_ARRAY_SIZE 500
#define METHOD_THREAD_COLLECTION_FRAME_COUNT 500

/*---------------[Chrono Typedefs]-----------------*/
typedef std::chrono::duration<double> RawDuration;
typedef std::chrono::nanoseconds Nanoseconds;
typedef std::chrono::steady_clock::time_point Timepoint;

/*--------------[ThreadID Typedefs]---------------*/
typedef std::size_t     idHash;
typedef std::thread::id threadID;

/*---------------[Data Structures]----------------*/
typedef std::array<Nanoseconds, D3D11_METHOD_COUNT>  delayArray;

/*--------------[Method specific]----------------*/
#define D3D11_METHOD_NAMES \
    X(QueryInterface)\
    X(AddRef)\
    X(Release)\
    X(SetPrivateData)\
    X(SetPrivateDataInterface)\
    X(GetPrivateData)\
    X(GetParent)\
    X(GetDevice)\
    X(Present)\
    X(GetBuffer)\
    X(SetFullscreenState)\
    X(GetFullscreenState)\
    X(GetDesc)\
    X(ResizeBuffers)\
    X(ResizeTarget)\
    X(GetContainingOutput)\
    X(GetFrameStatistics)\
    X(GetLastPresentCount)\
    X(QueryInterface)\
    X(AddRef)\
    X(Release)\
    X(CreateBuffer)\
    X(CreateTexture1D)\
    X(CreateTexture2D)\
    X(CreateTexture3D)\
    X(CreateShaderResourceView)\
    X(CreateUnorderedAccessView)\
    X(CreateRenderTargetView)\
    X(CreateDepthStencilView)\
    X(CreateInputLayout)\
    X(CreateVertexShader)\
    X(CreateGeometryShader)\
    X(CreateGeometryShaderWithStreamOutput)\
    X(CreatePixelShader)\
    X(CreateHullShader)\
    X(CreateDomainShader)\
    X(CreateComputeShader)\
    X(CreateClassLinkage)\
    X(CreateBlendState)\
    X(CreateDepthStencilState)\
    X(CreateRasterizerState)\
    X(CreateSamplerState)\
    X(CreateQuery)\
    X(CreatePredicate)\
    X(CreateCounter)\
    X(CreateDeferredContext)\
    X(OpenSharedResource)\
    X(CheckFormatSupport)\
    X(CheckMultisampleQualityLevels)\
    X(CheckCounterInfo)\
    X(CheckCounter)\
    X(CheckFeatureSupport)\
    X(GetPrivateData)\
    X(SetPrivateData)\
    X(SetPrivateDataInterface)\
    X(GetFeatureLevel)\
    X(GetCreationFlags)\
    X(GetDeviceRemovedReason)\
    X(GetImmediateContext)\
    X(SetExceptionMode)\
    X(GetExceptionMode)\
    X(QueryInterface)\
    X(AddRef)\
    X(Release)\
    X(GetDevice)\
    X(GetPrivateData)\
    X(SetPrivateData)\
    X(SetPrivateDataInterface)\
    X(SetConstantBuffers)\
    X(SetShaderResources)\
    X(SetShader)\
    X(SetSamplers)\
    X(SetShader)\
    X(DrawIndexed)\
    X(Draw)\
    X(Map)\
    X(Unmap)\
    X(SetConstantBuffers)\
    X(IASetInputLayout)\
    X(IASetVertexBuffers)\
    X(IASetIndexBuffer)\
    X(DrawIndexedInstanced)\
    X(DrawInstanced)\
    X(SetConstantBuffers)\
    X(SetShader)\
    X(IASetPrimitiveTopology)\
    X(SetShaderResources)\
    X(SetSamplers)\
    X(Begin)\
    X(End)\
    X(GetData)\
    X(SetPredication)\
    X(SetShaderResources)\
    X(SetSamplers)\
    X(OMSetRenderTargets)\
    X(OMSetRenderTargetsAndUnorderedAccessViews)\
    X(OMSetBlendState)\
    X(OMSetDepthStencilState)\
    X(SOSetTargets)\
    X(DrawAuto)\
    X(DrawIndexedInstancedIndirect)\
    X(DrawInstancedIndirect)\
    X(Dispatch)\
    X(DispatchIndirect)\
    X(RSSetState)\
    X(RSSetViewports)\
    X(RSSetScissorRects)\
    X(CopySubresourceRegion)\
    X(CopyResource)\
    X(UpdateSubresource)\
    X(CopyStructureCount)\
    X(ClearRenderTargetView)\
    X(ClearUnorderedAccessViewUint)\
    X(ClearUnorderedAccessViewFloat)\
    X(ClearDepthStencilView)\
    X(GenerateMips)\
    X(SetResourceMinLOD)\
    X(GetResourceMinLOD)\
    X(ResolveSubresource)\
    X(ExecuteCommandList)\
    X(SetShaderResources)\
    X(SetShader)\
    X(SetSamplers)\
    X(SetConstantBuffers)\
    X(SetShaderResources)\
    X(SetShader)\
    X(SetSamplers)\
    X(SetConstantBuffers)\
    X(SetShaderResources)\
    X(CSSetUnorderedAccessViews)\
    X(SetShader)\
    X(SetSamplers)\
    X(SetConstantBuffers)\
    X(VSGetConstantBuffers)\
    X(PSGetShaderResources)\
    X(PSGetShader)\
    X(PSGetSamplers)\
    X(VSGetShader)\
    X(PSGetConstantBuffers)\
    X(IAGetInputLayout)\
    X(IAGetVertexBuffers)\
    X(IAGetIndexBuffer)\
    X(GSGetConstantBuffers)\
    X(GSGetShader)\
    X(IAGetPrimitiveTopology)\
    X(VSGetShaderResources)\
    X(VSGetSamplers)\
    X(GetPredication)\
    X(GSGetShaderResources)\
    X(GSGetSamplers)\
    X(OMGetRenderTargets)\
    X(OMGetRenderTargetsAndUnorderedAccessViews)\
    X(OMGetBlendState)\
    X(OMGetDepthStencilState)\
    X(SOGetTargets)\
    X(RSGetState)\
    X(RSGetViewports)\
    X(RSGetScissorRects)\
    X(HSGetShaderResources)\
    X(HSGetShader)\
    X(HSGetSamplers)\
    X(HSGetConstantBuffers)\
    X(DSGetShaderResources)\
    X(DSGetShader)\
    X(DSGetSamplers)\
    X(DSGetConstantBuffers)\
    X(CSGetShaderResources)\
    X(CSGetUnorderedAccessViews)\
    X(CSGetShader)\
    X(CSGetSamplers)\
    X(CSGetConstantBuffers)\
    X(ClearState)\
    X(Flush)\
    X(GetType)\
    X(GetContextFlags)\
    X(FinishCommandList)\
    X(CopySubresourceRegion1)\
    X(UpdateSubresource1)\
    X(DiscardResource)\
    X(DiscardView)\
    X(SetConstantBuffers1)\
    X(SetConstantBuffers1)\
    X(SetConstantBuffers1)\
    X(SetConstantBuffers1)\
    X(SetConstantBuffers1)\
    X(SetConstantBuffers1)\
    X(VSGetConstantBuffers1)\
    X(HSGetConstantBuffers1)\
    X(DSGetConstantBuffers1)\
    X(GSGetConstantBuffers1)\
    X(PSGetConstantBuffers1)\
    X(CSGetConstantBuffers1)\
    X(SwapDeviceContextState)\
    X(ClearView)\
    X(DiscardView1)\
    X(UpdateTileMappings)\
    X(CopyTileMappings)\
    X(CopyTiles)\
    X(UpdateTiles)\
    X(ResizeTilePool)\
    X(TiledResourceBarrier)\
    X(IsAnnotationEnabled)\
    X(SetMarkerInt)\
    X(BeginEventInt)\
    X(EndEvent)

const static std::array<std::string, D3D11_METHOD_COUNT> methodNames = {
#define X(_MethodName)\
	#_MethodName,
    D3D11_METHOD_NAMES
#undef X
};

#endif // !_CONSTANT_VALUES
