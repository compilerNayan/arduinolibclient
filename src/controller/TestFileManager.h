#ifndef TEST_FILE_MANAGER_H
#define TEST_FILE_MANAGER_H

#include <IFileManager.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

/**
 * Test File Manager that tracks all file operations for testing
 * Stores files in memory and provides access to verify operations
 */
class TestFileManager  {
private:
    // In-memory file storage
    StdMap<StdString, StdString> files;
    
    // Track operations for verification
    StdVector<StdString> createOperations;
    StdVector<StdString> readOperations;
    StdVector<StdString> updateOperations;
    StdVector<StdString> deleteOperations;
    StdVector<StdString> appendOperations;

public:
    TestFileManager() = default;
    virtual ~TestFileManager() = default;

    // Create: Create a new file with the given filename and contents
    Bool Create(CStdString& filename, CStdString& contents) {
        StdString fileKey = StdString(filename.c_str());
        files[fileKey] = StdString(contents.c_str());
        createOperations.push_back(fileKey);
        std::cout << "[TestFileManager] Created file: " << filename.c_str() << std::endl;
        return true;
    }

    // Read: Read the contents of a file with the given filename
    StdString Read(CStdString& filename) {
        StdString fileKey = StdString(filename.c_str());
        readOperations.push_back(fileKey);
        
        auto it = files.find(fileKey);
        if (it != files.end()) {
            std::cout << "[TestFileManager] Read file: " << filename.c_str() << std::endl;
            return it->second;
        }
        
        std::cout << "[TestFileManager] File not found: " << filename.c_str() << std::endl;
        return StdString("");
    }

    // Update: Update an existing file with the given filename and new contents
    Bool Update(CStdString& filename, CStdString& contents) {
        StdString fileKey = StdString(filename.c_str());
        files[fileKey] = StdString(contents.c_str());
        updateOperations.push_back(fileKey);
        std::cout << "[TestFileManager] Updated file: " << filename.c_str() << std::endl;
        return true;
    }

    // Delete: Delete a file with the given filename
    Bool Delete(CStdString& filename) {
        StdString fileKey = StdString(filename.c_str());
        auto it = files.find(fileKey);
        if (it != files.end()) {
            files.erase(it);
            deleteOperations.push_back(fileKey);
            std::cout << "[TestFileManager] Deleted file: " << filename.c_str() << std::endl;
            return true;
        }
        std::cout << "[TestFileManager] File not found for deletion: " << filename.c_str() << std::endl;
        return false;
    }

    // Append: Append contents to an existing file
    Bool Append(CStdString& filename, CStdString& contents) {
        StdString fileKey = StdString(filename.c_str());
        files[fileKey] += StdString(contents.c_str());
        appendOperations.push_back(fileKey);
        std::cout << "[TestFileManager] Appended to file: " << filename.c_str() << std::endl;
        return true;
    }

    // Test helper methods
    Bool FileExists(CStdString& filename) const {
        StdString fileKey = StdString(filename.c_str());
        return files.find(fileKey) != files.end();
    }

    StdString GetFileContents(CStdString& filename) const {
        StdString fileKey = StdString(filename.c_str());
        auto it = files.find(fileKey);
        if (it != files.end()) {
            return it->second;
        }
        return StdString("");
    }

    void Clear() {
        files.clear();
        createOperations.clear();
        readOperations.clear();
        updateOperations.clear();
        deleteOperations.clear();
        appendOperations.clear();
    }

    size_t GetFileCount() const {
        return files.size();
    }

    StdVector<StdString> GetCreateOperations() const {
        return createOperations;
    }

    StdVector<StdString> GetReadOperations() const {
        return readOperations;
    }

    StdVector<StdString> GetUpdateOperations() const {
        return updateOperations;
    }

    StdVector<StdString> GetDeleteOperations() const {
        return deleteOperations;
    }
};

#endif // TEST_FILE_MANAGER_H

