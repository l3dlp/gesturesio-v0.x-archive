#include "XnMMsgParser.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ManagedNI
{
	XnMMsgParser::XnMMsgParser()
	{
		m_parser = new MsgParser();
	}

	XnMMsgParser::~XnMMsgParser()
	{
		delete m_parser;
	}

	XnMMsgParser::!XnMMsgParser()
	{
		delete m_parser;
	}

	String^ XnMMsgParser::Parse(String^ msg)
	{
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(msg).ToPointer());
		
		String^ str = gcnew String(m_parser->Parse(chars).c_str());

		Marshal::FreeHGlobal(IntPtr((void*)chars));

		return str;
	}
}