// 2023/08/11 17:01:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Config
{
    // Устанавливает файл с настройками.
    // Если "", будет установлен глобальный файл
    void SetFile(const wxString &file_path);

    int ReadInt(pchar key, int def = 0);

    uint64 ReadUInt64(pchar key, uint64 def = 0);

    int64 ReadInt64(pchar key, int64 def = 0);

    bool ReadBool(pchar key, bool def = false);

    wxString ReadString(pchar key, const wxString &def = "");

    void WriteUint(pchar key, uint);

    void WriteInt(pchar key, int);

    void WriteUInt64(pchar key, uint64);

    void WriteInt64(pchar kye, int64);

    void WriteBool(pchar key, bool);

    void WriteString(pchar key, const wxString &);
}
