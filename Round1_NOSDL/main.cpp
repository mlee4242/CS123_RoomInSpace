//========= Copyright Valve Corporation ============//

#include "CMainApplication.h"


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    CMainApplication pMainApplication( argc, argv );

    if (!pMainApplication.BInit())
    {
        pMainApplication.Shutdown();
        return 1;
    }

    pMainApplication.RunMainLoop();

    pMainApplication.Shutdown();

    return 0;
}
