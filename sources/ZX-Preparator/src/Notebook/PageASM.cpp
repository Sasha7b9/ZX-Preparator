// 2022/05/23 09:51:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageASM.h"


void PageASM::OnDraw(wxDC &)
{

}


void PageASM::SetProgram(ProgramASM &_program)
{
    program = _program;
}
