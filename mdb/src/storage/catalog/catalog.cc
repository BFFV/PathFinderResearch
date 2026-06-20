#include "catalog.h"

#include <cassert>
#include <stdexcept>

#include "graph_models/gql/gql_catalog.h"
#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/quad_model/quad_catalog.h"
#include "misc/fatal_error.h"
#include "system/file_manager.h"

using namespace std;

Catalog::Catalog(const string& filename)
{
    auto file_path = file_manager.get_file_path(filename);
    file.open(file_path, ios::out | ios::app);
    if (file.fail()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    file.close();
    file.open(file_path, ios::in | ios::out | ios::binary);
}

Catalog::~Catalog()
{
    file.close();
}

Catalog::ModelID Catalog::get_model_id(const std::string& db_dir) {
    auto catalog_path = db_dir + "/catalog.dat";
    auto catalog_fs = std::fstream(catalog_path, std::ios::in | std::ios::binary);
    if (!catalog_fs.is_open()) {
        FATAL_ERROR("Could not open catalog: ", catalog_path);
    }

    uint64_t model_id = 0;
    uint8_t buf[8];
    catalog_fs.read((char*) buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
        model_id |= static_cast<uint64_t>(buf[i]) << shift;
    }

    if (!catalog_fs.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    switch (model_id) {
        case GQLCatalog::MODEL_ID:
            return Catalog::ModelID::GQL;
        case RdfCatalog::MODEL_ID:
            return Catalog::ModelID::RDF;
        case QuadCatalog::MODEL_ID:
            return Catalog::ModelID::QUAD;
        default:
            FATAL_ERROR("Unknown ModelID in catalog: ", model_id);
            return Catalog::ModelID::GQL; // only to avoid warning, program will terminate
    }
}

bool Catalog::is_empty()
{
    file.seekg(0, file.end);
    return file.tellg() == 0;
}

void Catalog::start_io()
{
    file.seekg(0, file.beg);
}

uint8_t Catalog::read_uint8()
{
    auto res = static_cast<uint8_t>(file.get());

    if (!file.good()) {
        throw std::runtime_error("Error reading uint8");
    }

    return res;
}

uint64_t Catalog::read_uint64()
{
    uint64_t res = 0;
    uint8_t buf[8];
    file.read((char*) buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
        res |= static_cast<uint64_t>(buf[i]) << shift;
    }

    if (!file.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}

uint_fast32_t Catalog::read_uint32()
{
    uint_fast32_t res = 0;
    uint8_t buf[4];
    file.read((char*) buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 4; ++i, shift += 8) {
        res |= static_cast<uint_fast32_t>(buf[i]) << shift;
    }

    if (!file.good()) {
        throw std::runtime_error("Error reading uint32");
    }
    return res;
}

string Catalog::read_string()
{
    uint_fast32_t len = read_uint32();
    char* buf = new char[len];
    file.read(buf, len);
    string res(buf, len);
    delete[] buf;
    return res;
}

vector<string> Catalog::read_strvec()
{
    vector<string> ret;
    uint64_t size = read_uint32();
    for (uint64_t i = 0; i < size; ++i) {
        ret.push_back(read_string());
    }
    return ret;
}

boost::unordered_flat_map<uint64_t, uint64_t> Catalog::read_map()
{
    boost::unordered_flat_map<uint64_t, uint64_t> res;
    auto size = read_uint64();
    for (uint_fast32_t i = 0; i < size; i++) {
        auto k = read_uint64();
        auto v = read_uint64();
        res.insert({ k, v });
    }
    return res;
}

void Catalog::write_uint8(const uint8_t n)
{
    file.put(static_cast<char>(n));
}

void Catalog::write_uint64(const uint64_t n)
{
    uint8_t buf[8];
    for (unsigned int i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}

void Catalog::write_uint32(const uint_fast32_t n)
{
    uint8_t buf[4];
    for (unsigned int i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}

void Catalog::write_string(const string& s)
{
    write_uint32(s.size());
    file.write(s.c_str(), s.size());
}

void Catalog::write_strvec(const vector<string>& strvec)
{
    write_uint32(strvec.size());
    for (const auto& str : strvec) {
        write_string(str);
    }
}

void Catalog::write_map(const boost::unordered_flat_map<uint64_t, uint64_t>& map)
{
    write_uint64(map.size());
    for (auto&& [k, v] : map) {
        write_uint64(k);
        write_uint64(v);
    }
}

boost::unordered_flat_map<string, uint64_t> Catalog::convert_strvec_to_map(const vector<string>& strvec)
{
    boost::unordered_flat_map<string, uint64_t> res;

    for (size_t i = 0; i < strvec.size(); ++i) {
        res[strvec[i]] = i;
    }

    return res;
}
