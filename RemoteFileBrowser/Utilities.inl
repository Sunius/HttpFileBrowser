// Implementation file for Utilities.h inline functions


#if _DEBUG

#define Assert(x) do { if (!(x) && IsDebuggerPresent()) { __debugbreak(); } } while (0)

#else

#define Assert(x)

#endif

// Logging

inline void Utilities::Logging::Win32ErrorToMessageInline(int win32ErrorCode, wchar_t(&buffer)[kBufferSize])
{
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, win32ErrorCode, 0, buffer, kBufferSize, nullptr);
}

template <typename Message>
inline void Utilities::Logging::OutputMessages(const Message& message)
{
	OutputMessage(message);
}

template <typename FirstMessage, typename ...Message>
inline void Utilities::Logging::OutputMessages(const FirstMessage& message, Message&& ...messages)
{
	OutputMessage(message);
	OutputMessages(std::forward<Message>(messages)...);
}

inline void Utilities::Logging::OutputMessage(const std::wstring& message)
{
	OutputMessage(message.c_str());
}

template <typename ...Message>
inline void Utilities::Logging::Log(Message&& ...message)
{
	using namespace std;
	lock_guard<mutex> lock(s_LogMutex);

	OutputCurrentTimestamp();
	OutputMessages(std::forward<Message>(message)...);
	OutputMessage(L"\r\n");
}

template <typename ...Message>
inline void Utilities::Logging::Error(int win32ErrorCode, Message&& ...message)
{
	wchar_t errorMessage[kBufferSize];

	Win32ErrorToMessageInline(win32ErrorCode, errorMessage);
	Log(std::forward<Message>(message)..., errorMessage);
}

template <typename ...Message>
inline void Utilities::Logging::FatalError(int win32ErrorCode, Message&& ...message)
{
	wchar_t errorMessage[kBufferSize];

	Win32ErrorToMessageInline(win32ErrorCode, errorMessage);
	Log(L"Terminating due to critical error:\r\n\t\t", std::forward<Message>(message)..., errorMessage);

	Terminate(win32ErrorCode);
}

template <typename Action, typename ...Message>
inline void Utilities::Logging::PerformActionIfFailed(bool failed, Action action, Message&& ...message)
{
	if (!failed)
	{
		return;
	}

	Assert(false);
	auto errorCode = GetLastError();
	action(errorCode, std::forward<Message>(message)...);
}

template <typename ...Message>
inline void Utilities::Logging::LogErrorIfFailed(bool failed, Message&& ...message)
{
	PerformActionIfFailed(failed, [](int errorCode, Message&& ...msg)
	{
		Error(errorCode, std::forward<Message>(msg)...);
	}, std::forward<Message>(message)...);
}

template <typename ...Message>
inline void Utilities::Logging::LogFatalErrorIfFailed(bool failed, Message&& ...message)
{
	PerformActionIfFailed(failed, [](int errorCode, Message&& ...msg)
	{
		FatalError(errorCode, std::forward<Message>(msg)...);
	}, std::forward<Message>(message)...);
}

// Encoding

inline std::wstring Utilities::Encoding::Utf8ToUtf16(const std::string& str)
{
	return Utf8ToUtf16(str.c_str(), str.length());
}

inline std::string Utilities::Encoding::Utf16ToUtf8(const std::wstring& wstr)
{
	return Utf16ToUtf8(wstr.c_str(), wstr.length());
}

inline std::string Utilities::Encoding::DecodeUrl(const std::string& url)
{
	std::string result = url;
	DecodeUrlInline(result);
	return result;
}

inline std::string Utilities::Encoding::EncodeUrl(const std::string& url)
{
	std::string result = url;
	EncodeUrlInline(result);
	return result;
}

// File system

inline std::string Utilities::FileSystem::RemoveLastPathComponent(const std::string& path)
{
	std::string result = path;
	RemoveLastPathComponentInline(result);
	return result;
}
