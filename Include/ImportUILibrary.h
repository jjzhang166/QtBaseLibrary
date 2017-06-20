#pragma once

#include "UILibrary/AppGlobal.h"
#include "UILibrary/AppHelper.h"
#include "UILibrary/AppStruct.h"
#include "UILibrary/TitleBar.h"
#include "UILibrary/ToolButton.h"
#include "UILibrary/PushButton.h"
#include "UILibrary/AppFunctions.h"

#ifndef UILIBRARY_LIB
#if defined QT_NO_DEBUG
#pragma comment(lib, "UILibrary")
#else
#pragma comment(lib, "UILibraryd")
# endif
#endif

