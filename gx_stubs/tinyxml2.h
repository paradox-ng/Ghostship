#pragma once
// Stub tinyxml2 for the console build. The only compiled users are the audio
// importer factories' XML-metadata path, which is not exercised by the base game's
// binary o2r. The classes/methods exist so those factories compile and link;
// parsing always reports failure / returns no elements.

#include <cstddef>
#include <cstdint>

namespace tinyxml2 {

enum XMLError {
    XML_SUCCESS = 0,
    XML_NO_ATTRIBUTE,
    XML_WRONG_ATTRIBUTE_TYPE,
    XML_ERROR_FILE_NOT_FOUND,
    XML_ERROR_PARSING,
    XML_ERROR_EMPTY_DOCUMENT,
};

class XMLElement {
  public:
    XMLElement* FirstChildElement(const char* = nullptr) { return nullptr; }
    XMLElement* NextSiblingElement(const char* = nullptr) { return nullptr; }
    const char* Attribute(const char* = nullptr, const char* = nullptr) const { return nullptr; }
    const char* GetText() const { return nullptr; }
    const char* Name() const { return ""; }
    int IntAttribute(const char* = nullptr, int defaultValue = 0) const { return defaultValue; }
    unsigned UnsignedAttribute(const char* = nullptr, unsigned defaultValue = 0) const { return defaultValue; }
    int64_t Int64Attribute(const char* = nullptr, int64_t defaultValue = 0) const { return defaultValue; }
    float FloatAttribute(const char* = nullptr, float defaultValue = 0.0f) const { return defaultValue; }
    double DoubleAttribute(const char* = nullptr, double defaultValue = 0.0) const { return defaultValue; }
    bool BoolAttribute(const char* = nullptr, bool defaultValue = false) const { return defaultValue; }
};

class XMLNode {
  public:
    XMLElement* FirstChildElement(const char* = nullptr) { return nullptr; }
    XMLNode* NextSibling() { return nullptr; }
};

class XMLDocument {
  public:
    XMLError Parse(const char* = nullptr, size_t = 0) { return XML_ERROR_EMPTY_DOCUMENT; }
    XMLElement* RootElement() { return nullptr; }
    XMLElement* FirstChildElement(const char* = nullptr) { return nullptr; }
    bool Error() const { return true; }
    XMLError ErrorID() const { return XML_ERROR_EMPTY_DOCUMENT; }
    const char* ErrorStr() const { return "tinyxml2 stubbed (no XML parser on console)"; }
};

} // namespace tinyxml2
