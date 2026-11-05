// 2022/05/23 16:56:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include "Computer/RAM.h"
#pragma warning(push, 0)
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#pragma warning(pop)


using std::ofstream;
using std::string;
using std::vector;
using std::endl;


int StorageInstructions::NextAddress()
{
    if (addressesForScan.size() > 0)
    {
        return addressesForScan[0];
    }

    return -1;
}


void StorageInstructions::AddNewData(bool succsefull, int address, InfoStruct *params)
{
    RemoveScanAddress(address);

    Instruction instruction;

    instruction.address = address;

    for (int i = 0; i < params->numOpCodes; i++)
    {
        instruction.opCodes.push_back(params->GetOpcode(i));
    }

    if (succsefull)
    {
        instruction.numTackts = params->tackts;

        instruction.mnemonic = params->GetMnemonic();

        for (int i = 0; i < params->numAddresses; i++)
        {
            AppendEntryPoint((int)params->GetAddess(i));
        }

        instruction.transcript = string(params->transcript);

        instruction.flags = string(params->flags);

        instruction.comment = string(params->comment);

        instruction.tackts = params->tackts;
    }
    else
    {
        instruction.bad = true;
    }

    instructions.push_back(instruction);
}


void StorageInstructions::RemoveScanAddress(int address)
{
    for (int i = 0; i < (int)addressesForScan.size(); i++)
    {
        if (addressesForScan[(uint)i] == address)
        {
            addressesForScan.erase(addressesForScan.begin() + i);
            return;
        }
    }
}


void StorageInstructions::AppendEntryPoint(int address)
{
    if (address >= 0 && address < addr_max && !AddressAlreadyScanOrFuture(address))
    {
        addressesForScan.push_back(address);
    }
}


void StorageInstructions::AppendDefBytes(int address, int num_bytes)
{
    Instruction instr;
    instr.is_bytes = true;
    instr.bad = false;
    instr.address = address;

    for (int i = 0; i < num_bytes; i++)
    {
        uint8 code = RAM8(address++);
        instr.opCodes.push_back(code);
    }

    instructions.push_back(instr);
}


bool StorageInstructions::AddressAlreadyScanOrFuture(int address)
{
    for each(auto &instruction in instructions)
    {
        if (instruction.address == address)
        {
            return true;
        }
    }

    for each(auto addr in addressesForScan)
    {
        if (addr == address)
        {
            return true;
        }
    }

    return false;
}


void StorageInstructions::WriteBinaryByte(std::ofstream &file, int value)
{
    vector<int> bits;

    for (int i = 0; i < 8; i++)
    {
        bits.push_back(value % 2);
        value /= 2;
    }

    file << std::dec;

    file << bits[7];
    file << bits[6];
    file << " ";
    file << bits[5];
    file << bits[4];
    file << bits[3];
    file << " ";
    file << bits[2];
    file << bits[1];
    file << bits[0];
}


void StorageInstructions::WriteCommand(ofstream &file, Instruction &instruction)
{
    file << std::right << std::hex << std::setw(4) << std::setfill('0') << instruction.address << " | ";

    file << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)instruction.opCodes[0] << " | ";
    WriteBinaryByte(file, (int)instruction.opCodes[0]);
    file << " | ";

    if (instruction.bad)
    {
        file << "ERROR!!! BAD COMMAND!!! ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    else
    {
        file << std::setw(14) << std::left << std::setfill(' ') << instruction.mnemonic;
        /*
        if(!command.transcript.empty())
        {
            file << std::setw(20) << command.transcript << " | ";
        }
        else
        {
            file << "                     | ";
        }
        if(!command.flags.empty())
        {
            file << command.flags << " | ";
        }
        else
        {
            file << "         | ";
        }
        file << std::dec << std::setw(2) << command.tackts << " | ";
        */
        if (!instruction.comment.empty())
        {
            file << " | " << instruction.comment;
        }
    }

    for (uint i = 1; i < instruction.opCodes.size(); i++)
    {
        file << endl << "     | ";
        file << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)instruction.opCodes[i] << " | ";
        WriteBinaryByte(file, (int)instruction.opCodes[i]);
        file << " |";
    }

    file << endl;
}


void StorageInstructions::CreateReport(pchar file_name)
{
    std::sort(instructions.begin(), instructions.end());

    ofstream file;

    file.open(file_name);

    for (size_t i = 0; i < instructions.size(); i++)
    {
        WriteCommand(file, instructions[i]);

        if (i < instructions.size() - 1)
        {
            int addressNext = instructions[i + 1].address;
            if ((addressNext - instructions[i].address) > (int)instructions[i].opCodes.size())
            {
                file << "--------------------------------------------------------------------------------------------------------------" << endl;
            }
        }
    }

    file << endl << endl << " ***** BAD ADDRESSES *****" << endl;

    for (size_t i = 0; i < instructions.size(); i++)
    {
        if (instructions[i].bad)
        {
            WriteCommand(file, instructions[i]);
        }
    }

    file.close();
}


void StorageInstructions::CreateProgram(ProgramASM &program)
{
    std::sort(instructions.begin(), instructions.end());

    program.Clear();

    wxTextFile file{ "out.asm" };

    file.Create();

    Instruction prev_instruction;

    if (instructions.size())
    {
        prev_instruction = instructions[0];
    }

    for (Instruction instruction : instructions)
    {
        LineASM line(instruction.mnemonic);

        line.address = instruction.address;

        for (uint i = 0; i < instruction.opCodes.size(); i++)
        {
            line.codes.push_back(instruction.opCodes[i]);
        }

        program.lines.push_back(line);

        {
            wxString text;

            if (instruction.address > prev_instruction.address + (int)prev_instruction.opCodes.size())
            {
                // Некоторые байты не декодированы. Выведем их

                size_t address = prev_instruction.address + prev_instruction.opCodes.size();

                text = wxString::Format("%05d/#%04X :                       BYTES :", (int)address, (int)address);

                size_t num_bytes = instruction.address - prev_instruction.address - prev_instruction.opCodes.size();

                for (size_t i = 0; i < num_bytes; i++)
                {
                    text.Append(wxString::Format(" %02X", RAM8(address++)));
                }

                file.AddLine(text);
            }

            text = wxString::Format("%05d/#%04X : %s", instruction.address, instruction.address, instruction.mnemonic);

            while (text.Length() < 60)
            {
                text.Append(" ");
            }

            text.Append(";");

            for (uint i = 0; i < instruction.opCodes.size(); i++)
            {
                text.Append(wxString::Format(" %02X", instruction.opCodes[i]));
            }

            while (text.Length() < 80)
            {
                text.Append(" ");
            }

            for (uint i = 0; i < instruction.opCodes.size(); i++)
            {
                text.Append(wxString::Format(" %3d", instruction.opCodes[i]));
            }

            file.AddLine(text);
        }

        prev_instruction = instruction;
    }

    file.Write();
    file.Close();
}


StorageInstructions::StorageInstructions(int _addr_min, int _addr_max) :
    addr_min(_addr_min), addr_max(_addr_max)
{
    AppendEntryPoint(0x0066);
    AppendEntryPoint(0x03F8);
    AppendEntryPoint(0x04C2);
    AppendEntryPoint(0x053F);
    AppendEntryPoint(0x0605);
    AppendEntryPoint(0x0A23);
    AppendEntryPoint(0x0A3D);
    AppendEntryPoint(0x0A4F);
    AppendEntryPoint(0x0A5F);
    AppendEntryPoint(0x0A87);
    AppendEntryPoint(0x0EAC);
    AppendEntryPoint(0x0FA9);
    AppendEntryPoint(0x0FF3);

    AppendEntryPoint(0x1015);
    AppendEntryPoint(0x1059);
    AppendEntryPoint(0x1076);
    AppendEntryPoint(0x107F);
    AppendEntryPoint(0x10A8);
    AppendEntryPoint(0x10B7);
    AppendEntryPoint(0x11B7);
    AppendEntryPoint(0x1555);
    AppendEntryPoint(0x15C4);
    AppendEntryPoint(0x1634);
    AppendEntryPoint(0x164D);
    AppendEntryPoint(0x16D4);
    AppendEntryPoint(0x16E5);
    AppendEntryPoint(0x171C);
    AppendEntryPoint(0x1736);
    AppendEntryPoint(0x1781);
    AppendEntryPoint(0x1793);
    AppendEntryPoint(0x17F5);
    AppendEntryPoint(0x1B76);
    AppendEntryPoint(0x1C0D);
    AppendEntryPoint(0x1C1F);
    AppendEntryPoint(0x1C6C);
    AppendEntryPoint(0x1C4E);
    AppendEntryPoint(0x1CBE);
    AppendEntryPoint(0x1D03);
    AppendEntryPoint(0x1DEC);
    AppendEntryPoint(0x1E27);
    AppendEntryPoint(0x1E42);
    AppendEntryPoint(0x1E4F);
    AppendEntryPoint(0x1E5F);
    AppendEntryPoint(0x1E7A);
    AppendEntryPoint(0x1E80);

    AppendEntryPoint(0x3214);
    AppendEntryPoint(0x33A2);
    AppendEntryPoint(0x33C6);
    AppendEntryPoint(0x340F);
    AppendEntryPoint(0x341B);
    AppendEntryPoint(0x342D);
    AppendEntryPoint(0x3449);
    AppendEntryPoint(0x3492);
    AppendEntryPoint(0x34A5);
    AppendEntryPoint(0x34B3);
    AppendEntryPoint(0x34BC);
    AppendEntryPoint(0x351B);
    AppendEntryPoint(0x353B);
    AppendEntryPoint(0x35C9);
    AppendEntryPoint(0x3645);
    AppendEntryPoint(0x3669);
    AppendEntryPoint(0x3674);
    AppendEntryPoint(0x367A);
    AppendEntryPoint(0x368F);
    AppendEntryPoint(0x369B);
    AppendEntryPoint(0x36A0);
    AppendEntryPoint(0x37E2);
}
