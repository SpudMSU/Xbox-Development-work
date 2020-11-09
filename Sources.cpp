/*Here are links to posts I made in 2012 discussing topics in Xbox game modifications:

https://www.se7ensins.com/forums/threads/tu7-menu-tut-how-to-give-weapons-using-functions.813765/
- Tutorial on how to give yourself specific weapons in a game using their memory offsets)

https://www.se7ensins.com/forums/threads/program-tut-how-to-use-xepatcher-to-write-an-xex-to-a-file.815044/ 
- Tutorial on how to modify a xex (xbox's version of a windows exe or executable) to run a custom dll file on launch

https://www.se7ensins.com/forums/threads/c-tut-small-memory-editing.814628/#post-6024687
- Tutorial on how in game offsets relate to existing functions and arrays along with how to modify them.

Youtube channel which I uploaded videos of mods I made: https://www.youtube.com/user/CaPsychoModz

Here is the source code to a simple "mod menu" I made utilizing these practices*/

#include "stdafx.h"
#include <stdio.h>
#include <string>
using std::string;

//headers to in game functions that will later be mapped directly to addresses
typedef void (*SV_GameSendServerCommand_t)(int clientNum, int type, const char *text);
typedef int (*Key_IsDown)(int ClientNum, int ButtonEnum);
typedef bool (*Dvar_GetBool_t)(const char *dvarName);
typedef bool (*G_GivePlayerWeapon)(unsigned int pPS, int iWeaponIndex, char altModelIndex, int Akimbo);
typedef int (*G_GetWeaponIndexForName)(const char *name);
typedef void (*giveAmmo_t)(int clientEntity, unsigned int weaponIndex, char weaponModel, int count, int fillClip);
 
 
int MenuActive = 0;
int Scroll = 0;
//defining the xbox controller buttons
int RightBumper()
{
    //this function essentially acts as a listener for the byte 0x06 at the memory address for xbox keys which is 0x82141308
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x06))
    {
    return 1;
    }
    else
    return 0;
}
                     
int LeftBumper()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x05))
    {
    return 1;
    }
    else
    return 0;
}
 
int XSelect()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x03))
    {
    return 1;
    }
    else
    return 0;
}
 
int YSelect()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x04))
    {
    return 1;
    }
    else
    return 0;
}
 
int BSelect()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x02))
    {
    return 1;
    }
    else
    return 0;
}
 
int ASelect()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x01))
    {
    return 1;
    }
    else
    return 0;
}
 
int RightStick()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x11))
    {
        return 1;
        MenuActive = 1;
    }
    else
        return 0;
        MenuActive = 0;
}
 
int LeftStick()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x10))
    {
        return 1;
    }
    else
        return 0;
}
 
int StartButton()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x0E))
    {
        return 1;
    }
    else
        return 0;
}
 
int BackButton()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x0F))
    {
        return 1;
    }
    else
        return 0;
}
 
int LeftTrigger()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x12))
    {
        return 1;
    }
    else
        return 0;
}
 
int RightTrigger()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x13))
    {
        return 1;
    }
    else
        return 0;
}
 
int DPADUp()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x14))
    {
        return 1;
    }
    else
        return 0;
}
 
int DPADDown()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x15))
    {
        return 1;
    }
    else
        return 0;
}
 
int DPADLeft()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x16))
    {
        return 1;
    }
    else
        return 0;
}
 
int DPADRight()
{
    Key_IsDown Key_Down = (Key_IsDown)0x82141308;
    if (Key_Down(0,0x17))
    {
        return 1;
    }
    else
        return 0;
}

//end of defining functions for the keypad
 
DWORD WINAPI commands(LPVOID)
{
        //mapping the game functions to their addresses in the games engine (found using powerpc in ida pro)
        SV_GameSendServerCommand_t SV = (SV_GameSendServerCommand_t)0x822548C8;
        Key_IsDown button_Pressed = (Key_IsDown)0x82141308;
        Dvar_GetBool_t Dvar_GetBool = (Dvar_GetBool_t)0x8229EEE0;
        G_GivePlayerWeapon giveweap = (G_GivePlayerWeapon)0x82210B58;
        G_GetWeaponIndexForName GetWeaponIndex = (G_GetWeaponIndexForName)0x822105D0;
        giveAmmo_t giveAmmo = (giveAmmo_t)0x821E1F20;
 
        for(;;) //run forever to constantly listen for xbox controller keys
        {
        
        //values will be 1 if key is pressed
            int rb = RightBumper();
            int lb = LeftBumper();
            int xs = XSelect();
            int ys = YSelect();
            int bs = BSelect();
            int as = ASelect();
            int rs = RightStick();
            int ls = LeftStick();
            int sb = StartButton();
            int bb = BackButton();
            int lt = LeftTrigger();
            int rt = RightTrigger();
            int du = DPADUp();
            int dd = DPADDown();
            int dl = DPADLeft();
            int dr = DPADRight();
            
       
            if (Dvar_GetBool("cl_ingame")) //if in gam
            {
                if (rs == 1) //and rs button is pressed
                {
                    MenuActive = 1;
                    Scroll = 0;
                    //show this stuff
                    SV(-1,0, "f \"^5Gold Desert Eagle\"");
                    SV(-1,0, "f \"^5Ak47\"");
                    SV(-1,0, "f \"^5M16\"");
                    SV(-1,0, "f \"^5Scar\"");
                }
                if (MenuActive == 1)
                { //for showing the entire menu
                    if (rb == 1)
                    {
                        if (Scroll == 0)
                        {
                            SV(-1,0, "f \"^5Gold Desert Eagle\"");
                            SV(-1,0, "f \"^5Ak47\"");
                            SV(-1,0, "f \"^5M16\"");
                            SV(-1,0, "f \"^5Scar\"");
                        }
                        else if (Scroll == 1)
                        {
                            SV(-1,0, "f \"^1Gold Desert Eagle\"");
                            SV(-1,0, "f \"^5Ak47\"");
                            SV(-1,0, "f \"^5M16\"");
                            SV(-1,0, "f \"^5Scar\"");
                            Scroll++;
                        }
                        else if (Scroll == 2)
                        {
                            SV(-1,0, "f \"^5Gold Desert Eagle\"");
                            SV(-1,0, "f \"^1Ak47\"");
                            SV(-1,0, "f \"^5M16\"");
                            SV(-1,0, "f \"^5Scar\"");
                            Scroll++;
                        }
                        else if (Scroll == 3)
                        {
                            SV(-1,0, "f \"^5Gold Desert Eagle\"");
                            SV(-1,0, "f \"^5Ak47\"");
                            SV(-1,0, "f \"^1M16\"");
                            SV(-1,0, "f \"^5Scar\"");
                            Scroll++;
                        }
                        else if (Scroll == 4)
                        {
                            SV(-1,0, "f \"^5Gold Desert Eagle\"");
                            SV(-1,0, "f \"^5Ak47\"");
                            SV(-1,0, "f \"^5M16\"");
                            SV(-1,0, "f \"^1Scar\"");
                            Scroll=1;
                        }
                    }
                    if (xs == 1)
                    { // mapping the menu items to their actual executions
                        if (Scroll == 1)
                        {
                            giveweap(2198650752 , GetWeaponIndex("deserteaglegold_mp") , 0, 0);
                            giveweap(2198664832 , GetWeaponIndex("deserteaglegold_mp"), 0, 0);
                            giveAmmo(2196780544 , GetWeaponIndex("deserteaglegold_mp"), 1, 1, 999);
                            SV(-1,0, "c \"^2Gold Desert Eagle Given\"");
                        }
                        if (Scroll == 2)
                        {
                            giveweap(2198650752 , GetWeaponIndex("ak47_mp"), 0, 0);
                            giveweap(2198664832 , GetWeaponIndex("ak47_mp"), 0, 0);
                            giveAmmo(2196780544 , GetWeaponIndex("ak47_mp"), 1, 1, 999);
                            SV(-1,0, "c^2 Ak47 Given \"\"");
                        }
                        if (Scroll == 3)
                        {
                            giveweap(2198650752 , GetWeaponIndex("m16_mp"), 1, 1);
                            giveweap(2198664832 , GetWeaponIndex("m16_mp"), 1, 1);
                            giveAmmo(2196780544 , GetWeaponIndex("m16_mp"), 1, 1, 999);
                            SV(-1,0, "c^2M16 Akimbo Given\"\"");
                        }
                        if (Scroll == 4)
                        {
                            giveweap(2198650752 , GetWeaponIndex("scar_mp"), 1, 1);
                            giveweap(2198664832 , GetWeaponIndex("scar_mp"), 1, 1);
                            giveAmmo(2196780544 , GetWeaponIndex("scar_mp"), 1, 1, 999);
                            SV(-1,0, "c \"^2Scar Akimbo Given\"");
                        }
                    }
                }
            }//sleep statements are too prevent lag
            Sleep(45);
        }
        Sleep(45);
}
