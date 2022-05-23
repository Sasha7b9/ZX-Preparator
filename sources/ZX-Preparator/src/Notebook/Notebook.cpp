// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);
    pageBinary = new PageHEX(this);
    pageParsed = new PageBASIC(this);
    pageEdit = new PageASM(this);

    AddPage(pageInfo, pageInfo->GetName());
    AddPage(pageBinary, pageBinary->GetName());
    AddPage(pageParsed, pageParsed->GetName());
    AddPage(pageEdit, pageEdit->GetName());
}


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    type(_type)
{
    SetBackgroundColour(*wxWHITE);
    SetScrollbars(20, 20, 50, 50);

    if (type == TypePage::Info || type == TypePage::BASIC)
    {
        SetScrollbars(20, 20, 10, 10);
    }

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    SetName(name);
}


void Page::WriteText(wxDC &dc, const wxString &text, int &x, int &y, bool fill)
{
    wxSize size = dc.GetTextExtent(text);

    if (x + size.x > GetSize().x)
    {
        x = 0;
        y += font.GetPixelSize().y + 4;
    }

    if (fill)
    {
        static wxColour colour(240, 240, 240);
        static wxBrush brush(colour, wxBRUSHSTYLE_SOLID);
        static wxPen pen(colour);

        dc.SetBrush(brush);
        dc.SetPen(pen);
        dc.DrawRectangle(x, y, size.x, size.y);
    }

    dc.DrawText(text, x, y);
    x += size.x;
}


void PageBASIC::SetProgram(ProgramBASIC &program)
{
    for (LineBASIC line : program.lines)
    {
        lines.push_back(line);
    }

    Refresh();
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}


void PageInfo::SetDescriptionTAP(DescriptionTAP& desc)
{
    descTAP = desc;

    Refresh();
}


void PageInfo::OnDraw(wxDC &dc)
{
    dc.SetFont(font);

    if (descTAP.valid)
    {
        int x = 0;
        int y = 0;
        int dY = font.GetPointSize() + 10;

        dc.DrawText("Is TAP file", x, y);

        y += dY;

        dc.DrawText(wxString::Format("Number blocks : %d", descTAP.numberBlocks), x, y);

        y += dY;

        for (uint i = 0; i < (uint)descTAP.numberBlocks; i++)
        {
            dc.DrawText(wxString::Format("Block %d:", i + 1), x + 20, y);

            y += dY;

            static const pchar types[4] =
            {
                "BASIC PROGRAM",
                "NUMBER ARRAY",
                "CHARACTER ARRAY",
                "MACHINE CODE"
            };

            dc.DrawText(wxString::Format("Type : %s", types[descTAP.blocks[i].type]), x + 40, y);

            y += dY;

            dc.DrawText(wxString::Format("Size : %d bytes", descTAP.blocks[i].size), x + 40, y);

            y += dY;
        }
    }
    else
    {
        dc.DrawText("Is not TAP file", 10, 10);
    }
}


void PageBASIC::OnDraw(wxDC &dc)
{
    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    dc.SetFont(font);

    wxSize size = GetSize();

    int x = 0;
    int y = 0;

    for (LineBASIC line : lines)
    {
        bool fill = false;

        WriteText(dc, wxString::Format("%d %d ", line.number, line.size), x, y, fill);

        for (SymbolBASIC symbol : line.symbols)
        {
            fill = !fill;

            WriteText(dc, symbol.string, x, y, fill);
        }

        y += font.GetPixelSize().y + 4;
        x = 0;
    }

    dc.DrawText(wxString::Format("%d : %d", GetSize().x, GetSize().y), x, y);
}
