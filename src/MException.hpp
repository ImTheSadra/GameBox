#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <exception>

using namespace std;

#ifdef _WIN32
#include <windows.h>

string GetErrorMessageString(HRESULT hr);

class MException : public exception
{
public:
    MException(int line, const char *file, const string &msg = "Unknown error") noexcept;
    const char *what() const noexcept override;
    virtual const char *GetType() const noexcept;
    int GetLine() const noexcept;
    const string &GetFile() const noexcept;
    string GetOriginString() const noexcept;
    void ShowMessageBox() const noexcept;
    string translateMessgae(HRESULT);

private:
    int line;
    string file;
    string message;

protected:
    mutable string whatBuffer;
};

#else
#include <string>

using namespace std;

class MException : public exception
{
public:
    MException(int line, const char *file, const string &msg = "Unknown error") noexcept;
    const char *what() const noexcept override;
    virtual const char *GetType() const noexcept;
    int GetLine() const noexcept;
    const string &GetFile() const noexcept;
    string GetOriginString() const noexcept;
    void ShowMessageBox() const noexcept;

private:
    int line;
    string file;
    string message;

protected:
    mutable string whatBuffer;
};

#endif