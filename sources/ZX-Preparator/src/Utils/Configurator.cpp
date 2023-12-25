// 2023/08/11 17:01:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Configurator.h"


void Config::SetFile(const wxString &file_path)
{
    static wxFileConfig *config = nullptr;

    if (config)
    {
        delete config;
        config = nullptr;
    }

    if (file_path.IsEmpty())
    {
        wxConfigBase::Set(g_file_config);
        g_config = wxConfigBase::Get(false);
    }
    else
    {
        config = new wxFileConfig("", "", file_path);

        wxConfigBase::Set(config);
        g_config = wxConfigBase::Get(false);
    }
}


int Config::ReadInt(pchar key, int def)
{
    int result = def;

    g_config->Read(key, &result, def);

    return result;
}


uint64 Config::ReadUInt64(pchar key, uint64 def)
{
    uint64 result = def;

    g_config->Read(key, (int64 *)&result, (int64)def);

    return result;
}


int64 Config::ReadInt64(pchar key, int64 def)
{
    int64 result = def;

    g_config->Read(key, &result, def);

    return result;
}


bool Config::ReadBool(pchar key, bool def)
{
    bool result = def;

    g_config->Read(key, &result, def);

    return result;
}


wxString Config::ReadString(pchar key, const wxString &def)
{
    wxString result = def;

    g_config->Read(key, &result, def);

    return result;
}


void Config::WriteInt(pchar key, int value)
{
    g_config->Write(key, value);
}


void Config::WriteUint(pchar key, uint value)
{
    g_config->Write(key, value);
}


void Config::WriteUInt64(pchar key, uint64 value)
{
    g_config->Write(key, (int64)value);
}


void Config::WriteInt64(pchar key, int64 value)
{
    g_config->Write(key, value);
}


void Config::WriteBool(pchar key, bool value)
{
    g_config->Write(key, value);
}


void Config::WriteString(pchar key, const wxString &value)
{
    g_config->Write(key, value);
}
