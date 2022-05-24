// 2022/05/23 16:56:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>


using std::ofstream;
using std::string;
using std::vector;
using std::endl;


int StorageInstructions::NextAddress()
{
    if(addressesForScan.size() > 0)
    {
        return addressesForScan[0];
    }
    return -1;
}


void StorageInstructions::AddNewData(bool succsefull, int address, InfoStruct *params)
{
    RemoveScanAddress(address);

    Instruction command;
    
    command.address = address;

    for(int i = 0; i < params->numOpCodes; i++)
    {
        command.opCodes.push_back(params->opCodes[i]);
    }

    if(succsefull)
    {
        command.numTackts = params->tackts;

        command.mnemonic = std::string(params->mnemonic);

        for(int i = 0; i < params->numAddresses; i++)
        {
            AppendEntryPoint((int)params->addresses[i]);
        }

        command.transcript = string(params->transcript);

        command.flags = string(params->flags);

        command.comment = string(params->comment);

        command.tackts = params->tackts;
    }
    else
    {
        command.bad = true;
    }

    instructions.push_back(command);
}


void StorageInstructions::RemoveScanAddress(int address)
{
    for(int i = 0; i < (int)addressesForScan.size(); i++)
    {
        if(addressesForScan[(uint)i] == address)
        {
            addressesForScan.erase(addressesForScan.begin() + i);
            return;
        }
    }
}


void StorageInstructions::AppendEntryPoint(int address)
{
    if(address >= addr_min && address < addr_max && !AddressAlreadyScanOrFuture(address))
    {
        addressesForScan.push_back(address);
    }
}


bool StorageInstructions::AddressAlreadyScanOrFuture(int address)
{
    for each (auto &command in instructions)
    {
        if(command.address == address)
        {
            return true;
        }
    }

    for each(auto addr in addressesForScan)
    {
        if(addr == address)
        {
            return true;
        }
    }

    return false;
}


void StorageInstructions::WriteBinaryByte(std::ofstream &file, int value)
{
    vector<int> bits;

    for(int i = 0; i < 8; i++)
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


void StorageInstructions::WriteCommand(ofstream &file, Instruction &command)
{
    file << std::right << std::hex << std::setw(4) << std::setfill('0') << command.address << " | ";

   file << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)command.opCodes[0] << " | ";
   WriteBinaryByte(file, (int)command.opCodes[0]);
   file << " | ";

    if(command.bad)
    {
        file << "ERROR!!! BAD COMMAND!!! ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    else
    {
        file << std::setw(14) << std::left << std::setfill(' ') << command.mnemonic;
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
        if(!command.comment.empty())
        {
            file << " | " <<  command.comment;
        }
    }

    for(uint i = 1; i < command.opCodes.size(); i++)
    {
        file << endl << "     | ";
        file << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)command.opCodes[i] << " | ";
        WriteBinaryByte(file, (int)command.opCodes[i]);
        file << " |";
    }

    file << endl;
}


void StorageInstructions::CreateReport(pchar file_name)
{
    std::sort(instructions.begin(), instructions.end());

    ofstream file;

    file.open(file_name);

    for(size_t i = 0; i < instructions.size(); i++)
    {
        WriteCommand(file, instructions[i]);

        if(i < instructions.size() - 1)
        {
            int addressNext = instructions[i + 1].address;
            if ((addressNext - instructions[i].address) > (int)instructions[i].opCodes.size())
            {
                file << "--------------------------------------------------------------------------------------------------------------" << endl;
            }
        }
    }

    file << endl << endl << " ***** BAD ADDRESSES *****" << endl;

    for(size_t i = 0; i < instructions.size(); i++)
    {
        if(instructions[i].bad)
        {
            WriteCommand(file, instructions[i]);
        }
    }

    file.close();
}


void StorageInstructions::CreateProgram(ProgramASM &program)
{
    std::sort(instructions.begin(), instructions.end());
}
