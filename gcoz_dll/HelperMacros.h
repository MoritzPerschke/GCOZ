
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

#define EVERY_SECOND7_(second, ...) second, EVERY_SECOND6(__VA_ARGS__)
#define EVERY_SECOND7(first, ...) EVERY_SECOND7_(__VA_ARGS__)

#define EVERY_SECOND8_(second, ...) second, EVERY_SECOND7(__VA_ARGS__)
#define EVERY_SECOND8(first, ...) EVERY_SECOND8_(__VA_ARGS__)

#define EVERY_SECOND9_(second, ...) second, EVERY_SECOND8(__VA_ARGS__)
#define EVERY_SECOND9(first, ...) EVERY_SECOND9_(__VA_ARGS__)

#define EVERY_SECOND10_(second, ...) second, EVERY_SECOND9(__VA_ARGS__)
#define EVERY_SECOND10(first, ...) EVERY_SECOND10_(__VA_ARGS__)

#define COUNT_EVERY_SECOND_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,_6,__6,_7,__7,_8,__8,_9,__9,_10,__10,num,...) EVERY_SECOND##num
#define PARAMETER_NAMES(...) COUNT_EVERY_SECOND_(__VA_ARGS__,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for parameter types */
#define EVERY_FIRST1(first, ...) first
#define EVERY_FIRST2(first, second, ...) first, EVERY_FIRST1(__VA_ARGS__)
#define EVERY_FIRST3(first, second, ...) first, EVERY_FIRST2(__VA_ARGS__)
#define EVERY_FIRST4(first, second, ...) first, EVERY_FIRST3(__VA_ARGS__)
#define EVERY_FIRST5(first, second, ...) first, EVERY_FIRST4(__VA_ARGS__)
#define EVERY_FIRST6(first, second, ...) first, EVERY_FIRST5(__VA_ARGS__)
#define EVERY_FIRST7(first, second, ...) first, EVERY_FIRST6(__VA_ARGS__)
#define EVERY_FIRST8(first, second, ...) first, EVERY_FIRST7(__VA_ARGS__)
#define EVERY_FIRST9(first, second, ...) first, EVERY_FIRST8(__VA_ARGS__)
#define EVERY_FIRST10(first, second, ...) first, EVERY_FIRST9(__VA_ARGS__)

#define COUNT_EVERY_FIRST_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,_6,__6,_7,__7,_8,__8,_9,__9,_10,__10, num,...) EVERY_FIRST##num
#define PARAMETER_TYPES(...) COUNT_EVERY_FIRST_(__VA_ARGS__, 10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for function signatures */
#define ALL0()
#define ALL1(first, second) first second
#define ALL2(first, second, ...) first second, ALL1(__VA_ARGS__)
#define ALL3(first, second, ...) first second, ALL2(__VA_ARGS__)
#define ALL4(first, second, ...) first second, ALL3(__VA_ARGS__)
#define ALL5(first, second, ...) first second, ALL4(__VA_ARGS__)
#define ALL6(first, second, ...) first second, ALL5(__VA_ARGS__)
#define ALL7(first, second, ...) first second, ALL6(__VA_ARGS__)
#define ALL8(first, second, ...) first second, ALL7(__VA_ARGS__)
#define ALL9(first, second, ...) first second, ALL8(__VA_ARGS__)
#define ALL10(first, second, ...) first second, ALL9(__VA_ARGS__)

#define COUNT_ALL_(_1, __1, _2, __2, _3, __3, _4, __4, _5, __5, _6, __6, _7, __7, _8, __8, _9, __9, _10, __10, num, ...) ALL##num
#define FUNCTION_SIGNATURE(...) COUNT_ALL_(__VA_ARGS__, 10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

#define D3D11_METHODS \
	X(3, HRESULT, SetPrivateDataDevice, ID3D11Device*, pDevice, REFGUID, guid, UINT, DataSize, const void*, pData) \
	X(4, HRESULT, SetPrivateDataInterfaceDevice, ID3D11Device*, pDevice, REFGUID, guid) \
	X(5, HRESULT, GetPrivateDataDevice, ID3D11Device*, pDevice, REFGUID, guid, UINT*, pDataSize, void*, pData)\
	X(6, HRESULT, GetParent, IDXGIObject*, pObject, REFIID, riid, void**, pParent)\
	X(10, HRESULT, SetFullscreenState, IDXGISwapChain*, pSwapChain, BOOL, Fullscreen) \
	X(11, HRESULT, GetFullscreenState, IDXGISwapChain*, pSwapChain, BOOL*, pFullscreen, IDXGIOutput**, ppTarget)\
	X(13, HRESULT, ResizeBuffers, IDXGISwapChain*, pSwapChain, UINT, BufferCount, UINT, Width, UINT, Height, UINT, Format, DXGI_FORMAT, NewFormat, UINT, SwapChainFlags) \
	X(14, HRESULT, ResizeTarget, IDXGISwapChain*, pSwapChain, const DXGI_MODE_DESC*, pNewTargetParameters) \
	X(16, HRESULT, GetFrameStatistics, IDXGISwapChain*, pSwapChain, DXGI_FRAME_STATISTICS*, pStats) \
	X(37, HRESULT, CreateClassLinkage, ID3D11Device*, pDevice, ID3D11ClassLinkage**, ppLinkage) \
	X(47, HRESULT, CheckFormatSupport, ID3D11Device*, pDevice, DXGI_FORMAT, Format,  UINT*, pFormatSupport) \
	X(48, HRESULT, CheckMultisampleQualityLevels, ID3D11Device*, pDevice, DXGI_FORMAT, Format, UINT, SampleCount, UINT*, pNumQualityLevels) \
	X(51, HRESULT, CheckFeatureSupport, ID3D11Device*, pDevice, D3D11_FEATURE, Feature, void*, pFeatureSupportData, UINT, FeatureSupportDataSize) \
	X(55, D3D_FEATURE_LEVEL, GetFeatureLevel, ID3D11Device*, pDevice) \
	X(56, UINT, GetCreationFlags, ID3D11Device*, pDevice) \
	X(57, HRESULT, GetDeviceRemovedReason, ID3D11Device*, pDevice) \
	X(59, HRESULT, SetExceptionMode, ID3D11Device*, pDevice, UINT, RaiseFlags) \
	X(60, UINT, GetExceptionMode, ID3D11Device*, pDevice) \
	X(117, FLOAT, GetResourceMinLOD, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pResource) \
	X(174, UINT, GetContextFlags, ID3D11DeviceContext*, pDeviceContext) \
	X(196, HRESULT, CopyTileMappings, ID3D11DeviceContext2*, pDeviceContext, ID3D11Resource*, pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pDestRegionStartCoordinate, ID3D11Resource*, pSourceTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pSourceRegionStartCoordinate, const D3D11_TILE_REGION_SIZE*, pTileRegionSize, UINT, Flags) \
	X(199, HRESULT, ResizeTilePool, ID3D11DeviceContext2*, pDeviceContext, ID3D11Buffer*, pTilePool, UINT64, NewSizeInBytes) \
	X(201, BOOL, IsAnnotationEnabled, ID3D11DeviceContext2*, pDeviceContext) \
	X(15, HRESULT, GetContainingOutput, IDXGISwapChain*, pSwapChain, IDXGIOutput**, ppOutput)\
	X(17, HRESULT, GetLastPresentCount, UINT*, pLastPresentCount)\
	X(21, HRESULT, CreateBuffer, ID3D11Device*, pDevice, const D3D11_BUFFER_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Buffer**, ppBuffer)\
	X(22, HRESULT, CreateTexture1D, ID3D11Device*, pDevice, const D3D11_TEXTURE1D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture1D**, ppTexture1D)\
	X(23, HRESULT, CreateTexture2D, ID3D11Device*, pDevice, const D3D11_TEXTURE2D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture2D**, ppTexture2D)\
	X(24, HRESULT, CreateTexture3D, ID3D11Device*, pDevice, const D3D11_TEXTURE3D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture3D**, ppTexture3D)\
	X(25, HRESULT, CreateShaderResourceView, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC*, pDesc, ID3D11ShaderResourceView**, ppSRView)\
	X(26, HRESULT, CreateUnorderedAccessView, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC*, pDesc, ID3D11UnorderedAccessView**, ppUAView)\
	X(27, HRESULT, CreateRenderTargetView, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_RENDER_TARGET_VIEW_DESC*, pDesc, ID3D11RenderTargetView**, ppRTView)\
	X(28, HRESULT, CreateDepthStencilView, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC*, pDesc, ID3D11DepthStencilView**, ppDepthStencilView)\
	X(29, HRESULT, CreateInputLayout, ID3D11Device*, pDevice, const D3D11_INPUT_ELEMENT_DESC*, pInputElementDesc, UINT, NumElements, const void*, pShaderByteCodeWithInputSignature, SIZE_T, ByteCodeLength, ID3D11InputLayout**, ppInputLayout)\
	X(30, HRESULT, CreateVertexShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11VertexShader**, ppVertexShader)\
	X(31, HRESULT, CreateGeometryShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11GeometryShader**, ppGeometryShader)\
	X(32, HRESULT, CreateGeometryShaderWithStreamOutput, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength, const D3D11_SO_DECLARATION_ENTRY*, pSODeclaration, UINT, NumEntries, const UINT*, pBufferStrides, UINT, NumStrides, UINT, RasterizedStream, ID3D11ClassLinkage*, pClassLinkage, ID3D11GeometryShader**, ppGeometryShader)\
	X(33, HRESULT, CreatePixelShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength,  ID3D11ClassLinkage*, pClassLinkage, ID3D11PixelShader**, ppPixelShader)\
	X(34, HRESULT, CreateHullShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength,  ID3D11ClassLinkage*, pClassLinkage, ID3D11HullShader**, ppHullShader)\
	X(35, HRESULT, CreateDomainShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength,  ID3D11ClassLinkage*, pClassLinkage, ID3D11DomainShader**, ppDomainShader)\
	X(36, HRESULT, CreateComputeShader, ID3D11Device*, pDevice, const void*, pShaderByteCode, SIZE_T, BytecodeLength,  ID3D11ClassLinkage*, pClassLinkage, ID3D11ComputeShader**, ppComputeShader)\
	X(38, HRESULT, CreateBlendState, ID3D11Device*, pDevice, const D3D11_BLEND_DESC*, pBlendStateDesc, ID3D11BlendState**, ppBlendState)\
	X(39, HRESULT, CreateDepthStencilState, ID3D11Device*, pDevice, const D3D11_DEPTH_STENCIL_DESC**, pDepthStencilDesc, ID3D11DepthStencilState**, ppDepthStencilState)\
	X(40, HRESULT, CreateRasterizerState, ID3D11Device*, pDevice, const D3D11_RASTERIZER_DESC*, pRasterizerDesc, ID3D11RasterizerState**, ppRasterizerState)\
	X(41, HRESULT, CreateSamplerState, ID3D11Device*, pDevice, const D3D11_SAMPLER_DESC*, pSamplerDesc, ID3D11SamplerState**, ppSamplerState)\
	X(42, HRESULT, CreateQuery, ID3D11Device*, pDevice,const D3D11_QUERY_DESC*, pQueryDesc, ID3D11Query**, ppQuery)\
	X(43, HRESULT, CreatePredicate, ID3D11Device*, pDevice, const D3D11_QUERY_DESC*, pPrediacteDesc, ID3D11Predicate**, ppPredicate)\
	X(44, HRESULT, CreateCounter, ID3D11Device*, pDevice, const D3D11_COUNTER_DESC*, pCounterDesc, ID3D11Counter**, ppCounter)\
	X(45, HRESULT, CreateDeferredContext, ID3D11Device*, pDevice, UINT, ContextFlags, ID3D11DeviceContext**, ppDeferredContext)\
	X(46, HRESULT, OpenSharedResource, ID3D11Device*, pDevice, HANDLE, hResource, REFIID, ReturnedInterface, void**, ppResource)\
	X(50, HRESULT, CheckCounter, ID3D11Device*, pDevice, const D3D11_COUNTER_DESC*, pDesc, D3D11_COUNTER_TYPE*, pType, UINT*, pActiveCounters, LPSTR, szName, UINT*, pNameLength, LPSTR, szUnits, UINT*, pUnitsLength, LPSTR, szDescription, UINT*, pDescriptionLength)\
	X(52, HRESULT, GetPrivateDataObject, IDXGIObject*, pObject, REFGUID, Name, UINT*, pDataSize, void*, pData)\
	X(53, HRESULT, SetPrivateDataObject, IDXGIObject*, pObject, REFGUID, Name, UINT, DataSize, const void*, pData)\
	X(54, HRESULT, SetPrivateDataInterfaceObject, IDXGIObject*, pObject, REFGUID, Name, const IUnknown*, pUnknown)\
	X(75, HRESULT, Map, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource, UINT, Subresource, D3D11_MAP, MapType, UINT, MapFlags, D3D11_MAPPED_SUBRESOURCE*, pMappedResource)\
	X(90, HRESULT, GetData, ID3D11DeviceContext*, pContext, ID3D11Asynchronous*, pAsync, void*, pData, UINT, DataSize, UINT, GetDataFlags)\

#define D3D11_METHODS_VOID \
	X(49, void, CheckCounterInfo, ID3D11Device*, pDevice, D3D11_COUNTER_INFO*, pCounterinfo) \
	X(58, void, GetImmediateContext, ID3D11Device*, pDevice, ID3D11DeviceContext**, ppImmediateContext) \
	X(64, void, GetDevice, ID3D11DeviceChild*, pDeviceChild, ID3D11Device**, ppDevice) \
	X(73, void, DrawIndexed, ID3D11DeviceContext*, pDeviceContext, UINT, IndexCount, UINT, StartIndexLocation, INT, BaseVertexLocation) \
	X(74, void, Draw, ID3D11DeviceContext*, pDeviceContext, UINT, VertexCount, UINT, StartVertexLocation) \
	X(76, void, Unmap, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pResource, UINT, Subresource) \
	X(81, void, DrawIndexedInstanced, ID3D11DeviceContext*, pDeviceContext, UINT, IndexCountPerInstance, UINT, InstanceCount, UINT, StartIndexLocation, INT, BaseVertexLocation, UINT, StartInstanceLocation) \
	X(82, void, DrawInstanced, ID3D11DeviceContext*, pDeviceContext, UINT, VertexCountPerInstance, UINT, InstanceCount, UINT, StartVertexLocation, UINT, StartInstanceLocation) \
	X(85, void, IASetPrimitiveTopology, ID3D11DeviceContext*, pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY, Topology) \
	X(88, void, Begin, ID3D11DeviceContext*, pDeviceContext, ID3D11Asynchronous*, pAsync) \
	X(89, void, End, ID3D11DeviceContext*, pDeviceContext, ID3D11Asynchronous*, pAsync) \
	X(99, void, DrawAuto, ID3D11DeviceContext*, pDeviceContext) \
	X(100, void, DrawIndexedInstancedIndirect, ID3D11DeviceContext*, pDeviceContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedOffsetForArgs) \
	X(101, void, DrawInstancedIndirect, ID3D11DeviceContext*, pDeviceContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(102, void, Dispatch, ID3D11DeviceContext*, pDeviceContext, UINT, ThreadGroupCountX, UINT, ThreadGroupCountY, UINT, ThreadGroupCountZ) \
	X(103, void, DispatchIndirect, ID3D11DeviceContext*, pDeviceContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(108, void, CopyResource, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pDstResource, ID3D11Resource*, pSrcResource) \
	X(110, void, CopyStructureCount, ID3D11DeviceContext*, pDeviceContext, ID3D11Buffer*, pDstBuffer, UINT, DstAlignedByteOffset, ID3D11UnorderedAccessView*, pSrcView) \
	X(111, void, ClearRenderTargetView, ID3D11DeviceContext*, pDeviceContext, ID3D11RenderTargetView*, pRenderTargetView, const FLOAT*, ColorRGBA) \
	X(115, void, GenerateMips, ID3D11DeviceContext*, pDeviceContext, ID3D11ShaderResourceView*, pShaderResourceView) \
	X(112, void, ClearUnorderedAccessViewUint, ID3D11DeviceContext*, pDeviceContext, ID3D11UnorderedAccessView*, pUnorderedAccessView, const UINT*, Values) \
	X(113, void, ClearUnorderedAccessViewFloat, ID3D11DeviceContext*, pDeviceContext, ID3D11UnorderedAccessView*, pUnorderedAccessView, const FLOAT*, Values) \
	X(114, void, ClearDepthStencilView, ID3D11DeviceContext*, pDeviceContext, ID3D11DepthStencilView*, pDepthStencilView, UINT, ClearFlags, FLOAT, Depth, UINT8, Stencil) \
	X(116, void, SetResourceMinLOD, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pResource, FLOAT, MinLOD) \
	X(118, void, ResolveSubresource, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pDstResource, UINT, DstSubResource, ID3D11Resource*, pScrResource, UINT, SrcSubResource, DXGI_FORMAT, Format) \
	X(119, void, ExecuteCommandList, ID3D11DeviceContext*, pDeviceContext, ID3D11CommandList*, pCommandList, BOOL, RestoreContextState) \
	X(139, void, IAGetInputLayout, ID3D11DeviceContext*, pDeviceContext, ID3D11InputLayout**, ppInputLayout) \
	X(144, void, IAGetPrimitiveTopology, ID3D11DeviceContext*, pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY*, pTopology) \
	X(155, void, RSGetState, ID3D11DeviceContext*, pDeviceContext, ID3D11RasterizerState**, ppRasterizerState) \
	X(171, void, ClearState, ID3D11DeviceContext*, pDeviceContext) \
	X(172, void, Flush, ID3D11DeviceContext*, pDeviceContext) \
	X(173, void, GetType, ID3D11Resource*, pResource, D3D11_RESOURCE_DIMENSION*, pResourceDimension) \
	X(178, void, DiscardResource, ID3D11DeviceContext*, pDeviceContext, ID3D11Resource*, pResource) \
	X(179, void, DiscardView, ID3D11DeviceContext1*, pDeviceContext, ID3D11View*, pResourceView) \
	X(197, void, CopyTiles, ID3D11DeviceContext2*, pDeviceContext, ID3D11Resource*, pTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE*, pTileRegionSize, ID3D11Buffer*, pBuffer, UINT64, BufferStartOffsetInBytes, UINT, Flags) \
	X(198, void, UpdateTiles, ID3D11DeviceContext2*, pDeviceContext, ID3D11Resource*, pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pDestTiledRegionStartCoordinate, const D3D11_TILE_REGION_SIZE*, pDestTileRegionSize, const void*, pSourceTileData, UINT, Flags) \
	X(202, void, SetMarkerInt, ID3D11DeviceContext2*, pDeviceContext, LPCWSTR, pLabel, INT, Data) \
	X(203, void, BeginEventInt, ID3D11DeviceContext2*, pDeviceContext, LPCWSTR, pLabel, INT, Data) \
	X(204, void, EndEvent, ID3D11DeviceContext2*, pDeviceContext) \
	X(12, void, GetDesc, ID3D11Buffer*, pBuffer, D3D11_BUFFER_DESC*, pDesc)\
	X(78, void, IASetInputLayout, ID3D11DeviceContext*, pContext, ID3D11InputLayout*, pInputLayout)\
	X(79, void, IASetVertexBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer *const*, ppVertexBuffers, const UINT*, pStrides, const UINT*, pOffsets)\
	X(80, void, IASetIndexBuffer, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pIndexBuffer, DXGI_FORMAT, Format, UINT, Offset)\
	X(91, void, SetPredication, ID3D11DeviceContext*, pContext, ID3D11Predicate*, pPredicate, BOOL, PredicateValue)\
	X(94, void, OMSetRenderTargets, ID3D11DeviceContext*, pContext, UINT, NumViews, ID3D11RenderTargetView *const*, ppRenderTargetViews, ID3D11DepthStencilView*, pDepthStencilView)\
	X(95, void, OMSetRenderTargetsAndUnorderedAccessViews, ID3D11DeviceContext*, pContext, UINT, NumRTVs, ID3D11RenderTargetView *const*, ppRenderTargetViews,  ID3D11DepthStencilView*, pDepthStencilView, UINT, UAVStartSlot, UINT, numUAVs, ID3D11UnorderedAccessView *const*, ppUnorderedAccessViews, const UINT*, pUAVInitialCounts)\
	X(96, void, OMSetBlendState, ID3D11DeviceContext*, pContext, ID3D11BlendState*, pBlendState, const FLOAT*, BlendFactor, UINT, SampleMask)\
	X(97, void, OMSetDepthStencilState, ID3D11DeviceContext*, pContext, ID3D11DepthStencilState*, pDepthStencilState, UINT, StencilRef)\
	X(98, void, SOSetTargets, ID3D11DeviceContext*, pContext, UINT, NumBuffers, ID3D11Buffer *const*, ppSOTargets, const UINT*, pOffsets)\
	X(104, void, RSSetState, ID3D11DeviceContext*, pContext, ID3D11RasterizerState*, pRasterizerState)\
	X(105, void, RSSetViewports, ID3D11DeviceContext*, pContext, UINT, NumViewports, const D3D11_VIEWPORT*, pViewPorts)\
	X(106, void, RSSetScissorRects, ID3D11DeviceContext*, pContext, UINT, NumRects, const D3D11_RECT*, pRects)\
	X(107, void, CopySubresourceRegion, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, UINT, DstX, UINT, DstY, UINT, DstZ, ID3D11Resource*, pSrcResource, UINT, SrcSubResource, const D3D11_BOX*, pSrcBox)\
	X(109, void, UpdateSubresource, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, const D3D11_BOX*, pDstBox, const void*, pScrData, UINT, SrcRowPitch, UINT, SrcDepthPitch)\
	X(129, void, CSSetUnorderedAccessViews, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumUAVs, ID3D11UnorderedAccessView *const*, ppUnorderedAccessViews, const UINT*, pUAVInitialCounts)\
	X(130, void, CSSetShader, ID3D11DeviceContext*, pContext, ID3D11ComputeShader*, pComputeShader, ID3D11ClassInstance *const*, ppClassInstances, UINT, NumClassInstances)\
	X(131, void, CSSetSamplers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState *const*, ppSamplers)\
	X(132, void, CSSetConstantBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer *const*, ppConstantBuffers)\
	X(133, void, VSGetConstantBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers)\
	X(134, void, PSGetShaderResources, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews)\
	X(135, void, PSGetShader, ID3D11DeviceContext*, pContext, ID3D11PixelShader**, ppPixelShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances)\
	X(136, void, PSGetSamplers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers)\
	X(137, void, VSGetShader, ID3D11DeviceContext*, pContext, ID3D11VertexShader**, ppVertexShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances)\
	X(138, void, PSGetConstantBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers)\
	X(140, void, IAGetVertexBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppVertexBuffer, UINT*, pStrides, UINT*, pOffsets)\
	X(141, void, IAGetIndexBuffer, ID3D11DeviceContext*, pContext, ID3D11Buffer**, ppIndexBuffer, DXGI_FORMAT*, Format, UINT*, Offset)\
	X(142, void, GSGetConstantBuffers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers)\
	X(143, void, GSGetShader, ID3D11DeviceContext*, pContext, ID3D11GeometryShader**, ppGeometryShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances)\
	X(145, void, VSGetShaderResources, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews)\
	X(146, void, VSGetSamplers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers)\
	X(147, void, GetPredication, ID3D11DeviceContext*, pContext, ID3D11Predicate**, ppPredicate, BOOL*, pPredicateValue)\
	X(148, void, GSGetShaderResources, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews)\
	X(149, void, GSGetSamplers, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers)\
	X(150, void, OMGetRenderTargets, ID3D11DeviceContext*, pContext, UINT, NumViews, ID3D11RenderTargetView**, ppRenderTargetView, ID3D11DepthStencilView**, ppDepthStencilView)\
