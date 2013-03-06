========================================================================
    ACTIVEX 控件 DLL : KeWebCamOCX 项目概述
========================================================================

控件向导已为 KeWebCamOCX ActiveX 控件 DLL 创建此项目，其中包含一个控件。

此主干项目不仅演示了编写 ActiveX 控件的基本方法，而且还可以作为编写控件特定功能的起点。

本文件概要介绍组成 KeWebCamOCX ActiveX 控件 DLL 的每个文件的内容。

KeWebCamOCX.vcproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

KeWebCamOCX.h
    这是 ActiveX 控件 DLL 的主包含文件，其中包括其他
    项目特定的文件，如 resource.h。

KeWebCamOCX.cpp
    这是包含 DLL 初始化、终止和其他簿记的代码的主源文件。

KeWebCamOCX.rc
    这是项目使用的 Microsoft Windows 资源的清单。
    此文件可以直接用 Visual C++ 资源编辑器进行编辑。

KeWebCamOCX.def
    此文件包含关于在 Microsoft Windows 上运行所必需的 ActiveX 控件 DLL 的有关信息。

KeWebCamOCX.idl
    此文件包含控件类型库的对象描述语言源代码。

/////////////////////////////////////////////////////////////////////////////
CKeWebCamOCXCtrl 控件：

KeWebCamOCXCtrl.h
    此文件包含 CKeWebCamOCXCtrl C++ 类的声明。

KeWebCamOCXCtrl.cpp
    此文件包含 CKeWebCamOCXCtrl C++ 类的实现。

KeWebCamOCXPropPage.h
    此文件包含 CKeWebCamOCXPropPage C++ 类的声明。

KeWebCamOCXPropPage.cpp
    此文件包含 CKeWebCamOCXPropPage C++ 类的实现。

CKeWebCamOCXCtrl.bmp
    此文件包含一个位图，当 CKeWebCamOCXCtrl 控件出现在工具面板上时，容器将使用此位图来表示该控件。此位图包含在主资源文件 KeWebCamOCX.rc 中。

/////////////////////////////////////////////////////////////////////////////
授权支持：

KeWebCamOCX.lic
    这是用户许可证文件。此文件必须与控件 DLL 出现在相同目录中，这样才能允许在设计时环境中创建该控件的实例。通常您会将此文件与控件一同分发，但您的客户不会将其分发。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

stdafx.h, stdafx.cpp
    这些文件用于生成名为 KeWebCamOCX.pch 的预编译头 (PCH) 文件和名为 stdafx.obj 的预编译类型 (PCT) 文件。

resource.h
    这是标准头文件，可用于定义新的资源 ID。
    Visual C++ 资源编辑器将读取并更新此文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

控件向导使用“TODO:”来指示
应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////