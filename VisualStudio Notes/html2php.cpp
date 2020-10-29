// html2php.cpp : This file contains the 'main' function. 
// Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <time.h>

using namespace std;
using namespace filesystem;

struct cmdlineArgs
{
    string strInDir;
    string strOutDir;
    string strInFile;
} mArgs;

void ShowCmdLineUsage(void);
bool GetCmdLineArgs(bool = true);
string ReplaceString(string strInput, string oldStr, string newStr);
void TestParser(void);
string GetTimeDate(void);

const string strHeaderStartTag = "<header";
const string strHeaderEndTag = "</header";
const string strFooterStartTag = "<footer";
const string strFooterEndTag = "</footer";

int main()
{
    string strInFilename, strOutFilename;
    GetCmdLineArgs();

    if (__argc >= 2)
    {
        cout << "full argument:" << __argv[1] << endl; // input filename
        strInFilename = __argv[1];

        // Strip out the .html part the filename, if present.
        strInFilename = strInFilename.substr(0, strInFilename.find("."));
        cout << "First part of filename: '" << strInFilename << "'" << endl;

    }
    else // Show the arguments
    {
        GetCmdLineArgs(true); // Get & show the cmd line arguments.
    }

    TestParser();

    /*
        if (__argc <= 1)
        {
            ShowCmdLineUsage();
        }
        else // Show the arguments
        {
            GetCmdLineArgs(true); // Get & show the cmd line arguments.
        }
   */

    // TEST filesystem code here: START
    // NOTE for Visual Studio 2019 users: 
    // I had to go to Project: html2php Properties: Configuration Properties: C++: Language: 
    //      C++ Language Standard: and then choose C++17
    //      WHAT a pain in the a** !!!

    /* *****
    cout << current_path().string() << endl;
    for (auto& p : std::filesystem::directory_iterator("html_files"))
        std::cout << p.path() << '\n';
    */
 
    // TEST filesystem code here: END

    bool bFoundHeaderStart = false;
    bool bFoundHeaderEnd = false;
    bool bFoundFooterStart = false;
    bool bFoundFooterEnd = false;

    // This works !!!
    ifstream inFile;
    ofstream outFile;

    strOutFilename = "php_files\\" + strInFilename + ".php";
    strInFilename = "html_files\\" + strInFilename + ".html";

    inFile.open(strInFilename);
    outFile.open(strOutFilename);
    int lineNum = 0;
    string s;

    //cout << "TimeDate : " << GetTimeDate() << endl;
    outFile << "<?php " << endl << "// Created by html2php.exe on: " << GetTimeDate() << endl;
    outFile << "include_once (\"common-header-footer.php\");" << endl << "?>" << endl;

    // Read file until find Header tags.
    while (getline(inFile, s))
    {
        lineNum++;

        bFoundHeaderStart = ( s.find(strHeaderStartTag) != string::npos );

        if (bFoundHeaderStart && !bFoundHeaderEnd)
        {
            cout << "found header start. ";
            outFile << "<!--HEADER REMOVED by html2php program -->" << endl;

            // Now read & discard until finding end of header.
            while (getline(inFile, s) && !bFoundHeaderEnd)
            {
                bFoundHeaderEnd = (s.find(strHeaderEndTag) != string::npos);
            }
            outFile << "<?php EchoHeader(); ?>" << endl;

            s = ReplaceString(s, ".html", ".php");
            outFile << s << endl;

            cout << "found header end." << endl;
            break;
        }
        else
        {
            s = ReplaceString(s, ".html", ".php");
            outFile << s << endl;
        }

    }

     // Read file until find Footer tags.
    while (getline(inFile, s))
    {
        lineNum++;

        bFoundFooterStart = (s.find(strFooterStartTag) != string::npos);

        if (bFoundFooterStart && !bFoundFooterEnd)
        {
            cout << "found Footer start. ";
            outFile << "<!--Footer REMOVED by html2php program -->" << endl;

            // Now read & discard until finding end of Footer tag.
            while (getline(inFile, s) && !bFoundFooterEnd)
            {
                bFoundFooterEnd = (s.find(strFooterEndTag) != string::npos);
            }
            outFile << "<?php EchoFooter(); ?>" << endl;

            s = ReplaceString(s, ".html", ".php");
            outFile << s << endl;

            cout << "found Footer end." << endl;
            break;
        }
        else
        {
            s = ReplaceString(s, ".html", ".php");
            outFile << s << endl;
        }

    }

    // Copy remainder of file.
    while (getline(inFile, s))
    {
        s = ReplaceString(s, ".html", ".php");
        outFile << s << endl;
    }
    outFile.close();

}

void ShowCmdLineUsage(void)
{
    cout << "Command Line options:" << endl;
    cout << "-a, --all : process all files in directory" << endl;
    cout << "-v, --verbose : prints verbose desciption of progress" << endl;
    cout << "-id, --inputdir : read & process all files in this directory" << endl;
    cout << "-od, --outputdir : write output files to this directory" << endl;
    cout << "-hf, --headerfile : filename to get header text from" << endl;
    cout << "-ff, --footerfile : filename to get footer text from" << endl;
    cout << "-nh, --noheader : do not output full header, wrap found header in comment" << endl;
    cout << "-nf, --nofooter : do not output full footer, wrap found footer in comment" << endl;
    cout << "-a[all] : process all files in directory." << endl;
}

bool GetCmdLineArgs(bool bShowArgs)
{
    if (bShowArgs)
        cout << "Command Line Arguments:";

    mArgs.strInDir = current_path().string(); // Default input dir is current dir.
    //cout << mArgs.strInDir << endl;
    if (bShowArgs)
    {
        for (int i = 1; i < __argc; i++)
        {
            cout << __argv[i] << ' ';
        }
        cout << endl;
    }
    return true;
}

// Returns string with all occurences of strSearch replaced with strReplace.
string ReplaceString(string strInput, string strSearch, string strReplace)
{
    if (strInput.find(strSearch) != string::npos)
    {
        size_t pos;
        while ((pos = strInput.find(strSearch)) != std::string::npos)
        {
            strInput.replace(pos, strSearch.length(), strReplace);
        }
    }

    return strInput;
}

// Made this a separate function because ENTIRELY too much code due to the way C++ has been buggered up.
string GetTimeDate(void)
{
    time_t rawtime;
    time(&rawtime);
    //printf("The current local time is: %s", ctime(&rawtime));

    char buffer[64];
    auto t = std::time(nullptr);
    auto foo = *std::localtime(&t);
    std::strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", &foo);
    //std::cout << buffer << endl;
    return buffer;
}

// Test parsing logic here, then use it to parse ini file, etc.
void TestParser(void)
{
    string temp = "parameter=MyFileName.html";
    int nDelimiter = temp.find('=');

    string parmToken = temp.substr(0, temp.find("="));

    //temp = temp.substr(temp.find(" ") + 1);
    //string delimiter = temp.substr(0, temp.find(" "));

    string parmValue = temp.substr(nDelimiter + 1);

    cout << "nDelimiter: " << nDelimiter << endl;
    cout << "token: "       << parmToken << endl;
    //cout << "delimiter: "   << delimiter << endl;
    cout << "value: "       << parmValue << endl;
}
