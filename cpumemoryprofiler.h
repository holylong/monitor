#ifndef CPUMEMORYPROFILER_H
#define CPUMEMORYPROFILER_H

#include <QObject>
#include <Windows.h>
#include <QDebug>
#include <QSysInfo>
//#include <QSystemCPUUsage>
//#include <QSystemMemoryInfo>

class CpuMemoryProfiler : public QObject
{
    Q_OBJECT
public:
    explicit CpuMemoryProfiler(QObject *parent = nullptr);


    static void InitCpu(){

    }


    static __int64 MsFiletime2Int64(const FILETIME &ftime)
    {
      LARGE_INTEGER li;
      li.LowPart = ftime.dwLowDateTime;
      li.HighPart = ftime.dwHighDateTime;
      return li.QuadPart;
    }

    static __int64 CompareFileTime2(const FILETIME &preTime, const FILETIME &nowTime)
    {
      return MsFiletime2Int64(nowTime) - MsFiletime2Int64(preTime);
    }

    static double GetCpuUsage(FILETIME preKernelTime,
                              FILETIME preUserTime,
                              FILETIME preIdleTime)
    {
        FILETIME idleTime;
        FILETIME kernelTime;
        FILETIME userTime;
        GetSystemTimes (&idleTime, &kernelTime, &userTime);

        auto idle = CompareFileTime2 (preIdleTime, idleTime);
        auto kernel = CompareFileTime2 (preKernelTime, kernelTime);
        auto user = CompareFileTime2 (preUserTime, userTime);

        if (kernel + user == 0)
          return 0;

        return 100.0* (kernel + user - idle) / (kernel + user);
    }

    static double GetMemoryUsage()
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

//        qDebug() << "Total physical memory: " << memInfo.ullTotalPhys;
//        qDebug() << "Available physical memory: " << memInfo.ullAvailPhys;
//        qDebug() << "Total virtual memory: " << memInfo.ullTotalVirtual;;
//        qDebug() << "Available virtual memory: " << memInfo.ullAvailVirtual;

        double ret = 100*((double)memInfo.ullAvailPhys/memInfo.ullTotalPhys);

//        qDebug() << "memory rate:" << ret;
        return ret;
    }

signals:

};

#endif // CPUMEMORYPROFILER_H
