#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

using InputToFile = std::ofstream;
using OutputFromFile = std::ifstream;
using FileStream = std::fstream;
namespace FileAPI{
    class FileHandler {//Files are indexed from 0
    private:
        std::string filename;
        std::vector<std::string> lines;
        int readPtr; // Pointer to track current read position
        int writePtr; // Pointer to track current write position
        bool isSaved;
        bool isLoaded;

        #ifdef FILEHANDLE_THREADSAFE//For multithreaded environments (slower than without mutex)
            mutable std::mutex fileMutex;
        #endif
        
        // ========== PRIVATE UNSAFE METHODS ==========
        // These assume the mutex is already locked by the caller
        
        void checkLoadedUnsafe() const {
            if(!isLoaded){
                std::cerr << "File not loaded. Please load the file before performing operations.\n";
                exit(1);
            }
        }
        
        void advanceReadPtrUnsafe(bool move){
            if(move && !EOFReadptrUnsafe()){
                ++readPtr;
            }
        }
        
        void advanceWritePtrUnsafe(bool move){
            if(move && !EOFWriteptrUnsafe()){
                ++writePtr;
            }
        }
        
        bool EOFReadptrUnsafe() const {
            return readPtr >= lines.size();
        }
        
        bool EOFWriteptrUnsafe() const {
            return writePtr >= lines.size();
        }
        
        bool FindJumpReadptrUnsafe(const std::string& searchString){
            for(size_t i = 0; i < lines.size(); ++i){
                if(lines[i].find(searchString) != std::string::npos){
                    readPtr = i;
                    return true;
                }
            }
            return false;
        }
        
        bool FindJumpWriteptrUnsafe(const std::string& searchString){
            for(size_t i = 0; i < lines.size(); ++i){
                if(lines[i].find(searchString) != std::string::npos){
                    writePtr = i;
                    return true;
                }
            }
            return false;
        }

    public:
        FileHandler(const std::string& filename, const bool loadFile = false) 
            : filename(filename), readPtr(0), writePtr(0), isSaved(true), isLoaded(loadFile){
            if(loadFile){
                load();
            }
        }
        
        ~FileHandler(){
            if(!isSaved){
                save();
            }
        }
        
        // ========== PUBLIC THREAD-SAFE METHODS ==========
        // All public methods handle their own locking
        
        void load(){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            lines.clear();
            OutputFromFile file(filename);
            if(!file.is_open()){
                std::cerr << "Error opening file for reading: " << filename << '\n';
                exit(1);
            }
            std::string line;
            while(std::getline(file, line)){
                lines.push_back(line);
            }
            file.close();
            isSaved = true;
            isLoaded = true;
        }
        
        void save(){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            if(isSaved) return;
            InputToFile file(filename, std::ios::trunc);
            if(!file.is_open()){
                std::cerr << "Error opening file for saving: " << filename << '\n';
                exit(1);
            }
            for(const auto& line : lines){
                file << line << '\n';
            }
            file.close();
            isSaved = true;
        }
        
        bool isFileLoaded() const {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            return isLoaded;
        }
        
        bool isFileSaved() const {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            return isSaved;
        }
        
        void updateReadptr(int lineNumber){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(lineNumber < 0 || lineNumber >= lines.size()){
                std::cerr << "Invalid line number for read pointer update.\n";
                exit(1);
            }
            readPtr = lineNumber;
        }
        
        void updateWriteptr(int lineNumber){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(lineNumber < 0 || lineNumber >= lines.size()){
                std::cerr << "Invalid line number for write pointer update.\n";
                exit(1);
            }
            writePtr = lineNumber;
        }
        
        bool EOFReadptr() const {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return EOFReadptrUnsafe();
        }
        
        bool EOFWriteptr() const {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return EOFWriteptrUnsafe();
        }
        
        bool FindJumpReadptr(const std::string& searchString){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return FindJumpReadptrUnsafe(searchString);
        }
        
        bool FindJumpWriteptr(const std::string& searchString){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return FindJumpWriteptrUnsafe(searchString);
        }
        
        void resetReadptr(){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            readPtr = 0;
        }
        
        void resetWriteptr(){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            writePtr = 0;
        }
        
        size_t size() const { 
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return lines.size(); 
        }
        
        bool empty() const { 
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            return lines.empty(); 
        }
        
        int getReadPtr() const { 
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            return readPtr; 
        }
        
        int getWritePtr() const { 
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            return writePtr; 
        }
        
        std::vector<std::string> getLines(const int startLine, const int endLine){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(startLine < 0 || endLine > lines.size() - 1 || startLine > endLine){
                std::cerr << "Invalid line range specified.\n";
                exit(1);
            }
            std::vector<std::string> readLines;
            for(int i = startLine; i <= endLine; ++i){
                readLines.push_back(lines[i]);
            }
            return readLines;
        }
        
        std::string readLine(bool movePtr = false){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(readPtr < 0 || readPtr >= lines.size()){
                std::cerr << "Read pointer out of bounds.\n";
                exit(1);
            }
            std::string line = lines[readPtr];
            advanceReadPtrUnsafe(movePtr);
            return line;
        }
        
        std::string getLine(const int lineNumber) {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(lineNumber < 0 || lineNumber >= lines.size()){
                std::cerr << "Invalid line number specified.\n";
                exit(1);
            }
            return lines[lineNumber];
        }
        
        void writeLine(const std::string& newLine, bool movePtr = false){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(writePtr < 0 || writePtr > lines.size()){
                std::cerr << "Write pointer out of bounds.\n";
                exit(1);
            }
            if (writePtr == lines.size()) {
                lines.push_back(newLine);
            } else {
                lines[writePtr] = newLine;
            }
            advanceWritePtrUnsafe(movePtr);
            isSaved = false;
        }
        
        void insertLine(const int lineNumber, const std::string& newLine){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            if(lineNumber < 0 || lineNumber > lines.size()){
                std::cerr << "Invalid line number for insertion.\n";
                exit(1);
            }
            lines.insert(lines.begin() + lineNumber, newLine);
            isSaved = false;
        }
        
        void appendLine(const std::string& newLine){
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            checkLoadedUnsafe();
            lines.push_back(newLine);
            isSaved = false;
        }
        
        void insertAtRandomLine(const std::string& text) {
            #ifdef FILEHANDLE_THREADSAFE
                std::lock_guard<std::mutex> lock(fileMutex);
            #endif
            if (lines.empty()) return;
            int randomLine = rand() % lines.size();
            lines.insert(lines.begin() + randomLine, text);
            isSaved = false;
        }
    };
}