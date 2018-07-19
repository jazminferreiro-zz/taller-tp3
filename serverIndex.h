#ifndef __INDEX_H__
#define __INDEX_H__

#define FILE_TYPE "f"
#define TAG_TYPE "t"

#include <fstream>
#include <string>
#include <sstream>  
#include <algorithm>
#include <map>
#include <set>
#include <mutex>
#include "commonError.h"
#include "serverLock.h"
using std::cout;
using std::ios;


struct cmp{
    bool operator()(const std::string& s1, const std::string& s2) const{
        for (std::string::const_iterator it1 = s1.begin(), it2 =  s2.begin();//
            it1!=s1.end() && it2!=s2.end(); ++it1 , ++it2 ){
            if ((*it1) == (*it2)){
                continue;
            }
            if (std::isdigit(*it1) && !std::isdigit(*it2)){
                return true;
            }
            if (!std::isdigit(*it1) && std::isdigit(*it2)){
               return false;
            }
            if (std::isdigit(*it1) && std::isdigit(*it2)) {
                int n1 = (*it1) -48;
                int n2 = (*it2) -48;
                return n1 < n2;
            }
        }
        return s1 < s2;
    }
};

class Index{
    private:
    std::string  index_file_name;
    std::mutex file_mutex; 
    std::mutex tag_mutex; 
    std::map<std::string,std::set<std::string,cmp>> files;
    std::map<std::string,std::set<std::string,cmp>> tags;

    bool numeric_string_compare(const std::string& s1, const std::string& s2);
    //recibe un mapa un caracter de typo y escribe en el archivo
    //de indice  los datos del mapa con el formato correspondiente
    void write_index_file(std::map<std::string,//
    std::set<std::string,cmp>> & map, //
    const std::string & type);

public:
    //abre el archivo index_file_name y crea un mapa de files 
    // y otro de tags, en los cuales la key es el nombre y 
    // el value es un set de hashes
    explicit Index(const std::string & index_file_name);


    //devuelve true si el hash existe dentro de algun set de hashes
    //de alguno de los archivos
    bool does_hash_exist(const std::string & hash);

    //devuelve el nombre del archivo que tiene ese hash 
    //dentro de su set de hashes
    std::string get_file_name(const std::string & hash);

    //agrega el nuevo hash para ese archivo
    //en el mapa de archivos
    //(si el file name no existia en el mapa lo agtrga)
    void add_file_hash(const std::string & file_name, const std::string & hash);

    //devuelve un set de archivos asociados a ese tag
    //si no existe devolvera un ser vacio
    std::set<std::string,cmp> get_tag_files(const std::string & tag_name);


    //agrega el nuevo hash para ese tag
    //en el mapa de tags
    //(si el tag no existia en el mapa lo agtrga)
    void add_tag_hash(const std::string & tag_name, const std::string & hash);

    
    //Sobre escribe el archivo llamado index_file_name   
    //con el mapa de files y el mapa de tags.
    //la version inicial se pierde.
    void close();
};

#endif

