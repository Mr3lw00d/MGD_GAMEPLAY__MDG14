#include "LOGManager.h"
#include "glog\logging.h"
#include "tinyxml2.h"
#include <stdio.h>

namespace MGD_LOG
{
	const char* LOGManager::m_szFileName("Data/LogConfiguration.xml");

	void LOGManager::CreateInstance()
	{
		if(GetSingletonPtr() == NULL)
			new LOGManager();
	}

	LOGManager::LOGManager()
	{
		//TODO -> Studente MGD
		
		//check if file exist
		tinyxml2::XMLDocument oDocument;
		
		const char* strContextLabel = "Context";
		const char* strValueLabel = "value";

		if (oDocument.LoadFile(LOGManager::m_szFileName)==0)
		{
			const tinyxml2::XMLElement *pRoot = oDocument.RootElement(); //ICD: get the root
			
			const tinyxml2::XMLElement *pContext = pRoot->FirstChildElement(strContextLabel);
			while (pContext != nullptr)
			{
				AddLogContext(ObjectId( pContext->Attribute(strValueLabel)));
				pContext = pContext->NextSiblingElement(strContextLabel);
			}
		}
		else
		{
			//ICD: Not a valid file
		}
		/*	Funzione che aggiunge il contesto

			AddLogContext(CONTESTO_COME_OBJECTID);
		*/
	}

	LOGManager::~LOGManager()
	{
	}

	void LOGManager::WriteLog( LogSeverity i_oLogSeverity, const ObjectId& i_oContext, const char* i_szMassage, ...)
	{
		MGDVector<ObjectId>::iterator it = m_LogContextAvailable.begin();
		for(;it != m_LogContextAvailable.end();++it)
		{
			if( (*it) == i_oContext )
			{
				// ******** TODO-> improve this part of code
				va_list argList;
				std::string szMessage;
				va_start(argList, i_szMassage);
				const size_t nLength  = _vscprintf( i_szMassage, argList ) + 1;
				
				szMessage.resize(nLength);

				vsprintf_s(const_cast<char*>(szMessage.c_str()), nLength + 1, i_szMassage, argList);
				
				va_end(argList);
				// *******

				switch(i_oLogSeverity)
				{
				case MGD_INFO:
					{
						LOG(INFO) << szMessage;
						break;
					}
				case MGD_WARNING:
					{
						LOG(WARNING) << szMessage;
						break;
					}
				case MGD_ERROR:
					{
						LOG(ERROR) << szMessage;
						break;
					}
				case MGD_FATAL:
					{
						LOG(FATAL) << szMessage;
						break;
					}
				default:
					{
						LOG(ERROR) << "Incorrect LogSeverity!";
						break;
					}
				}
			}
		}
	}
}


