//-----------------------------------------------------------------------------
//  PerfWin.exe
//  This is the windows side of getting the perf meter and saving information
//  to a log file.  the location is specific to a cygwin release, so the 
//  output can easily be read.
//
//  Credit for original code goes to sayyed moshsen zahraee from a web 
//  site:
//  http://w3facility.org/question/
//  how-to-determine-cpu-and-memory-consumption-from-inside-a-process/
//  I have changed the code and added some things to make it robust.
//  If there is no other way to access the windows kernel through
//  cygwin, and this becomes popular, plans will be to move this 
//  to a service so that when machine starts up, the performance
//  is written to the location on machine start up.
//
//  This will windows program will write to an area that cygwin
//  has a file syste.  
//  Open Dos Window and start PerfWin.exe.
//  Will write the cpu % usage to file.  Cygwin can then read the
//  file to get cpu perforance informat.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <process.h>
#include <errno.h>

#define ERROR_INFO() printf("abort | errno = %d | file : %s | function : %s \
                     | line : %d\n", err, __FILE__ ,  __FUNCTION__ ,  __LINE__)

#define FILE_PATH()  "C:\\cygwin\\var\\tmp"
#define FILE_NAME()   "loadcygwinavg"

using namespace std; 
    
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
int cpuusage(void);

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef BOOL ( __stdcall * pfnGetSystemTimes)( LPFILETIME lpIdleTime, 
	                       LPFILETIME lpKernelTime, LPFILETIME lpUserTime );

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
static pfnGetSystemTimes s_pfnGetSystemTimes = NULL;
static HMODULE s_hKernel = NULL;
FILE *stream;

//-----------------------------------------------------------------------------
// Function GetSystemTimesAddress()
// params: none
// return: none
// 
// Use Windows dll to get kernel times for CPU process
//-----------------------------------------------------------------------------
void GetSystemTimesAddress()
{
  if( s_hKernel == NULL )
  {   
     s_hKernel = LoadLibrary( L"Kernel32.dll" );
     if( s_hKernel != NULL )
      {
         s_pfnGetSystemTimes = (pfnGetSystemTimes)GetProcAddress( s_hKernel, 
			                                                "GetSystemTimes" );
         if( s_pfnGetSystemTimes == NULL )
         {
           FreeLibrary( s_hKernel ); 
		   s_hKernel = NULL;
         }
      }
  }
}

//-----------------------------------------------------------------------------
// Function: cpuusage()
// params : none
// return : value in the range 0 - 100 representing actual CPU usage in 
// percent.
//-----------------------------------------------------------------------------
int cpuusage()
{
    FILETIME               ft_sys_idle;
    FILETIME               ft_sys_kernel;
    FILETIME               ft_sys_user;

    ULARGE_INTEGER         ul_sys_idle;
    ULARGE_INTEGER         ul_sys_kernel;
    ULARGE_INTEGER         ul_sys_user;

    static ULARGE_INTEGER  ul_sys_idle_old;
    static ULARGE_INTEGER  ul_sys_kernel_old;
    static ULARGE_INTEGER  ul_sys_user_old;

    int usage = 0;

	 //System idel, kernel, and user times
     s_pfnGetSystemTimes(&ft_sys_idle, &ft_sys_kernel, &ft_sys_user); 

     CopyMemory(&ul_sys_idle  , &ft_sys_idle  , sizeof(FILETIME)); 
     CopyMemory(&ul_sys_kernel, &ft_sys_kernel, sizeof(FILETIME)); 
     CopyMemory(&ul_sys_user  , &ft_sys_user  , sizeof(FILETIME));

     usage = static_cast<int>
             ( ( ( ( ( ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart ) +
             ( ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart ) ) -
             ( ul_sys_idle.QuadPart-ul_sys_idle_old.QuadPart ) ) * (100) ) /
             ( ( ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart ) +
             ( ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart ) ) );

     ul_sys_idle_old.QuadPart   = ul_sys_idle.QuadPart;
     ul_sys_user_old.QuadPart   = ul_sys_user.QuadPart;
     ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

    return usage;
}

//-----------------------------------------------------------------------------
// Perormance Montior goes from 0 to 60 sec
// May want to rename CygPerfMon
//----------------------------------------------------------------------------_

int _tmain(int argc, _TCHAR* argv[])
{
		errno_t err;

		SetCurrentDirectoryA( FILE_PATH() );

        GetSystemTimesAddress();

		for(;;)
        {	
			if (   ( fopen_s(&stream, FILE_NAME(), "w") != 0 ) || stream == NULL )
			{
 				_get_errno( &err );
				ERROR_INFO();
				abort();
			 }										
		
			if (  fprintf(stream, "%3d%%\r",  cpuusage() ) <= -1 )
			{
				_get_errno( &err );
				ERROR_INFO();
				abort();
			}

			if ( fclose(stream) == EOF )
		    {
				_get_errno( &err );
				ERROR_INFO();
				abort();
			}

			//1 second samples
            Sleep(1000);
		    //system("type loadwinavg");
        }
       
	return EXIT_SUCCESS; 
}

