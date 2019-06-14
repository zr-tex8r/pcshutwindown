/*
 * pcshutwindown.c : (Not so) Lua module 'pcshutwindown'
 */
#include <stdio.h>
#include <windows.h>
/* No Lua things :-) */

/*
 * Initiates system shutdown.
 * Returns a bool showing whether the job succeeded.
 */
static BOOL shutdown_system (void) {
    HANDLE TokenHandle;
    TOKEN_PRIVILEGES Privileges;

    if (!OpenProcessToken(GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
            &TokenHandle))
        return FALSE;
    Privileges.PrivilegeCount = 1;
    Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
            &Privileges.Privileges[0].Luid);
    AdjustTokenPrivileges(TokenHandle, FALSE, &Privileges,
            0, NULL, NULL);
    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    return ExitWindowsEx(EWX_SHUTDOWN, 0);
}

/*
 * The entry point.
 * Here shutdown will be initiated.
 */
__declspec(dllexport) int luaopen_pcshutwindown(void *L) {
    int c;
    for (c = 10; c >= 1; c--) {
        printf("System will be shutdown in %d second%s...\n", c,
            (c == 1) ? "" : "s");
        Sleep(1000);
    }

    if (shutdown_system()) { 
        printf("FAREWELL!\n");
    } else {
        printf("ERROR: cannot initiate shutdown\n"); 
    }

    return 0;
}
