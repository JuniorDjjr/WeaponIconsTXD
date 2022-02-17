#include "plugin.h"
#include "CTxdStore.h"
#include "CHud.h"
#include "CKeyGen.h"

using namespace plugin;

class WeaponIconsTXD
{
public:

    inline static RwTexDictionary* weaponIconsDict;

    WeaponIconsTXD()
    {
        Events::initScriptsEvent += []
        {
            int txdSlot = CTxdStore::AddTxdSlot("weaponicons");
            CTxdStore::LoadTxd(txdSlot, "weaponicons.txd");
            CTxdStore::AddRef(txdSlot);
            CTxdStore::AddRef(txdSlot);
            CTxdStore::SetCurrentTxd(txdSlot);
            weaponIconsDict = plugin::CallAndReturn<RwTexDictionary*, 0x408340, int>(txdSlot);

            if (weaponIconsDict)
            {
                // Check fist icon exists
                if (RwTexDictionaryFindHashNamedTexture(weaponIconsDict, CKeyGen::GetUppercaseKey("fist")))
                {
                    // Instead of m_pTexture, call SetTexture (safer)
                    CHud::Sprites[0].SetTexture((char*)"fist");
                }

                // Patch weapon icons call
                patch::RedirectCall(0x58D84B, CustomRwTexDictionaryFindHashNamedTexture);
                patch::RedirectCall(0x58E895, CustomRwTexDictionaryFindHashNamedTexture);
            }

            CTxdStore::PopCurrentTxd();
        };
    }

    static RwTexture* __cdecl CustomRwTexDictionaryFindHashNamedTexture(RwTexDictionary* a1, int hash)
    {
        RwTexture* retTexture = nullptr;
        retTexture = RwTexDictionaryFindHashNamedTexture(weaponIconsDict, hash);
        if (!retTexture)
        {
            retTexture = RwTexDictionaryFindHashNamedTexture(a1, hash);
        }
        return retTexture;
    }

} weaponIconsTXD;
