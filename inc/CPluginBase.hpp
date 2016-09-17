
/* Plugin SDK - for licensing and copyright see license.txt */

#pragma once

#include <IPluginBase.h>
#include <IPluginManager.h>

#if defined(PLUGINMANAGER_EXPORTS)
#include <CPluginManager.h>
#endif
#include <include/capi/cef_base_capi.h>
#include <Nodes/G2FlowBaseNode.h>
#include <CPluginBaseMinimal.hpp>
#include <PMUtils.hpp>
#include <FLog.h>

namespace PluginManager {

	/**
	 * @brief Plugin Base Class
	 */
	class   CPluginBase :public CPluginBaseMinimal, public IPluginLog {
	public:
		CPluginBase() {
			CPluginBaseMinimal();
		};

		virtual ~CPluginBase() {
			Release(true);
		};

		// IPluginBase
		virtual bool Release(bool bForce = false) {
			bool    bRet = true;
			bool    bWasInitialized = m_bIsFullyInitialized;	// Will be reset by base class so backup

			if (!m_bCanUnload) {
				bRet = CPluginBaseMinimal::Release(bForce);

				if (bRet) {
					if (bWasInitialized) {
						// Unregister all types
						RegisterTypes(FT_All, true);
					}
				}
			}

			return bRet;
		};

		virtual bool InitDependencies() {
			return CPluginBaseMinimal::InitDependencies();
		}

		virtual bool RegisterTypes(int nFactoryType, bool bUnregister) {
			return true;	// or maybe false -- sfinktah
		};

		virtual const char *ListNodes() const {
			return NULL;
		};

		// https://msdn.microsoft.com/en-us/library/28d5ce15.aspx
		virtual void test_of_dynamically_allocated_memory_vsprintf( char * format, ... )
		{
			va_list args;
			int     len;
			char    *buffer;

			// retrieve the variable arguments
			va_start( args, format );

			// We are going to add some line feeds here.
			len = _vscprintf(format, args) // _vscprintf doesn't count
				+ 1 + 2;                   // terminating '\0'

			// If you're using C99, you can also just use a variable-length
			// array, so instead of mallocing str, just declare it as "char
			// str[length+1];" after the call to snprintf. No need to
			// deallocate it then, as it's on the stack.
			// -- http://stackoverflow.com/questions/295013/using-sprintf-without-a-manually-allocated-buffer
			buffer = (char*)malloc( len * sizeof(char) );

			vsprintf( buffer, format, args ); // C4996
											  // Note: vsprintf is deprecated; consider using vsprintf_s instead
			strcat(buffer, "\n");
			puts( buffer );
			free( buffer );
		}

		// IPluginLog
		virtual void CPluginBase::LogAlways(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
			strcat_s(buf, "\n");
			OutputDebugStringA(buf);
			printf("Notice: %s\n", buf);

		} virtual void CPluginBase::LogWarning(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
			strcat_s(buf, "\n");
			OutputDebugStringA(buf);
			printf("Warning: %s\n", buf);
		} virtual void CPluginBase::LogError(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
			strcat_s(buf, "\n");
			OutputDebugStringA(buf);
			printf("Error: %s\n", buf);
		}

		virtual void CPluginBase::LogV(FLog::ELogType nType, const char* sFormat, va_list ArgList) const
		{
			if (!sFormat)
			{
				return;
			}

			std::string strFormat = "[";
			strFormat += GetName();
			strFormat += "_";
			strFormat += PLUGIN_TEXT;
			strFormat += "] ";
			strFormat += sFormat;

			FLog flog;
			flog.LogV(nType, strFormat.c_str(), ArgList);
			//if (gEnv && !IsBadReadPtr(gEnv, sizeof(void*)) && gEnv->pSystem && !IsBadReadPtr(gEnv->pSystem, sizeof(void*)) && gEnv->pLog)
			//{
				//gEnv->pLog->LogV(nType, strFormat.c_str(), ArgList);
			//}
		};
	};
}
