#include "Sysdiff.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <map>
#include <openssl/sha.h>
#include <string>
#include "FileMap.h"

std::string sha256(const std::string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void find_files(FileMapLeaf* parent, FileMap& fileMap, int maxDepth, int currentdepth) {
    if (!exists(parent->getPath()));
    boost::filesystem::directory_iterator end_itr;
    boost::filesystem::directory_iterator itr(parent->getPath());
    while (itr != end_itr) {
        try {
            if (strstr(itr->path().string().c_str(), "Windows")) { ++itr; continue; }
            if (is_directory(itr->status())) {
                boost::filesystem::directory_iterator inner_end_itr;
                boost::filesystem::directory_iterator inner_itr(itr->path());
                std::string totalS;
                while (inner_itr != inner_end_itr) {
                    try {
                        totalS += inner_itr->path().filename().string();
                    }
                    catch (...) {
                    }
                    ++inner_itr;
                }

                std::string hash = sha256(totalS);
                FileMapLeaf* leaf = new FileMapLeaf(hash, itr->path());
                leaf->setParent(parent);
                leaf->getParent()->addChild(leaf);

                if (currentdepth < maxDepth) {
                    int depth = currentdepth + 1;
                    find_files(leaf, fileMap, maxDepth, depth);
                }
            }
        }
        catch (...) {
        }
        ++itr;
    }
}

int main() {
    FileMap fileMap;
    
    int maxDepth = 2;
    boost::filesystem::path rootPath("/");
    FileMapLeaf* root = new FileMapLeaf("", rootPath);
    fileMap.setRoot(root);
    find_files(root, fileMap, maxDepth, 0);

    std::cout << root->getSave(0);

    return 0;
}
