
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

		// IPluginLog
		virtual void CPluginBase::LogAlways(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
			OutputDebugStringA(buf);
			printf("Notice: %s\n", buf);

		} virtual void CPluginBase::LogWarning(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
			printf("Warning: %s\n", buf);
		} virtual void CPluginBase::LogError(const char *sFormat, ...) const {
			char buf[2048] = { 0 };

			va_list ArgList;
			va_start(ArgList, sFormat);

			vsprintf_s(buf, sFormat, ArgList);

			// LogV(ILog::eAlways, sFormat, ArgList);
			va_end(ArgList);
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
