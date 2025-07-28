// Copyright (C) 2024 Gilles Degottex <gilles.degottex@gmail.com> All Rights Reserved.
//
// You may use, distribute and modify this code under the
// terms of the Apache 2.0 license.
// If you don't have a copy of this license, please visit:
//     https://github.com/gillesdegottex/cpptopdb

#ifndef CPPTOPDB_H_
#define CPPTOPDB_H_

#include <mutex>
#include <deque>
#include <typeindex>
#include <typeinfo>
#include <filesystem>
namespace fs = std::filesystem;

#define CPPTOPDB_CPP std::map<std::string,std::string> g_cpptopdb_names2types; std::mutex g_cpptopdb_access;
extern std::map<std::string,std::string> g_cpptopdb_names2types;
extern std::mutex g_cpptopdb_access;

namespace cpptopdb {

    inline void clear() {
        std::lock_guard<std::mutex> guard(g_cpptopdb_access);
        g_cpptopdb_names2types.clear();
    }

    inline void update() {
        if (!fs::is_directory("cpptopdb") || !fs::exists("cpptopdb")) {
            fs::create_directory("cpptopdb");
        }
        std::ofstream outfile("cpptopdb/cpptopdb.json");
        outfile << "{";
        outfile << "\"variables\":[";
        int n = 0;
        for (const auto& kv : g_cpptopdb_names2types) {
            if (n > 0)
                outfile << ",";
            outfile << "{\"varname\":\"" << kv.first << "\",\"dtype\":\"" << kv.second << "\"}";
            n++;
        }
        outfile << "]";
        outfile << "}" << std::endl;
    }

    // Matches any container array_type that has a value_type member
    template<typename array_type>
    inline void add(const array_type& var, const std::string& varname) {
        std::lock_guard<std::mutex> guard(g_cpptopdb_access);
        if (!fs::is_directory("cpptopdb") || !fs::exists("cpptopdb")) {
            fs::create_directory("cpptopdb");
        }
        phaseshift::dev::binaryfile_write("./cpptopdb/"+varname+".bin", var);

        std::string type;
        if (std::is_same<typename array_type::value_type, std::complex<float>>::value) {
            type = "complex64";
        } else if (std::is_same<typename array_type::value_type, std::complex<double>>::value) {
            type = "complex128";
        } else if (std::is_same<typename array_type::value_type, float>::value) {
            type = "float32";
        } else if (std::is_same<typename array_type::value_type, double>::value) {
            type = "float64";
        } else {
            type = "unknown";
            // std::cerr << "cpptopdb: Warning: unknown value_type for variable '" << varname << "' of type " << std::type_index(typeid(typename array_type::value_type)).name() << std::endl;
        }
        g_cpptopdb_names2types.insert(make_pair(varname, type));
        update();
    }

    inline void print() {
        std::lock_guard<std::mutex> guard(g_cpptopdb_access);
        std::cerr << "cpptopdb: variables:" << std::endl;
        for (const auto& kv : g_cpptopdb_names2types) {
            std::cerr << "    " << kv.first << " [" << kv.second << "]" << std::endl;
        }
    }

    //! Wait for an Enter key press
    inline void wait_info(std::string file, size_t line) {
        print();
        std::cerr << "cpptopdb: waiting at " << fs::path(file).filename().string() << ":" << line << ", press Enter to continue." << std::endl;
        std::cin.get();
    }
    inline void wait() {
        print();
        std::cerr << "cpptopdb: waiting, press Enter to continue." << std::endl;
        std::cin.get();
    }
}

#ifdef CPPTOPDB_ENABLE
    #define cpptopdb_clear()	        cpptopdb::clear();
    #define cpptopdb_add(VAR, VARNAME)	cpptopdb::add(VAR, VARNAME);
    #define cpptopdb_wait()	            cpptopdb::wait_info(__FILE__, __LINE__);
#else
    #define cpptopdb_clear()
    #define cpptopdb_add(VAR, VARNAME)
    #define cpptopdb_wait()
#endif

#endif // CPPTOPDB_H_
