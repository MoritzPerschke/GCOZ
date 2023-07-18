#pragma once
#incldue "HelperMacros.h"

DWORD methodDelays[205];


// commas between everythin to get "EVERY_..." macro working
#define D3D11_METHODS \
	X(3, HRESULT, SetPrivateData, REFGUID, guid, UINT, const void, *pData) \
	X(5, HRESULT, GetPrivateData, REFGUID, guid, UINT, *pDataSize, void, *pData) \
	X(8, HRESULT, Present, IDXGISwapChain*, pSwapChain, UINT, SyncInterval, UINT, Flags) \
	X(10, HRESULT, SetFullscreenState, BOOL, Fullscreen, IDXGI, *pTarget) \
	X(13, HRESULT, ResizeBuffers, UINT, BufferCount, UINT, Width, UINT, Height, UINT, Format, DXGI_FORMAT, NewFormat, UINT, SwapChainFlags) \
	X(14, HRESULT, ResizeTarget, const DXGI_MODE_DESC, *pNewTargetParameters) \
	X(16, HRESULT, GetFrameStatistics, DXGI_FRAME_STATISTICS, *pStats) \
	X(37, HRESULT, CreateClassLinkage, out ID3D11ClassLinkage, **ppLinkage) \
	X(47, HRESULT, CheckFormatSupport, DXGI_FORMAT, Format, out UINT, *pFormatSupport) \
	X(48, HRESULT, CheckMultisampleQualityLevels, DXGI_FORMAT Format, UINT, SampleCount, out UINT, *pNumQualityLevels) \
	X(49, void, CheckCounterInfo, out D3D11_COUNTER_INFO, *pCounterinfo) \
	X(51, HRESULT, CheckFeatureSupport, D3D11_FEATURE, Feature, out void, *pFeatureSupportData, UINT, FeatureSupportDataSize) \
	X(55, D3D_FEATURE_LEVEL, GetFeatureLevel) \
	X(56, UINT, GetCreationFlags) \
	X(57, HRESULT, GetDeviceRemovedReason) \
	X(58, void, GetImmediateContext, out ID3D11DeviceContext, **ppImmediateContext) \
	X(59, HRESULT, SetExceptionMode, UINT, RaiseFlags) \
	X(60, UINT, GetExceptionMode) \
	X(64, void, GetDevice, out ID3D11Device, **ppDevice) \
	X(73, void, DrawIndexed, UINT IndexCount, UINT, StartIndexLocation, INT, BaseVertexLocation) \
	X(74, void, Draw, UINT, VertexCount, UINT, StartVertexLocation) \
	X(76, void, Unmap, ID3D11Resource, *pResource, UINT, Subresource) \
	X(81, void, DrawIndexedInstanced, UINT, IndexCountPerInstance, UINT, InstanceCount, UINT, StartIndexLocation, INT, BaseVertexLocation, UINT, StartInstanceLocation) \
	X(82, void, DrawInstanced, UINT, VertexCountPerInstance, UINT, InstanceCount, UINT, StartVertexLocation, UINT, StartInstanceLocation) \
	X(85, void, IASetPrimitiveTopology, D3D11_PRIMITIVE_TOPOLOGY, Topology) \
	X(88, void, Begin, ID3D11Asynchronous, *pAsync) \
	X(89, void, End, ID3D11Asynchronous, *pAsync) \
	X(99, void, DrawAuto) \
	X(100, void DrawIndexedInstancedIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedOffsetForArgs) \
	X(101, void, DrawInstancedIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(102, void, Dispatch, UINT, ThreadGroupCountX, UINT, ThreadGroupCountY, UINT, ThreadGroupCountZ) \
	X(103, void, DispatchIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(108, void, CopyResource, ID3D11Resource, *pDstResource, ID3D11Resource, *pSrcResource) \
	X(110, void, CopyStructureCount, ID3D11Buffer, *pDstBuffer, UINT, DstAlignedByteOffset, ID3D11UnorderedAccessView, *pSrcView) \
	X(111, void, ClearRenderTargetView, ID3D11RenderTargetView, *pRenderTargetView, const FLOAT[4], ColorRGBA) \
	X(112, void, ClearUnorderedAccessViewUnit, ID3D11UnorderedAccessView, *pUnorderedAccessView, const UINT[4], Values) \
	X(113, void, ClearUnorderedAccessViewFloat, ID3D11UnorderedAccessView, *pUnorderedAccessView, const FLOAT[4], Values) \
	X(114, void, ClearDepthStencilView, ID3D11DepthStencilView, *pDepthStencilView, UINT, ClearFlags, FLOAT, Depth, UINT8, Stencil) \
	X(115, void, GenerateMips, ID3D11ShaderResourceView, *pShaderResourceView) \
	X(116, void, SetResourceMinLOD, ID3D11Resource, *pResource, FLOAT, MinLOD) \
	X(117, FLOAT, GetResourceMinLOD, ID3D11Resource, *pResource) \
	X(118, void, ResolveSubresource, ID3D11Resource, *pDstResource, UINT, DstSubResource, ID3D11Resource, *pScrResource, UINT, SrcSubResource, DXGI_FORMAT, Format) \
	X(119, void, ExecuteCommandList, ID3D11CommandList, *pCommandList, BOOL, RestoreContextState) \
	X(139, void, IAGetInputLayout, ID3D11InputLayout, **ppInputLayout) \
	X(144, void IAGetPrimitiveTopology, out D3D11_PRIMITIVE_TOPOLOGY, *pTopology) \
	X(155, void RSGetState, out ID3D11RasterizerState, **ppRasterizerState) \
	X(171, void, ClearState) \
	X(172, void, Flush) \
	X(173, void, GetType, out D3D11_RESOURCE_DIMENSION, *pResourceDimension) \
	X(174, UINT, GetContextFlags)\ 
	X(178, void, DiscardResource, ID3D11Resource, *pResource) \
	X(179, void, DiscardView, ID3D11View, *pResourceView) \
	X(196, HRESULT, CopyTileMappings, ID3D11Resource, *pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pDestRegionStartCoordinate, ID3D11Resource, *pSourceTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pSourceRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pTileRegionSize, UINT, Flags) \
	X(197, void, CopyTiles, ID3D11Resource, *pTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pTileRegionSize, ID3D11Buffer, *pBuffer, UINT64, BufferStartOffsetInBytes, UINT, Flags) \
	X(198, void, UpdateTiles, ID3D11Resource, const D3D11_TILED_RESOURCE_COORDINATE, *pDestTiledRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pDestTileRegionSize, const void, *pSourceTileData, UINT, Flags) \
	X(199, HRESULT, ResizeTilePool, ID3D11Bufer, *pTilePool, UINT64, NewSizeInBytes) \
	X(201, BOOL, IsAnnotationEnabled) \
	X(202, void, SetMarkerInt, LPCWSTR, pLabel, INT, Data) \
	X(203, void, BeginEventInt, LPCWSTR, pLabel, INT, Data) \
	X(204, void, EndEvent) \

#define X(idx, returnType, name, ...) \
	typedef returnType (__stdcall* name)(PARAMETER_TYPES(__VA_ARGS__)); \
	static name o##name = NULL; \

D3D11_METHODS
#undef X

// expands to hooked d3d11 function
#define X(idx, returnType, name, ...) \
	returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
		Sleep(methodDelays[idx]); \
		o##name(PARAMETER_NAMES(__VA_ARGS__)); \
	}\

D3D11_METHODS
#undef X
