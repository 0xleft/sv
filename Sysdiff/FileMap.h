#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <json.hpp>

using json = nlohmann::json;

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

	json getSave() {
		json out;
		out["children"] = json::array();
		out["hash"] = this->hash;
		out["path"] = this->path.string();

		for (FileMapLeaf* child : this->children) {
			json childOut = child->getSave();
			out["children"].push_back(childOut);
		}

		return out;
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