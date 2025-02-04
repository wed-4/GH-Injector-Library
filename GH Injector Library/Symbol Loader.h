#pragma once

#include "pch.h"

#include "Error.h"
#include "Download Manager.h"

class SYMBOL_LOADER
{
	HANDLE			m_hPdbFile;
	std::wstring	m_szPdbPath;
	std::wstring	m_szModulePath;
	DWORD			m_Filesize;

	HANDLE	m_hInterruptEvent;
	bool	m_bInterruptEvent;

	DownloadManager m_DlMgr;
	float	m_fProgress;
	bool	m_bStartDownload;
	
	bool m_bReady;

	bool VerifyExistingPdb(const GUID & guid);

public:

	SYMBOL_LOADER();
	~SYMBOL_LOADER();

	DWORD Initialize(const std::wstring & szModulePath, const std::wstring & path, std::wstring * pdb_path_out, bool Redownload, bool WaitForConnection = false, bool AutoDownload = false);
	void Cleanup();

	void SetDownload(bool bDownload);
	void Interrupt();

	const std::wstring &	GetFilepath() const;
	DWORD					GetFilesize() const;
	float					GetDownloadProgress() const;

	bool IsReady() const;
};

struct PdbInfo
{
	DWORD	Signature;
	GUID	Guid;
	DWORD	Age;
	char	PdbFileName[1];
};

//Thanks mambda
//https://bitbucket.org/mambda/pdb-parser/src/master/
struct PDBHeader7
{
	char signature[0x20];
	int page_size;
	int allocation_table_pointer;
	int file_page_count;
	int root_stream_size;
	int reserved;
	int root_stream_page_number_list_number;
};

struct RootStream7
{
	int num_streams;
	int stream_sizes[ANYSIZE_ARRAY]; //num_streams
};

struct GUID_StreamData
{
	int ver;
	int date;
	int age;
	GUID guid;
};

#ifdef  _WIN64
inline SYMBOL_LOADER				sym_ntdll_wow64;
inline std::shared_future<DWORD>	sym_ntdll_wow64_ret;
#endif

inline SYMBOL_LOADER				sym_ntdll_native;
inline std::shared_future<DWORD>	sym_ntdll_native_ret;

#ifdef  _WIN64
inline SYMBOL_LOADER				sym_kernel32_wow64;
inline std::shared_future<DWORD>	sym_kernel32_wow64_ret;
#endif

inline SYMBOL_LOADER				sym_kernel32_native;
inline std::shared_future<DWORD>	sym_kernel32_native_ret;