#include "C:\CodeProjects\dapi.h"
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>

// Origial Version Copywrite OSTLabs Inc.
// Author requested to be nameless - But I know I couldn't have done this part without him.


// Global Variables

// cAssignedDriveSuccessful is a control variable used to check if a drive was assigned successfully
// z = nothing assigned
// q = quit program - drive assigned and found successfully
char cAssignedDriveSuccessful='z';

// A sample callback function implementation.
// If any DAPI functions must be called, they should be done outside
// of the callback function.

void _stdcall EventCallback(HDEVICE hDev, DWORD eventType, void* pEx)
   {
               // log device insert and eject events

    domainInfo DriveInfo[3];
    HRESULT res;
    CHAR DriveLetter[2];
    WORD wDomains=0;

//    BOOL bU3 = (S_OK == dapiQueryDeviceCapability(hDev, DAPI_CAP_U3));

    int iNumDomains = 0;

    switch(eventType)
         {
         case DAPI_EVENT_DEVICE_UPDATE:
                iNumDomains = 0;
//                printf("Device updated\n");

//                printf("Device is U3 compatible? = %2d \n", bU3);

                res = (dapiQueryDomainInformation(hDev, NULL, &wDomains));
//                printf("Number of domains is %i \n", wDomains);

                if (SUCCEEDED(res))
                {
                    if(dapiQueryDomainInformation(hDev, &(DriveInfo[0]), &wDomains) == S_OK)
                    {
//                        printf("Drives available are ");
                        while (iNumDomains < wDomains)
                        {
                            if (DriveInfo[iNumDomains].typeMask == DAPI_PI_REMOVABLE_PUBLIC)
                            {
                                memcpy(DriveLetter,DriveInfo[iNumDomains].szPath,2);
                                printf("%s", DriveLetter);
                            }
                            iNumDomains++;
                        }
                        printf("\n");
                    }
               }
               cAssignedDriveSuccessful = 'q';
               break;

         case DAPI_EVENT_DEVICE_CONNECT:

                iNumDomains = 0;

//                printf("Device %2d was connected\n", hDev);

                res = (dapiQueryDomainInformation(hDev, NULL, &wDomains));

                if (SUCCEEDED(res))
                {
                    if(dapiQueryDomainInformation(hDev, &(DriveInfo[0]), &wDomains) == S_OK)
                    {
//                        printf("Drives available are ");
                        while (iNumDomains < wDomains)
                        {
                            if (DriveInfo[iNumDomains].typeMask == DAPI_PI_REMOVABLE_PUBLIC)
                            {
                                memcpy(DriveLetter,DriveInfo[iNumDomains].szPath,2);
                                printf("%s", DriveLetter);
                                cAssignedDriveSuccessful = 'q';
                            }
                            iNumDomains++;
                        }
                        printf("\n");
                    }
               }
               break;

         case DAPI_EVENT_DEVICE_DISCONNECT:
//               printf("Device %2d was disconnected\n", hDev);
               break;
         }
   }

   // Basic function to create a session and register a callback.
HSESSION startDAPI(void)
   {
   // create the session
   HSESSION hSession = ILLEGAL_HSESSION;

   if(S_OK == dapiCreateSession(&hSession))
         {

         // register the callback for all devices
         HDEVICE hCallback = ILLEGAL_HCALLBACK;
         DAPI_CALLBACK pCallback = (DAPI_CALLBACK)&EventCallback;

         if(S_OK != dapiRegisterCallback(
hSession,  // handle to the session
                           NULL,      // all devices
                           pCallback, // pointer to EventCallback(...)
                           NULL,      // no pEx data,
                           &hCallback))
               {          // callback registration failed, let us destroy
                     // the session now.
               dapiDestroySession(hSession);
               hSession = ILLEGAL_HSESSION;

               printf("Callback NOT successful\n");

               }
         }

   return hSession;
   }

// this will destroy the session and all associated callback events
// handlers and device handles.

void stopDAPI(HSESSION hSession)
   {
   dapiDestroySession(hSession);
   }


// main entry point to application.
// Create the session and maintain the application waiting for key input
// before destroying the session and quiting.

//int main(int argc, _TCHAR* argv[])
int main(void)
   {


   HSESSION hSession = startDAPI();

// Keep receiving events until cAssignedDriveSuccessful = 'q'.
// Assignment is done in the void _stdcall EventCallback(HDEVICE hDev, DWORD eventType, void* pEx)
// function. Assignment is done if a drive with a writeable partition is found.
// Otherwise, loop continues.

    while(cAssignedDriveSuccessful != 'q')
    {
        //Sleep is used so that the while loop does not take up 100% of the CPU cycles.
        Sleep(100);
    }

   stopDAPI(hSession);
   return 0; }
