#pragma once

#include "CriticalSection.h"

namespace Utilities
{
	class Logging
	{
	public:
		static const int kBufferSize = 256;

		static inline void Win32ErrorToMessageInline(int win32ErrorCode, wchar_t (&buffer)[kBufferSize]);
		static inline void Win32ErrorToMessageInline(int win32ErrorCode, char (&buffer)[kBufferSize]);
		static std::wstring Win32ErrorToMessage(int win32ErrorCode);
		
		static inline void OutputMessage(const std::string& message);
		static inline void OutputMessage(const char* message);
		template <size_t Length>
		static inline void OutputMessage(const char (&message)[Length]);
		static void OutputMessage(const char* message, size_t length);
		static void OutputCurrentTimestamp();

		template <typename ...Message>
		static inline void Log(Message&& ...message);
		template <typename ...Message>
		static inline void Error(int win32ErrorCode, Message&& ...message);
		template <typename ...Message>
		static inline void FatalError(int win32ErrorCode, Message&& ...message);

		template <typename ...Message>
		static inline void LogErrorIfFailed(bool failed, Message&& ...message);
		template <typename ...Message>
		static inline void LogFatalErrorIfFailed(bool failed, Message&& ...message);

		static void Initialize();
		static void Shutdown();

		Logging() = delete;
		Logging(const Logging&) = delete;
		~Logging() = delete;

	private:
		// Output* functions are NOT thread safe
		template <typename Message>
		static inline void OutputMessages(const Message& message);

		template <typename FirstMessage, typename ...Message>
		static inline void OutputMessages(const FirstMessage& message, Message&& ...messages);

		template <typename Action, typename ...Message>
		static inline void PerformActionIfFailed(bool failed, Action action, Message&& ...message);

		static void Terminate(int errorCode = -1);

		static CriticalSection s_LogCriticalSection;
	};

	namespace Encoding
	{
		size_t Utf8ToUtf16Inline(const char* str, size_t strLength, wchar_t* destination, size_t destinationLength);

		template <size_t SourceLength, size_t DestinationLength>
		inline size_t Utf8ToUtf16Inline(const char (&str)[SourceLength], wchar_t (&destination)[DestinationLength]);

		std::wstring Utf8ToUtf16(const char* str, size_t strLength);
		inline std::wstring Utf8ToUtf16(const std::string& str);

		size_t Utf16ToUtf8Inline(const wchar_t* wstr, size_t wstrLength, char* destination, size_t destinationLength);

		template <size_t SourceLength, size_t DestinationLength>
		inline size_t Utf16ToUtf8Inline(const wchar_t (&wstr)[SourceLength], char (&destination)[DestinationLength]);

		std::string Utf16ToUtf8(const wchar_t* wstr, size_t wstrLength);
		inline std::string Utf16ToUtf8(const std::wstring& wstr);

		void DecodeUrlInline(std::string& url);
		inline std::string DecodeUrl(const std::string& url);

		void EncodeUrlInline(std::string& url);
		inline std::string EncodeUrl(const std::string& url);

		void EncodeBase64Inline(std::string& data);
		inline std::string EncodeBase64(const std::string& data);
	}

	namespace FileSystem
	{
		enum class FileStatus
		{
			FileNotFound,
			AccessDenied,
			Directory,
			File
		};

		struct FileInfo
		{
			std::string fileName;
			FileStatus fileStatus;
			std::string dateModified;
			uint64_t fileSize;

			FileInfo(const std::string& fileName, FileStatus fileStatus, const std::string& dateModified, uint64_t fileSize);
			FileInfo(std::string&& fileName, FileStatus fileStatus, std::string&& dateModified, uint64_t fileSize);
		};

		void RemoveLastPathComponentInline(std::string& path);
		inline std::string RemoveLastPathComponent(const std::string& path);

		std::string CombinePaths(const std::string& left, const std::string& right);

		std::string FormatFileSizeString(uint64_t size);

		FileStatus QueryFileStatus(const std::wstring& path);
		std::vector<FileInfo> EnumerateFiles(std::wstring path);
		std::vector<std::string> EnumerateSystemVolumes();

		std::vector<uint8_t> ReadFileToVector(const std::wstring& path);
	}

	namespace System
	{
		const std::string& GetUniqueSystemId();
	}
};

#include "Utilities.inl"