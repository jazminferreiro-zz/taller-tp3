#include "serverIndex.h"
#include <map> 
#include <string> 
#include <set> 


Index::Index(const std::string & index_file_name){
    this->index_file_name = index_file_name;
    std::ifstream index_file(this->index_file_name);
    if (!index_file.is_open()){
        throw Error("Unable to open index file %s\n",//
            this->index_file_name.c_str());
    }
    std::string line;
    std::string type;
    std::string name;
    std::string hash;
    while (std::getline(index_file,line,';')){
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    	std::istringstream linereader(line, std::ios::binary);
    	std::map<std::string,std::set<std::string,cmp>> * map;
    	if (!std::getline(linereader, type, ' ')){
    		break;
    	}		
        if (type.compare(FILE_TYPE) == 0){
            map = &files;
        } else if (type.compare(TAG_TYPE) == 0){
            map = &tags;
        } else{
            throw Error("reading input file: "//
            	"%s is not valid type\n" // 
            	"shuld be %s for files and %s for tags\n",//
            	type.c_str(), FILE_TYPE,TAG_TYPE);
        }
            
        std::getline(linereader, name, ' ');
        std::set<std::string,cmp> hashes;

        while (std::getline(linereader, hash, ' ')){
           	hashes.insert(hash);
    	}
    		
    	(*map)[name] = hashes;
    }
}

void Index::add_file_hash(const std::string & file_name, //
                                const std::string & hash){
    Lock l(file_mutex);
    std::set<std::string,cmp> hashes; 

    std::map<std::string, std::set<std::string,cmp> >::iterator //
     it = files.find(file_name);
    if (it != files.end()){
         hashes = it->second;
    }
    hashes.insert(hash);

    files[file_name] = hashes;
}

bool Index::does_hash_exist(const std::string &  hash){
    Lock l(file_mutex);
    for (std::map<std::string,std::set<std::string,cmp>>::iterator//
    map_iter=files.begin(); map_iter!=files.end(); ++map_iter){
        if (map_iter->second.find(hash) != map_iter->second.end()){
            return true;
        }
    }
    return false;   
}


std::set<std::string,cmp> Index::get_tag_files(const std::string & tag_name){
    Lock l(tag_mutex);
    std::map<std::string, std::set<std::string,cmp> >::iterator //
     it = tags.find(tag_name);
    if (it == tags.end()){
        std::set<std::string,cmp> empty_set;
        return empty_set;
    }
    return it->second;
}

std::string Index::get_file_name(const std::string & hash){
    Lock l(file_mutex);
    for (std::map<std::string,std::set<std::string,cmp>>::iterator//
    map_iter=files.begin(); map_iter!=files.end(); ++map_iter){
        if (map_iter->second.find(hash) != map_iter->second.end()){
            return map_iter->first;
        }
    }
    throw Error("invalid hash"); 
}

void Index::add_tag_hash(const std::string & tag_name,//
                             const std::string & hash){
    Lock l(tag_mutex);
    std::set<std::string,cmp> hashes; 

    std::map<std::string, std::set<std::string,cmp>>::iterator //
     it = tags.find(tag_name);
    if (it != tags.end()){
         hashes = it->second;
    }
    hashes.insert(hash);

    tags[tag_name] = hashes;
}



void Index::write_index_file(std::map<std::string,//
    std::set<std::string,cmp>> & map, //
    const std::string & type){
    std::ofstream index_file(this->index_file_name,ios::out | ios::app);
    
    if (!index_file.is_open()){
        throw Error("Unable to open file %s\n",this->index_file_name);
    }
    for (std::map<std::string,std::set<std::string,cmp>>::iterator//
    map_iter=map.begin(); map_iter!=map.end(); ++map_iter){
        index_file  << type <<" "<<(*map_iter).first;

        for (std::set<std::string>::iterator//
            set_iter=(*map_iter).second.begin();//
            set_iter!=(*map_iter).second.end();//
            ++set_iter){
            index_file  <<  " " << (*set_iter);   
        }
        index_file  << " ;\n";   
    }
    index_file.close();
}


void Index::close(){
    std::ofstream index(this->index_file_name,ios::out | ios::trunc);
    index.close();

    write_index_file(files, "f");
    write_index_file(tags, "t");    
}
