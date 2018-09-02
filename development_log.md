PROBLEM
    problem-01:
        #include <Windows.h> 包含了 WinSock2.h 和 WinSock.h 开发时需要把 WinSock2.h 放在 Windows.h前面
    analyse-01:
        主要原因是因为<Windows.h>中包含了<Winsock.h>头文件,由于其版本的不同，导致出现上述的错误。<Windows.h>中相关代码如下: 
            ifndef WIN32_LEAN_AND_MEAN
                #include <cderr.h>
                #include <dde.h>
                #include <ddeml.h>
                #include <dlgs.h>
                #ifndef _MAC
                #include <lzexpand.h>
                #include <mmsystem.h>
                #include <nb30.h>
                #include <rpc.h>
                #endif
                #include <shellapi.h>
                #ifndef _MAC
                #include <winperf.h> 
                #if(_WIN32_WINNT >= 0x0400)
                #include <winsock2.h>
                #include <mswsock.h>
                #else
                #include <winsock.h>
                #endif /* _WIN32_WINNT >=  0x0400 */
                #endif
        winsocket2.h相关代码
            #ifndef _WINSOCK2API_
                #define _WINSOCK2API_
                #define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */ //为了避免后面包含winsock.h
        
        如果先包含windows.h 在包含winsock2.h 这样就会出现 windows.h包含了winsock.h 然后在包含winsock2.h就出现了重定义
    reslove-01:
        第一种.解决方案预处理 定义   WIN32_LEAN_AND_MEAN
        第二种 winsock2.h 定义在 windows.h前面

    problem-02:
        
