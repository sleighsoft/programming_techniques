#include <iostream>
#include <string>
#include <algorithm>
#include <regex>

#define ENTRIES 7

std::wstring dataset[ENTRIES][4] = { { L"max.muestermann@bmw.de", L"Max", L"Mustermann", L"SAP" },
    { L"juergen.doellner@hpi.de", L"Jürgen", L"Döllner", L"HPI" },
    { L"soeren.discher@hpi.de", L"Sören", L"Discher", L"HPI" } ,
    { L"daniel.maeller@hpi.de", L"Daniel", L"Mäller", L"HPI" },
    { L"paul.deissler@hertha.de", L"Sebastian", L"Deißler", L"herTha" },
	{ L"mueller.marga@sap.com", L"Marga", L"Müller", L"SAP" },
    { L"h.boss@service.bayer.com", L"Hugo", L"Boss", L"Bayer" }};

void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void caseConvert(std::wstring& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void replace(std::wstring& str) {
    ReplaceAll(str, L"ä", L"ae");
    ReplaceAll(str, L"ü", L"ue");
    ReplaceAll(str, L"ö", L"oe");
    ReplaceAll(str, L"ß", L"ss");   
}

bool emailCheck(std::wstring mail, std::wstring firstname, std::wstring name, std::wstring company)
{
    std::wcout << mail << " Firstname: " << firstname << " Name: " << name << " Company: " << company << std::endl;
    std::wregex pieces_regex(L"([a-zA-Z]+)\\.([a-zA-Z]+)@([a-zA-Z]+(?:\\.[a-zA-Z]+)+)");
    std::wsmatch pieces_match;
    
    caseConvert(firstname);
    caseConvert(name);
    caseConvert(company);
    replace(firstname);
    replace(name);
    replace(company);
    
    bool firstnameMatches = false;
    bool nameMatches = false;
    bool companyMatches = false;
    if (std::regex_search(mail, pieces_match, pieces_regex))
    {
        for (size_t i = 1; i < pieces_match.size(); ++i)
        {
            std::wssub_match sub_match = pieces_match[i];
            std::wstring piece = sub_match.str();
            if(i == 1)
            {
                if(!piece.compare(firstname) || !piece.compare(firstname.substr(0, 1)))
                {
                    firstnameMatches = true;
                }
                else if(!piece.compare(name) || !piece.compare(name.substr(0, 1)))
                {
                    nameMatches = true;
                }
            }
            else if(i == 2)
            {
                if(!piece.compare(name))
                {
                    nameMatches = true;
                }
                else if(!piece.compare(firstname))
                {
                    firstnameMatches = true;
                }
            }
            else if(i == 3)
            {
                std::wregex company_regex(L"(?:(\\w+)\\.\\w+)$");
                std::wsmatch company_match;
                if (std::regex_search(piece, company_match, company_regex))
                {
                    for (size_t i = 1; i < company_match.size(); ++i)
                    {
                         std::wssub_match company_sub_match = company_match[i];
                         std::wstring company_piece = company_sub_match.str();
                         if(!company_piece.compare(company))
                         {
                             companyMatches = true;
                             break;
                         }
                    }
                }
            }
        } 
    }
    return (firstnameMatches && nameMatches && companyMatches) ? true : false;
}

int main(int argc, char * argv[])
{
    for (int i = 0; i < ENTRIES; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            replace(dataset[i][j]);
            caseConvert(dataset[i][j]);
        }
        
        std::cout << emailCheck(dataset[i][0], dataset[i][1], dataset[i][2], dataset[i][3]) << std::endl;
    }
    
    return 0;
    
}