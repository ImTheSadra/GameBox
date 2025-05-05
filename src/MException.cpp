#include "MException.hpp"

MException::MException(int line, const char *file, const string &msg) noexcept
    : line(line), file(file), message(msg) {}

const char *MException::what() const noexcept
{
    ostringstream oss;
    oss << GetType() << "\n"
        << "Message: " << message << "\n"
        << "[File] " << file << "\n"
        << "[Line] " << line;
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *MException::GetType() const noexcept
{
    return "Error";
}

int MException::GetLine() const noexcept
{
    return line;
}

const string &MException::GetFile() const noexcept
{
    return file;
}

string MException::GetOriginString() const noexcept
{
    ostringstream oss;
    oss << "[File] " << file << "\n[Line] " << line;
    return oss.str();
}

#ifdef _WIN32

string GetErrorMessageString(HRESULT hr){
    switch (hr){
        case S_OK:return "EVERYONE ARE HAPPY HERE :)"; break;
        case E_INVALIDARG:return "INVALLID ARGUMENT"; break;
        case E_ACCESSDENIED:return "ACCESS IS DENIED"; break;
        case E_PENDING:return "PENDING..."; break;
        case E_UNEXPECTED:return "UNEXPECTED"; break;
        default:return "Unkown error (sorry) "+to_string(hr);
    }
}

string MException::translateMessgae(HRESULT hr)
{
    switch (hr)
    {
    default:
        return "unknown hresult"; break;
    }
}

#endif

void MException::ShowMessageBox() const noexcept {
    #ifdef _WIN32
        int r = MessageBoxA(nullptr, what(), (const char*)((string)GetType()+" GameBox").c_str(), MB_ICONERROR | MB_OKCANCEL);
        if (r == IDCANCEL) {
            cerr << GetType() << "\t" << what() << endl;
            exit(1);
        }
    #else
        std::string command = "zenity --error --text=\"" + std::string(what()) + 
                             "\" --title=\"" + std::string(GetType()) + "\"";
        if (system(nullptr)) { 
            int result = system(command.c_str());
            if (result != 0) { 
                command = "kdialog --error \"" + std::string(what()) + 
                         "\" --title \"" + std::string(GetType()) + "\"";
                result = system(command.c_str());
                if (result != 0) { 
                    command = "xmessage -center \"" + std::string(what()) + "\"";
                    system(command.c_str());
                }
            }
        } else {
            std::cerr << GetType() << "\t" << what() << std::endl;
        }
    #endif
}