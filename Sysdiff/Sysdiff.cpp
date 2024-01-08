#include "Sysdiff.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <map>
#include <openssl/sha.h>
#include <string>
#include "FileMap.h"
#include <fstream>
#include <args.hxx>

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

void find_files(FileMapLeaf* parent, FileMap& fileMap, int maxDepth, int currentdepth, bool evade) {
    if (!exists(parent->getPath()));
    boost::filesystem::directory_iterator end_itr;
    boost::filesystem::directory_iterator itr(parent->getPath());
    while (itr != end_itr) {
        try {
            if (evade) {
                // this temporary as should probably be implemented better
                if (itr->path().string() == "/proc" || itr->path().string() == "/Windows") { ++itr; continue; }
            }

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
                    find_files(leaf, fileMap, maxDepth, depth, evade);
                }
            }
        }
        catch (...) {
        }
        ++itr;
    }
}

void saveFile(std::string toSave) {
    std::ofstream outfile("sdiff.bin", std::ios_base::binary);
    outfile.write(toSave.c_str(), toSave.size());
    outfile.close();
}

int main(int argc, char** argv) {
    args::ArgumentParser parser("Sysdiff", "");
    args::HelpFlag help(parser, "help", "help", { 'h', "help" });
    args::ValueFlag<int> maxDepth(parser, "maxDepth", "Max Depth", { 'd' }, 5);
    args::Flag save(parser, "save", "If it should save to sdiff.bin :. it will not print to std::cout", { 's', "save" });
    args::Flag evade(parser, "evade", "Don't go into commonly bad folders", { 'e', "evade" });
    args::ValueFlag<std::string> startPath(parser, "startPath", "Start Path", {'p', "path"}, "/");


    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help& h) {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    FileMap fileMap;

    int i_maxDepth = args::get(maxDepth);
    boost::filesystem::path rootPath(args::get(startPath));
    FileMapLeaf* root = new FileMapLeaf("", rootPath);
    fileMap.setRoot(root);
    find_files(root, fileMap, i_maxDepth, 0, args::get(evade));

    if (args::get(save)) {
        saveFile(root->getSave().dump());
    }
    else {
        std::cout << root->getSave().dump() << "\n";
    }

    return 0;
}