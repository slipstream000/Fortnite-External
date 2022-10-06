#pragma once
#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <ntstatus.h>
#include <atomic>
#include <mutex>
#include <TlHelp32.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <xmmintrin.h>

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "dwmapi.lib")

typedef enum _KEY_VALUE_INFORMATION_CLASS {
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
	KeyValueFullInformationAlign64,
	KeyValuePartialInformationAlign64,
	KeyValueLayerInformation,
	MaxKeyValueInfoClass  // MaxKeyValueInfoClass should always be the last enum
} KEY_VALUE_INFORMATION_CLASS;

typedef struct _KEY_VALUE_FULL_INFORMATION {
	ULONG   TitleIndex;
	ULONG   Type;
	ULONG   DataOffset;
	ULONG   DataLength;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable size
//          Data[1];            // Variable size data not declared
} KEY_VALUE_FULL_INFORMATION, * PKEY_VALUE_FULL_INFORMATION;


#ifdef __cplusplus
extern "C++"
{
	char _RTL_CONSTANT_STRING_type_check(const char* s);
	char _RTL_CONSTANT_STRING_type_check(const WCHAR* s);
	// __typeof would be desirable here instead of sizeof.
	template <size_t N> class _RTL_CONSTANT_STRING_remove_const_template_class;
template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(char)> { public: typedef  char T; };
template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(WCHAR)> { public: typedef WCHAR T; };
#define _RTL_CONSTANT_STRING_remove_const_macro(s) \
    (const_cast<_RTL_CONSTANT_STRING_remove_const_template_class<sizeof((s)[0])>::T*>(s))
}
#else
char _RTL_CONSTANT_STRING_type_check(const void* s);
#define _RTL_CONSTANT_STRING_remove_const_macro(s) (s)
#endif
#define RTL_CONSTANT_STRING(s) \
{ \
    sizeof( s ) - sizeof( (s)[0] ), \
    sizeof( s ) / sizeof(_RTL_CONSTANT_STRING_type_check(s)), \
    _RTL_CONSTANT_STRING_remove_const_macro(s) \
}

extern "C" {
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
			_Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
			_In_ ULONG Length,
			_Out_ PULONG ResultLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwClose(
			_In_ HANDLE Handle
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenKey(
			_Out_ PHANDLE KeyHandle,
			_In_ ACCESS_MASK DesiredAccess,
			_In_ POBJECT_ATTRIBUTES ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
			_Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
			_In_ ULONG Length,
			_Out_ PULONG ResultLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_opt_ ULONG TitleIndex,
			_In_ ULONG Type,
			_In_reads_bytes_opt_(DataSize) PVOID Data,
			_In_ ULONG DataSize
		);

	NTSYSAPI NTSTATUS ZwCreateKey(
		PHANDLE            KeyHandle,
		ACCESS_MASK        DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes,
		ULONG              TitleIndex,
		PUNICODE_STRING    Class,
		ULONG              CreateOptions,
		PULONG             Disposition
	);
}

namespace RegistryUtils
{
	__forceinline ULONG GetKeyInfoSize(HANDLE hKey, PUNICODE_STRING Key)
	{
		NTSTATUS Status;
		ULONG KeySize;

		Status = ZwQueryValueKey(hKey, Key, KeyValueFullInformation, 0, 0, &KeySize);

		if (Status == STATUS_BUFFER_TOO_SMALL || Status == STATUS_BUFFER_OVERFLOW)
			return KeySize;

		return 0;
	}

	template <typename type>
	__forceinline type ReadRegistry(UNICODE_STRING RegPath, UNICODE_STRING Key)
	{
		HANDLE hKey;
		OBJECT_ATTRIBUTES ObjAttr;
		NTSTATUS Status = STATUS_UNSUCCESSFUL;

		InitializeObjectAttributes(&ObjAttr, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		Status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &ObjAttr);

		if (NT_SUCCESS(Status))
		{
			ULONG KeyInfoSize = GetKeyInfoSize(hKey, &Key);
			ULONG KeyInfoSizeNeeded;

			if (KeyInfoSize == NULL)
			{
				ZwClose(hKey);
				return 0;
			}

			PKEY_VALUE_FULL_INFORMATION pKeyInfo = (PKEY_VALUE_FULL_INFORMATION)malloc(KeyInfoSize);
			RtlZeroMemory(pKeyInfo, KeyInfoSize);

			Status = ZwQueryValueKey(hKey, &Key, KeyValueFullInformation, pKeyInfo, KeyInfoSize, &KeyInfoSizeNeeded);

			if (!NT_SUCCESS(Status) || (KeyInfoSize != KeyInfoSizeNeeded))
			{
				ZwClose(hKey);
				free(pKeyInfo);
				return 0;
			}

			ZwClose(hKey);
			free(pKeyInfo);

			return *(type*)((LONG64)pKeyInfo + pKeyInfo->DataOffset);
		}

		return 0;
	}

	__forceinline bool WriteRegistry(UNICODE_STRING RegPath, UNICODE_STRING Key, PVOID Address, ULONG Type, ULONG Size)
	{
		bool Success = false;
		HANDLE hKey;
		OBJECT_ATTRIBUTES ObjAttr;
		NTSTATUS Status = STATUS_UNSUCCESSFUL;

		InitializeObjectAttributes(&ObjAttr, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		Status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &ObjAttr);

		if (NT_SUCCESS(Status))
		{
			Status = ZwSetValueKey(hKey, &Key, NULL, Type, Address, Size);

			if (NT_SUCCESS(Status))
				Success = true;

			ZwClose(hKey);
		}
		else {
			Status = ZwCreateKey(&hKey, KEY_ALL_ACCESS, &ObjAttr, 0, &RegPath, 0, 0);

			if (NT_SUCCESS(Status))
			{
				Status = ZwSetValueKey(hKey, &Key, NULL, Type, Address, Size);

				if (NT_SUCCESS(Status))
					Success = true;
			}
			ZwClose(hKey);
		}

		return Success;
	}
}


//d3d11 w2s for ut4 engine games by n7

//==========================================================================================================================

//globals

DWORD Daimkey = VK_RBUTTON;		//aimkey
int aimheight = 46;				//aim height value
unsigned int asdelay = 90;		//use x-999 (shoot for xx millisecs, looks more legit)
bool IsPressed = false;			//
DWORD astime = timeGetTime();	//autoshoot timer

//init only once
bool firstTime = true;

//viewport
UINT vps = 1;
D3D11_VIEWPORT viewport;
float ScreenCenterX;
float ScreenCenterY;

//vertex
ID3D11Buffer *veBuffer;
UINT Stride = 24;
UINT veBufferOffset = 0;
D3D11_BUFFER_DESC vedesc;

//index
ID3D11Buffer *inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;

//rendertarget
ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView = NULL;

//shader
ID3D11PixelShader* psRed = NULL;
ID3D11PixelShader* psGreen = NULL;

//pssetshaderresources
UINT pssrStartSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  Descr;
ID3D11ShaderResourceView* ShaderResourceView;
D3D11_TEXTURE2D_DESC texdesc;

//psgetConstantbuffers
ID3D11Buffer *pcsBuffer;
D3D11_BUFFER_DESC pscdesc;
UINT pscStartSlot;

//vsgetconstantbuffers
ID3D11Buffer *mConstantBuffers;
UINT vsConstant_StartSlot;

UINT psStartSlot;
UINT vsStartSlot;



//==========================================================================================================================

//wh
char *state;
ID3D11RasterizerState * rwState;
ID3D11RasterizerState * rsState;



ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

void SetDepthStencilState(eDepthState aState)
{
	pContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
}
