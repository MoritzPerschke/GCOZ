
/*
  To precompile correctly I had to add /experimental:preprocessor to the cmd options of the solution.
  Otherwise, MSVC treats __VA_ARGS__ as one token and not as multiple seperated by a comma :))
  https://stackoverflow.com/questions/9183993/msvc-variadic-macro-expansion
*/

// https://stackoverflow.com/questions/45756920/c-c-preprocessor-extract-every-second-variadic-parameter
#define EVERY_SECOND0(...)

#define EVERY_SECOND1_(second, ...) second
#define EVERY_SECOND1(first, ...) EVERY_SECOND1_(__VA_ARGS__)

#define EVERY_SECOND2_(second, ...) second, EVERY_SECOND1(__VA_ARGS__)
#define EVERY_SECOND2(first, ...) EVERY_SECOND2_(__VA_ARGS__)

#define EVERY_SECOND3_(second, ...) second, EVERY_SECOND2(__VA_ARGS__)
#define EVERY_SECOND3(first, ...) EVERY_SECOND3_(__VA_ARGS__)

#define EVERY_SECOND4_(second, ...) second, EVERY_SECOND3(__VA_ARGS__)
#define EVERY_SECOND4(first, ...) EVERY_SECOND4_(__VA_ARGS__)

#define EVERY_SECOND5_(second, ...) second, EVERY_SECOND4(__VA_ARGS__)
#define EVERY_SECOND5(first, ...) EVERY_SECOND5_(__VA_ARGS__)

#define EVERY_SECOND6_(second, ...) second, EVERY_SECOND5(__VA_ARGS__)
#define EVERY_SECOND6(first, ...) EVERY_SECOND6_(__VA_ARGS__)

#define COUNT_EVERY_SECOND_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,_6,__6,num,...) EVERY_SECOND##num
#define PARAMETER_NAMES(...) COUNT_EVERY_SECOND_(__VA_ARGS__,6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for parameter types */
#define EVERY_FIRST1(first, ...) first
#define EVERY_FIRST2(first, second, ...) first, EVERY_FIRST1(__VA_ARGS__)
#define EVERY_FIRST3(first, second, ...) first, EVERY_FIRST2(__VA_ARGS__)
#define EVERY_FIRST4(first, second, ...) first, EVERY_FIRST3(__VA_ARGS__)
#define EVERY_FIRST5(first, second, ...) first, EVERY_FIRST4(__VA_ARGS__)
#define EVERY_FIRST6(first, second, ...) first, EVERY_FIRST5(__VA_ARGS__)
#define EVERY_FIRST7(first, second, ...) first, EVERY_FIRST6(__VA_ARGS__)

#define COUNT_EVERY_FIRST_(_1,__1,_2, __2,_3, __3,_4, __4,_5, __5, _6, __6, _7, __7, num,...) EVERY_FIRST##num
#define PARAMETER_TYPES(...) COUNT_EVERY_FIRST_(__VA_ARGS__, 7,7,6,6,5,5,4,4,3,3,2,2,1,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for function signatures */
#define ALL0()
#define ALL1(first, second) first second
#define ALL2(first, second, ...) first second, ALL1(__VA_ARGS__)
#define ALL3(first, second, ...) first second, ALL2(__VA_ARGS__)
#define ALL4(first, second, ...) first second, ALL3(__VA_ARGS__)
#define ALL5(first, second, ...) first second, ALL4(__VA_ARGS__)
#define ALL6(first, second, ...) first second, ALL5(__VA_ARGS__)

#define COUNT_ALL_(_1, __1, _2, __2, _3, __3, _4, __4, _5, __5, _6, __6, num, ...) ALL##num
#define FUNCTION_SIGNATURE(...) COUNT_ALL_(__VA_ARGS__, 6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

#define D3D11_METHODS \
	X(5, HRESULT, GetPrivateData, REFGUID, guid, UINT, *pDataSize, void*, pData) \
	X(10, HRESULT, SetFullscreenState, BOOL, Fullscreen, _In_opt_ IDXGIOutput, *pTarget) \
	X(13, HRESULT, ResizeBuffers, UINT, BufferCount, UINT, Width, UINT, Height, UINT, Format, DXGI_FORMAT, NewFormat, UINT, SwapChainFlags) \
	X(14, HRESULT, ResizeTarget, const DXGI_MODE_DESC, *pNewTargetParameters) \
	X(16, HRESULT, GetFrameStatistics, DXGI_FRAME_STATISTICS, *pStats) \
	X(37, HRESULT, CreateClassLinkage, ID3D11ClassLinkage, **ppLinkage) \
	X(47, HRESULT, CheckFormatSupport, DXGI_FORMAT, Format,  UINT, *pFormatSupport) \
	X(48, HRESULT, CheckMultisampleQualityLevels, DXGI_FORMAT, Format, UINT, SampleCount, UINT, *pNumQualityLevels) \
	X(51, HRESULT, CheckFeatureSupport, D3D11_FEATURE, Feature, void*, pFeatureSupportData, UINT, FeatureSupportDataSize) \
	X(55, D3D_FEATURE_LEVEL, GetFeatureLevel) \
	X(56, UINT, GetCreationFlags) \
	X(57, HRESULT, GetDeviceRemovedReason) \
	X(59, HRESULT, SetExceptionMode, UINT, RaiseFlags) \
	X(60, UINT, GetExceptionMode) \
	X(117, FLOAT, GetResourceMinLOD, ID3D11Resource, *pResource) \
	X(174, UINT, GetContextFlags) \
	X(196, HRESULT, CopyTileMappings, ID3D11Resource, *pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pDestRegionStartCoordinate, ID3D11Resource, *pSourceTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pSourceRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pTileRegionSize, UINT, Flags) \
	X(199, HRESULT, ResizeTilePool, ID3D11Buffer, *pTilePool, UINT64, NewSizeInBytes) \
	X(201, BOOL, IsAnnotationEnabled) \
	//X(3, HRESULT, SetPrivateData, REFGUID, guid, UINT, DataSize, const void*, pData) \

#define D3D11_METHODS_VOID \
	X(49, void, CheckCounterInfo, D3D11_COUNTER_INFO, *pCounterinfo) \
	X(58, void, GetImmediateContext, ID3D11DeviceContext, **ppImmediateContext) \
	X(64, void, GetDevice, ID3D11Device, **ppDevice) \
	X(73, void, DrawIndexed, UINT, IndexCount, UINT, StartIndexLocation, INT, BaseVertexLocation) \
	X(76, void, Unmap, ID3D11Resource, *pResource, UINT, Subresource) \
	X(81, void, DrawIndexedInstanced, UINT, IndexCountPerInstance, UINT, InstanceCount, UINT, StartIndexLocation, INT, BaseVertexLocation, UINT, StartInstanceLocation) \
	X(82, void, DrawInstanced, UINT, VertexCountPerInstance, UINT, InstanceCount, UINT, StartVertexLocation, UINT, StartInstanceLocation) \
	X(85, void, IASetPrimitiveTopology, D3D11_PRIMITIVE_TOPOLOGY, Topology) \
	X(88, void, Begin, ID3D11Asynchronous, *pAsync) \
	X(89, void, End, ID3D11Asynchronous, *pAsync) \
	X(99, void, DrawAuto) \
	X(100, void, DrawIndexedInstancedIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedOffsetForArgs) \
	X(101, void, DrawInstancedIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(102, void, Dispatch, UINT, ThreadGroupCountX, UINT, ThreadGroupCountY, UINT, ThreadGroupCountZ) \
	X(103, void, DispatchIndirect, ID3D11Buffer, *pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(108, void, CopyResource, ID3D11Resource, *pDstResource, ID3D11Resource, *pSrcResource) \
	X(110, void, CopyStructureCount, ID3D11Buffer, *pDstBuffer, UINT, DstAlignedByteOffset, ID3D11UnorderedAccessView, *pSrcView) \
	X(111, void, ClearRenderTargetView, ID3D11RenderTargetView, *pRenderTargetView, const FLOAT*, ColorRGBA) \
	X(115, void, GenerateMips, ID3D11ShaderResourceView, *pShaderResourceView) \
	X(112, void, ClearUnorderedAccessViewUnit, ID3D11UnorderedAccessView, *pUnorderedAccessView, const UINT*, Values) \
	X(113, void, ClearUnorderedAccessViewFloat, ID3D11UnorderedAccessView, *pUnorderedAccessView, const FLOAT*, Values) \
	X(114, void, ClearDepthStencilView, ID3D11DepthStencilView, *pDepthStencilView, UINT, ClearFlags, FLOAT, Depth, UINT8, Stencil) \
	X(116, void, SetResourceMinLOD, ID3D11Resource, *pResource, FLOAT, MinLOD) \
	X(118, void, ResolveSubresource, ID3D11Resource, *pDstResource, UINT, DstSubResource, ID3D11Resource, *pScrResource, UINT, SrcSubResource, DXGI_FORMAT, Format) \
	X(119, void, ExecuteCommandList, ID3D11CommandList, *pCommandList, BOOL, RestoreContextState) \
	X(139, void, IAGetInputLayout, ID3D11InputLayout, **ppInputLayout) \
	X(144, void, IAGetPrimitiveTopology, D3D11_PRIMITIVE_TOPOLOGY, *pTopology) \
	X(155, void, RSGetState, ID3D11RasterizerState, **ppRasterizerState) \
	X(171, void, ClearState) \
	X(172, void, Flush) \
	X(173, void, GetType, D3D11_RESOURCE_DIMENSION, *pResourceDimension) \
	X(178, void, DiscardResource, ID3D11Resource, *pResource) \
	X(179, void, DiscardView, ID3D11View, *pResourceView) \
	X(197, void, CopyTiles, ID3D11Resource, *pTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pTileRegionSize, ID3D11Buffer, *pBuffer, UINT64, BufferStartOffsetInBytes, UINT, Flags) \
	X(198, void, UpdateTiles, ID3D11Resource, *pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE, *pDestTiledRegionStartCoordinate, const D3D11_TILE_REGION_SIZE, *pDestTileRegionSize, const void*, pSourceTileData, UINT, Flags) \
	X(202, void, SetMarkerInt, LPCWSTR, pLabel, INT, Data) \
	X(203, void, BeginEventInt, LPCWSTR, pLabel, INT, Data) \
	X(204, void, EndEvent) \
	//X(74, void, Draw, UINT, VertexCount, UINT, StartVertexLocation) \
