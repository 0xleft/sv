#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>

class FileMapLeaf {
	FileMapLeaf* parent = nullptr;
	std::vector<FileMapLeaf*> children;
	std::string hash;
	boost::filesystem::path path;

public:
	FileMapLeaf(std::string hash, boost::filesystem::path path) {
		this->hash = hash;
		this->path = path;
	}

	std::string getSave(int depth) {
		std::string finalS;
		std::string spacer(depth, '\t');


		for (FileMapLeaf* child : this->children) {
			int newDepth = depth + 1;
			finalS += spacer + child->getSave(newDepth) + "\n";
		}

		return this->getPath().string() + " hash: " + this->hash + (finalS == "" ? "" : "\n") + finalS;
	}

	void setParent(FileMapLeaf* parent) { this->parent = parent; }
	void addChild(FileMapLeaf* child) { this->children.push_back(child); }
	
	std::vector<FileMapLeaf*> getChildren() { return this->children; }
	FileMapLeaf* getParent() { return this->parent; }
	boost::filesystem::path getPath() { return this->path; }

	~FileMapLeaf() {
		for (FileMapLeaf* child : this->children) {
			delete child;
		}
	}
};

class FileMap {
	FileMapLeaf* root;

public:
	void setRoot(FileMapLeaf* root) {
		this->root = root;
	}

	FileMapLeaf* getRoot() { return this->root; }

	~FileMap() {
		delete this->root;
	}
};