// 2026/11/02 09:55:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTAP.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


bool ParserTAP::Execute(pchar fileName)
{
    file = std::make_shared<FileTAP>();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file->dump.FillFrom(fileName);

    wxFileInputStream file_input(fileName);

    wxBufferedInputStream stream(file_input);

    wxDataInputStream data(stream);

    while (data.IsOk())
    {
        BlockTAP *header = BlockTAP::ReadHeaderBlock(data);

        if (header)
        {
            if (header->IsHeader())
            {
                file->AppendBlock(std::shared_ptr<BlockTAP>(header));

                BlockTAP *block = BlockTAP::ReadDataBlock(data, header);

                if (block)
                {
                    file->AppendBlock(std::shared_ptr<BlockTAP>(block));
                }
            }
        }
        else
        {
            while (data.IsOk())
            {
                data.Read8();
                file->number_unreaded_bytes++;
            }

            break;
        }
    }

    file->is_valid = (file->NumberBlocks() != 0);

    return file->is_valid;
}
