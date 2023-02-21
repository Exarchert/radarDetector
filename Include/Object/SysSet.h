#pragma once
#include <Object\Export.h>
#include <COM\kernelcontext\KernelContext.h>

namespace EMObj
{
	OBJECT_EXPORT void SysSet(COM::CKernelContext &kContext);

	OBJECT_EXPORT void SysUnSet();
};