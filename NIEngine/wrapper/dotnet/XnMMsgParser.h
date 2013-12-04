#pragma once

#include "MsgParser.h"

using namespace System;

namespace ManagedNI
{
	public ref class XnMMsgParser
	{
	public:
		XnMMsgParser();
		~XnMMsgParser();

		!XnMMsgParser();

		String^ Parse(String^ msg);

	private:
		MsgParser* m_parser;
	};
}