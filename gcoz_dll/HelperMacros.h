
/*
  This file contains the macros used to generate the function definitions needed for hooking

  To precompile correctly I had to add /experimental:preprocessor to the cmd options of the solution.
  Otherwise, MSVC treats __VA_ARGS__ as one token and not as multiple seperated by a comma :))
  https://stackoverflow.com/questions/9183993/msvc-variadic-macro-expansion
*/

// https://stackoverflow.com/questions/45756920/c-c-preprocessor-extract-every-second-variadic-parameter
// These macros extract either the type or name of the parameter of a function defined below
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

#define EVERY_SECOND11_(second, ...) second, EVERY_SECOND10(__VA_ARGS__)
#define EVERY_SECOND11(first, ...) EVERY_SECOND11_(__VA_ARGS__)

#define EVERY_SECOND12_(second, ...) second, EVERY_SECOND11(__VA_ARGS__)
#define EVERY_SECOND12(first, ...) EVERY_SECOND12_(__VA_ARGS__)

#define EVERY_SECOND13_(second, ...) second, EVERY_SECOND12(__VA_ARGS__)
#define EVERY_SECOND13(first, ...) EVERY_SECOND13_(__VA_ARGS__)

#define EVERY_SECOND14_(second, ...) second, EVERY_SECOND13(__VA_ARGS__)
#define EVERY_SECOND14(first, ...) EVERY_SECOND14_(__VA_ARGS__)

#define EVERY_SECOND15_(second, ...) second, EVERY_SECOND14(__VA_ARGS__)
#define EVERY_SECOND15(first, ...) EVERY_SECOND15_(__VA_ARGS__)

#define COUNT_EVERY_SECOND_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,_6,__6,_7,__7,_8,__8,_9,__9,_10,__10,_11,__11,_12,__12,_13,__13,_14,__14,_15,__15, num,...) EVERY_SECOND##num
#define PARAMETER_NAMES(...) COUNT_EVERY_SECOND_(__VA_ARGS__,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

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
#define EVERY_FIRST11(first, second, ...) first, EVERY_FIRST10(__VA_ARGS__)
#define EVERY_FIRST12(first, second, ...) first, EVERY_FIRST11(__VA_ARGS__)
#define EVERY_FIRST13(first, second, ...) first, EVERY_FIRST12(__VA_ARGS__)
#define EVERY_FIRST14(first, second, ...) first, EVERY_FIRST13(__VA_ARGS__)
#define EVERY_FIRST15(first, second, ...) first, EVERY_FIRST14(__VA_ARGS__)

#define COUNT_EVERY_FIRST_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,_6,__6,_7,__7,_8,__8,_9,__9,_10,__10,_11,__11,_12,__12,_13,__13,_14,__14,_15,__15, num,...) EVERY_FIRST##num
#define PARAMETER_TYPES(...) COUNT_EVERY_FIRST_(__VA_ARGS__,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0)(__VA_ARGS__)

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
#define ALL11(first, second, ...) first second, ALL10(__VA_ARGS__)
#define ALL12(first, second, ...) first second, ALL11(__VA_ARGS__)
#define ALL13(first, second, ...) first second, ALL12(__VA_ARGS__)
#define ALL14(first, second, ...) first second, ALL13(__VA_ARGS__)
#define ALL15(first, second, ...) first second, ALL14(__VA_ARGS__)

#define COUNT_ALL_(_1, __1, _2, __2, _3, __3, _4, __4, _5, __5, _6, __6, _7, __7, _8, __8, _9, __9, _10, __10, _11, __11,_12,__12,_13,__13,_14,__14,_15,__15, num, ...) ALL##num
#define FUNCTION_SIGNATURE(...) COUNT_ALL_(__VA_ARGS__,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

// This macro defines all the parameters for the functions kiero can hook
// It uses x-macro expansion to generate the hook functions (happens in 'D3D11Hooks.h')
#define D3D11_METHODS \
	X(19, ULONG, AddRefDevice, ID3D11Device*, pDevice) \
	X(20, ULONG, ReleaseDevice, ID3D11Device*, pDevice) \
	X(21, HRESULT, CreateBufferDevice, ID3D11Device*, pDevice, const D3D11_BUFFER_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Buffer**, ppBuffer) \
	X(22, HRESULT, CreateTexture1DDevice, ID3D11Device*, pDevice, const D3D11_TEXTURE1D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture1D**, ppTexture1D) \
	X(23, HRESULT, CreateTexture2DDevice, ID3D11Device*, pDevice, const D3D11_TEXTURE2D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture2D**, ppTexture2D) \
	X(24, HRESULT, CreateTexture3DDevice, ID3D11Device*, pDevice, const D3D11_TEXTURE3D_DESC*, pDesc, const D3D11_SUBRESOURCE_DATA*, pInitialData, ID3D11Texture3D**, ppTexture3D) \
	X(25, HRESULT, CreateShaderResourceViewDevice, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC*, pDesc, ID3D11ShaderResourceView**, ppSRView) \
	X(26, HRESULT, CreateUnorderedAccessViewDevice, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC*, pDesc, ID3D11UnorderedAccessView**, ppUAView) \
	X(27, HRESULT, CreateRenderTargetViewDevice, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_RENDER_TARGET_VIEW_DESC*, pDesc, ID3D11RenderTargetView**, ppRTView) \
	X(28, HRESULT, CreateDepthStencilViewDevice, ID3D11Device*, pDevice, ID3D11Resource*, pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC*, pDesc, ID3D11DepthStencilView**, ppDepthStencilView) \
	X(29, HRESULT, CreateInputLayoutDevice, ID3D11Device*, pDevice, const D3D11_INPUT_ELEMENT_DESC*, pInputElementDescs, UINT, NumElements, const void*, pShaderBytecodeWithInputSignature, SIZE_T, BytecodeLength, ID3D11InputLayout**, ppInputLayout) \
	X(30, HRESULT, CreateVertexShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11VertexShader**, ppVertexShader) \
	X(31, HRESULT, CreateGeometryShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11GeometryShader**, ppGeometryShader) \
	X(32, HRESULT, CreateGeometryShaderWithStreamOutputDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, const D3D11_SO_DECLARATION_ENTRY*, pSODeclaration, UINT, NumEntries, const UINT*, pBufferStrides, UINT, NumStrides, UINT, RasterizedStream, ID3D11ClassLinkage*, pClassLinkage, ID3D11GeometryShader**, ppGeometryShader) \
	X(33, HRESULT, CreatePixelShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11PixelShader**, ppPixelShader) \
	X(34, HRESULT, CreateHullShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11HullShader**, ppHullShader) \
	X(35, HRESULT, CreateDomainShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11DomainShader**, ppDomainShader) \
	X(36, HRESULT, CreateComputeShaderDevice, ID3D11Device*, pDevice, const void*, pShaderBytecode, SIZE_T, BytecodeLength, ID3D11ClassLinkage*, pClassLinkage, ID3D11ComputeShader**, ppComputeShader) \
	X(37, HRESULT, CreateClassLinkageDevice, ID3D11Device*, pDevice, ID3D11ClassLinkage**, ppLinkage) \
	X(38, HRESULT, CreateBlendStateDevice, ID3D11Device*, pDevice, const D3D11_BLEND_DESC*, pBlendStateDesc, ID3D11BlendState**, ppBlendState) \
	X(39, HRESULT, CreateDepthStencilStateDevice, ID3D11Device*, pDevice, const D3D11_DEPTH_STENCIL_DESC*, pDepthStencilDesc, ID3D11DepthStencilState**, ppDepthStencilState) \
	X(40, HRESULT, CreateRasterizerStateDevice, ID3D11Device*, pDevice, const D3D11_RASTERIZER_DESC*, pRasterizerDesc, ID3D11RasterizerState**, ppRasterizerState) \
	X(41, HRESULT, CreateSamplerStateDevice, ID3D11Device*, pDevice, const D3D11_SAMPLER_DESC*, pSamplerDesc, ID3D11SamplerState**, ppSamplerState) \
	X(42, HRESULT, CreateQueryDevice, ID3D11Device*, pDevice, const D3D11_QUERY_DESC*, pQueryDesc, ID3D11Query**, ppQuery)\
	X(43, HRESULT, CreatePredicateContext, ID3D11DeviceContext*, pContext, const D3D11_QUERY_DESC*, pPredicateDesc, ID3D11Predicate**, ppPredicate) \
	X(44, HRESULT, CreateCounterContext, ID3D11DeviceContext*, pContext, const D3D11_COUNTER_DESC*, pCounterDesc, ID3D11Counter**, ppCounter) \
	X(45, HRESULT, CreateDeferredContextContext, ID3D11DeviceContext*, pContext, UINT, ContextFlags, ID3D11DeviceContext**, ppDeferredContext) \
	X(46, HRESULT, OpenSharedResourceContext, ID3D11DeviceContext*, pContext, HANDLE, hResource, REFIID, ReturnedInterface, void**, ppResource) \
	X(47, HRESULT, CheckFormatSupportContext, ID3D11DeviceContext*, pContext, DXGI_FORMAT, Format, UINT*, pFormatSupport) \
	X(48, HRESULT, CheckMultisampleQualityLevelsContext, ID3D11DeviceContext*, pContext, DXGI_FORMAT, Format, UINT, SampleCount, UINT*, pNumQualityLevels) \
	X(50, HRESULT, CheckCounterContext, ID3D11DeviceContext*, pContext, const D3D11_COUNTER_DESC*, pDesc, D3D11_COUNTER_TYPE*, pType, UINT*, pActiveCounters, LPSTR, szName, UINT*, pNameLength, LPSTR, szUnits, UINT*, pUnitsLength, LPSTR, szDescription, UINT*, pDescriptionLength) \
	X(52, HRESULT, GetPrivateDataContext, ID3D11DeviceContext*, pContext, REFGUID, Name, UINT*, pDataSize, void*, pData) \
	X(53, HRESULT, SetPrivateDataContext, ID3D11DeviceContext*, pContext, REFGUID, Name, UINT, DataSize, const void*, pData) \
	X(54, HRESULT, SetPrivateDataInterfaceContext, ID3D11DeviceContext*, pContext, REFGUID, Name, const IUnknown*, pUnknown) \
	X(56, UINT, GetCreationFlagsContext, ID3D11DeviceContext*, pContext) \
	X(57, HRESULT, GetDeviceRemovedReasonContext, ID3D11DeviceContext*, pContext) \
	X(59, HRESULT, SetExceptionModeContext, ID3D11DeviceContext*, pContext, UINT, RaiseFlags) \
	X(60, UINT, GetExceptionModeContext, ID3D11DeviceContext*, pContext) \
	X(61, HRESULT, QueryInterfaceContext, ID3D11DeviceContext*, pContext, REFIID, riid, void**, ppvObject) \
	X(62, ULONG, AddRefContext, ID3D11DeviceContext*, pContext) \
	X(63, ULONG, ReleaseContext, ID3D11DeviceContext*, pContext) \
	X(65, HRESULT, GetPrivateDataChild, ID3D11DeviceChild*, pChild, REFGUID, Name, UINT*, pDataSize, void*, pData) \
	X(66, HRESULT, SetPrivateDataChild, ID3D11DeviceChild*, pChild, REFGUID, Name, UINT, DataSize, const void*, pData) \
	X(67, HRESULT, SetPrivateDataInterfaceChild, ID3D11DeviceChild*, pChild, REFGUID, Name, const IUnknown*, pUnknown) \
	X(75, HRESULT, MapContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource, UINT, Subresource, D3D11_MAP, MapType, UINT, MapFlags, D3D11_MAPPED_SUBRESOURCE*, pMappedResource) \
	X(90, HRESULT, GetDataContext, ID3D11DeviceContext*, pContext, ID3D11Asynchronous*, pAsync, void*, pData, UINT, DataSize, UINT, GetDataFlags) \
	X(117, FLOAT, GetResourceMinLODContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource) \
	X(174, UINT, GetContextFlagsContext, ID3D11DeviceContext*, pContext) \
	X(175, HRESULT, FinishCommandListContext, ID3D11DeviceContext*, pContext, BOOL, RestoreDeferredContextState, ID3D11CommandList**, ppCommandList) \
	X(201, BOOL, IsAnnotationEnabledContext, ID3D11DeviceContext*, pContext) \

#define D3D11_METHODS_VOID \
	X(49, void, CheckCounterInfoContext, ID3D11DeviceContext*, pContext, D3D11_COUNTER_INFO*, pCounterInfo) \
	X(51, void, CheckFeatureSupportContext, ID3D11DeviceContext*, pContext, D3D11_FEATURE, Feature, void*, pFeatureSupportData, UINT, FeatureSupportDataSize) \
	X(55, void, GetFeatureLevelContext, ID3D11DeviceContext*, pContext, D3D_FEATURE_LEVEL*, pFeatureLevel) \
	X(58, void, GetImmediateContextContext, ID3D11DeviceContext*, pContext, ID3D11DeviceContext**, ppImmediateContext) \
	X(64, void, GetDeviceContext, ID3D11DeviceContext*, pContext, ID3D11Device**, ppDevice) \
	X(68, void, VSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(69, void, VSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(70, void, VSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11ComputeShader*, pComputeShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(71, void, VSSetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(72, void, HSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11DomainShader*, pShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(73, void, DrawIndexedContext, ID3D11DeviceContext*, pContext, UINT, IndexCount, UINT, StartIndexLocation, INT, BaseVertexLocation) \
	X(74, void, DrawContext, ID3D11DeviceContext*, pContext, UINT, VertexCount, UINT, StartVertexLocation) \
	X(76, void, UnmapContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource, UINT, Subresource) \
	X(77, void, HSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, ID3D11HullShader*, pShader, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(78, void, IASetInputLayoutContext, ID3D11DeviceContext*, pContext, ID3D11InputLayout*, pInputLayout) \
	X(79, void, IASetVertexBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppVertexBuffers, const UINT*, pStrides, const UINT*, pOffsets) \
	X(80, void, IASetIndexBufferContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pIndexBuffer, DXGI_FORMAT, Format, UINT, Offset) \
	X(81, void, DrawIndexedInstancedContext, ID3D11DeviceContext*, pContext, UINT, IndexCountPerInstance, UINT, InstanceCount, UINT, StartIndexLocation, INT, BaseVertexLocation, UINT, StartInstanceLocation) \
	X(82, void, DrawInstancedContext, ID3D11DeviceContext*, pContext, UINT, VertexCountPerInstance, UINT, InstanceCount, UINT, StartVertexLocation, UINT, StartInstanceLocation) \
	X(83, void, DSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, ID3D11DomainShader*, pShader, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(84, void, DSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11GeometryShader*, pShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(85, void, IASetPrimitiveTopologyContext, ID3D11DeviceContext*, pContext, D3D11_PRIMITIVE_TOPOLOGY, Topology) \
	X(86, void, HSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, ID3D11HullShader*, pShader, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(87, void, HSSetSamplersContext, ID3D11DeviceContext*, pContext, ID3D11HullShader*, pShader, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(88, void, BeginContext, ID3D11DeviceContext*, pContext, ID3D11Asynchronous*, pAsync) \
	X(89, void, EndContext, ID3D11DeviceContext*, pContext, ID3D11Asynchronous*, pAsync) \
	X(91, void, SetPredicationContext, ID3D11DeviceContext*, pContext, ID3D11Predicate*, pPredicate, BOOL, PredicateValue) \
	X(92, void, DSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(93, void, DSSetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(94, void, OMSetRenderTargetsContext, ID3D11DeviceContext*, pContext, UINT, NumViews, ID3D11RenderTargetView* const*, ppRenderTargetViews, ID3D11DepthStencilView*, pDepthStencilView) \
	X(95, void, OMSetRenderTargetsAndUnorderedAccessViewsContext, ID3D11DeviceContext*, pContext, UINT, NumRTVs, ID3D11RenderTargetView* const*, ppRenderTargetViews, ID3D11DepthStencilView*, pDepthStencilView, UINT, UAVStartSlot, UINT, NumUAVs, ID3D11UnorderedAccessView* const*, ppUnorderedAccessViews) \
	X(96, void, OMSetBlendStateContext, ID3D11DeviceContext*, pContext, ID3D11BlendState*, pBlendState, const FLOAT, BlendFactor[4], UINT, SampleMask) \
	X(97, void, OMSetDepthStencilStateContext, ID3D11DeviceContext*, pContext, ID3D11DepthStencilState*, pDepthStencilState, UINT, StencilRef) \
	X(98, void, SOSetTargetsContext, ID3D11DeviceContext*, pContext, UINT, NumBuffers, ID3D11Buffer* const*, ppSOTargets, const UINT*, pOffsets) \
	X(99, void, DrawAutoContext, ID3D11DeviceContext*, pContext) \
	X(100, void, DrawIndexedInstancedIndirectContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(101, void, DrawInstancedIndirectContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(102, void, DispatchContext, ID3D11DeviceContext*, pContext, UINT, ThreadGroupCountX, UINT, ThreadGroupCountY, UINT, ThreadGroupCountZ) \
	X(103, void, DispatchIndirectContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pBufferForArgs, UINT, AlignedByteOffsetForArgs) \
	X(104, void, RSSetStateContext, ID3D11DeviceContext*, pContext, ID3D11RasterizerState*, pRasterizerState) \
	X(105, void, RSSetViewportsContext, ID3D11DeviceContext*, pContext, UINT, NumViewports, const D3D11_VIEWPORT*, pViewports) \
	X(106, void, RSSetScissorRectsContext, ID3D11DeviceContext*, pContext, UINT, NumRects, const D3D11_RECT*, pRects) \
	X(107, void, CopySubresourceRegionContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, UINT, DstX, UINT, DstY, UINT, DstZ, ID3D11Resource*, pSrcResource, UINT, SrcSubresource, const D3D11_BOX*, pSrcBox) \
	X(108, void, CopyResourceContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, ID3D11Resource*, pSrcResource) \
	X(109, void, UpdateSubresourceContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, const D3D11_BOX*, pDstBox, const void*, pSrcData, UINT, SrcRowPitch, UINT, SrcDepthPitch) \
	X(110, void, CopyStructureCountContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pDstBuffer, UINT, DstAlignedByteOffset, ID3D11UnorderedAccessView*, pSrcView) \
	X(111, void, ClearRenderTargetViewContext, ID3D11DeviceContext*, pContext, ID3D11RenderTargetView*, pRenderTargetView, const FLOAT, ColorRGBA[4]) \
	X(112, void, ClearUnorderedAccessViewUintContext, ID3D11DeviceContext*, pContext, ID3D11UnorderedAccessView*, pUnorderedAccessView, const UINT, Values[4]) \
	X(113, void, ClearUnorderedAccessViewFloatContext, ID3D11DeviceContext*, pContext, ID3D11UnorderedAccessView*, pUnorderedAccessView, const FLOAT, Values[4]) \
	X(114, void, ClearDepthStencilViewContext, ID3D11DeviceContext*, pContext, ID3D11DepthStencilView*, pDepthStencilView, UINT, ClearFlags, FLOAT, Depth, UINT8, Stencil) \
	X(115, void, GenerateMipsContext, ID3D11DeviceContext*, pContext, ID3D11ShaderResourceView*, pShaderResourceView) \
	X(116, void, SetResourceMinLODContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource, FLOAT, MinLOD) \
	X(118, void, ResolveSubresourceContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, ID3D11Resource*, pSrcResource, UINT, SrcSubresource, DXGI_FORMAT, Format) \
	X(119, void, ExecuteCommandListContext, ID3D11DeviceContext*, pContext, ID3D11CommandList*, pCommandList, BOOL, RestoreContextState) \
	X(120, void, GSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(121, void, GSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11VertexShader*, pVertexShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(122, void, GSSetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(123, void, GSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(124, void, PSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(125, void, PSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11VertexShader*, pVertexShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(126, void, PSSetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(127, void, PSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(128, void, CSSetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView* const*, ppShaderResourceViews) \
	X(129, void, CSSetUnorderedAccessViewsContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumUAVs, ID3D11UnorderedAccessView* const*, ppUnorderedAccessViews, const UINT*, pUAVInitialCounts) \
	X(130, void, CSSetShaderContext, ID3D11DeviceContext*, pContext, ID3D11ComputeShader*, pComputeShader, ID3D11ClassInstance* const*, ppClassInstances, UINT, NumClassInstances) \
	X(131, void, CSSetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState* const*, ppSamplers) \
	X(132, void, CSSetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers) \
	X(133, void, VSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(134, void, PSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(135, void, PSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11PixelShader**, ppPixelShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(136, void, PSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(137, void, VSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11VertexShader**, ppVertexShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(138, void, PSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(139, void, IAGetInputLayoutContext, ID3D11DeviceContext*, pContext, ID3D11InputLayout**, ppInputLayout) \
	X(140, void, IAGetVertexBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppVertexBuffers, UINT*, pStrides, UINT*, pOffsets) \
	X(141, void, IAGetIndexBufferContext, ID3D11DeviceContext*, pContext, ID3D11Buffer**, ppIndexBuffer, DXGI_FORMAT*, pFormat, UINT*, pOffset) \
	X(142, void, GSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(143, void, GSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11GeometryShader**, ppGeometryShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(144, void, IAGetPrimitiveTopologyContext, ID3D11DeviceContext*, pContext, D3D11_PRIMITIVE_TOPOLOGY*, pTopology) \
	X(145, void, VSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(146, void, VSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(147, void, GetPredicationContext, ID3D11DeviceContext*, pContext, ID3D11Predicate**, ppPredicate, BOOL*, pPredicateValue) \
	X(148, void, GSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(149, void, GSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(150, void, OMGetRenderTargetsContext, ID3D11DeviceContext*, pContext, UINT, NumViews, ID3D11RenderTargetView**, ppRenderTargetViews, ID3D11DepthStencilView**, ppDepthStencilView) \
	X(151, void, OMGetRenderTargetsAndUnorderedAccessViewsContext, ID3D11DeviceContext*, pContext, UINT, NumRTVs, ID3D11RenderTargetView**, ppRenderTargetViews, ID3D11DepthStencilView**, ppDepthStencilView, UINT, UAVStartSlot, UINT, NumUAVs, ID3D11UnorderedAccessView**, ppUnorderedAccessViews) \
	X(152, void, OMGetBlendStateContext, ID3D11DeviceContext*, pContext, ID3D11BlendState**, ppBlendState, FLOAT*, BlendFactor, UINT*, SampleMask) \
	X(153, void, OMGetDepthStencilStateContext, ID3D11DeviceContext*, pContext, ID3D11DepthStencilState**, ppDepthStencilState, UINT*, pStencilRef) \
	X(154, void, SOGetTargetsContext, ID3D11DeviceContext*, pContext, UINT, NumBuffers, ID3D11Buffer**, ppSOTargets) \
	X(155, void, RSGetStateContext, ID3D11DeviceContext*, pContext, ID3D11RasterizerState**, ppRasterizerState) \
	X(156, void, RSGetViewportsContext, ID3D11DeviceContext*, pContext, UINT*, NumViewports, D3D11_VIEWPORT**, pViewports) \
	X(157, void, RSGetScissorRectsContext, ID3D11DeviceContext*, pContext, UINT*, NumRects, D3D11_RECT**, pRects) \
	X(158, void, HSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(159, void, HSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11HullShader**, ppHullShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(160, void, HSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(161, void, HSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(162, void, DSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(163, void, DSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11DomainShader**, ppDomainShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(164, void, DSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(165, void, DSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(166, void, CSGetShaderResourcesContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumViews, ID3D11ShaderResourceView**, ppShaderResourceViews) \
	X(167, void, CSGetUnorderedAccessViewsContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumUAVs, ID3D11UnorderedAccessView**, ppUnorderedAccessViews) \
	X(168, void, CSGetShaderContext, ID3D11DeviceContext*, pContext, ID3D11ComputeShader**, ppComputeShader, ID3D11ClassInstance**, ppClassInstances, UINT*, pNumClassInstances) \
	X(169, void, CSGetSamplersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumSamplers, ID3D11SamplerState**, ppSamplers) \
	X(170, void, CSGetConstantBuffersContext, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers) \
	X(171, void, ClearStateContext, ID3D11DeviceContext*, pContext) \
	X(172, void, FlushContext, ID3D11DeviceContext*, pContext) \
	X(173, void, GetTypeContext, ID3D11DeviceContext*, pContext) \
	X(176, void, CopySubresourceRegion1Context, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, UINT, DstX, UINT, DstY, UINT, DstZ, ID3D11Resource*, pSrcResource, UINT, SrcSubresource, const D3D11_BOX*, pSrcBox, UINT, CopyFlags) \
	X(177, void, UpdateSubresource1Context, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDstResource, UINT, DstSubresource, const D3D11_BOX*, pDstBox, const void*, pSrcData, UINT, SrcRowPitch, UINT, SrcDepthPitch, UINT, CopyFlags) \
	X(178, void, DiscardResourceContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pResource) \
	X(179, void, DiscardViewContext, ID3D11DeviceContext*, pContext, ID3D11View*, pResourceView) \
	X(180, void, VSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(181, void, HSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(182, void, DSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(183, void, GSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(184, void, PSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(185, void, CSSetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer* const*, ppConstantBuffers, const UINT*, pFirstConstant, const UINT*, pNumConstants) \
	X(186, void, VSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(187, void, HSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(188, void, DSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(189, void, GSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(190, void, PSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(191, void, CSGetConstantBuffers1Context, ID3D11DeviceContext*, pContext, UINT, StartSlot, UINT, NumBuffers, ID3D11Buffer**, ppConstantBuffers, UINT*, pFirstConstant, UINT*, pNumConstants) \
	X(192, void, SwapDeviceContextStateContext, ID3D11DeviceContext*, pContext, ID3D11DeviceContext*, pOtherContext) \
	X(193, void, ClearViewContext, ID3D11DeviceContext*, pContext, ID3D11View*, pView, const FLOAT, Color[4], const D3D11_RECT*, pRect, UINT, NumRects) \
	X(194, void, DiscardView1Context, ID3D11DeviceContext*, pContext, ID3D11View*, pResourceView, const D3D11_RECT*, pRects, UINT, NumRects) \
	X(195, void, UpdateTileMappingsContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pTiledResource, UINT, NumTiledResourceRegions, const D3D11_TILED_RESOURCE_COORDINATE*, pTiledResourceRegionStartCoordinates, const D3D11_TILE_REGION_SIZE*, pTiledResourceRegionSizes, ID3D11Buffer*, pBufferTileMappings, UINT, Flags) \
	X(196, void, CopyTileMappingsContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pDestRegionStartCoordinate, ID3D11Resource*, pSourceTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pSourceRegionStartCoordinate, const D3D11_TILE_REGION_SIZE*, pTileRegionSize, UINT, Flags) \
	X(197, void, CopyTilesContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pTileRegionStartCoordinate, ID3D11Buffer*, pBuffer, UINT, Flags) \
	X(198, void, UpdateTilesContext, ID3D11DeviceContext*, pContext, ID3D11Resource*, pDestTiledResource, const D3D11_TILED_RESOURCE_COORDINATE*, pDestTileRegionStartCoordinate, const D3D11_TILE_REGION_SIZE*, pDestTileRegionSize, const void*, pSourceTileData, UINT, Flags) \
	X(199, void, ResizeTilePoolContext, ID3D11DeviceContext*, pContext, ID3D11Buffer*, pTilePool, UINT64, NewSizeInBytes) \
	X(200, void, TiledResourceBarrierContext, ID3D11DeviceContext*, pContext, ID3D11DeviceChild*, pTiledResourceOrViewAccessBeforeBarrier, ID3D11DeviceChild*, pTiledResourceOrViewAccessAfterBarrier) \
	X(202, void, SetMarkerIntContext, ID3D11DeviceContext*, pContext, LPCWSTR, pLabel) \
	X(203, void, BeginEventIntContext, ID3D11DeviceContext*, pContext, LPCWSTR, pLabel) \
	X(204, void, EndEventContext, ID3D11DeviceContext*, pContext)

