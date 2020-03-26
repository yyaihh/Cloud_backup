#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<unordered_map>
#include<zlib.h>
#include<boost/filesystem.hpp>
#include<pthread.h>
#include"httplib.h"

using namespace std;
namespace cloud_sys{

class FileUtil{ //文件工具类
public:
    static bool Read(const string& name, string* body);//从文件中读取内容
    static bool Write(const string& name, const string& body);//向文件中写入数据
};

class CompressUtil{//压缩工具类
public:
    static bool Compress(const string& src, const string& dst);//压缩(文件名, 压缩包名)
    static bool UnCompress(const string& src, const string& dst);//解压(压缩包名, 文件名)
};

class DataManagement{ //数据管理模块
    string m_back_file;//保存在磁盘中的文件列表文件的名称
    unordered_map<string, string> m_file_list;//数据管理容器
    pthread_rwlock_t m_rwlock;
public:
    DataManagement(const string& path):m_back_file(path){
        pthread_rwlock_init(&m_rwlock, NULL);
    }
    ~DataManagement(){
        pthread_rwlock_destroy(&m_rwlock);
    }
    bool IsExists(const string& name);//判断文件是否存在
    bool IsCompress(const string& name);//判断文件是否已经压缩
    bool UncompressList(vector<string>* list);//获取未压缩文件列表
    bool Insert(const string& src, const string& dst);//插入/更新数据
    bool GetAllFileName(vector<string>* list);
    bool Storage();//数据更新/改变后保存到磁盘
    bool InitLoad();//程序启动时加载磁盘中的数据
};

class NotHot{
    string m_ComFilePath;//压缩后文件在磁盘中的存储路径

    bool IsNotHot(const string& name);//判断文件是否为热点文件
public:
    bool Start();//开始压缩接口
};
class Server{
    string m_FileBackupPath;//上传的备份文件在磁盘中的路径
    httplib::Server m_server;
    //文件上传处理函数
    static void FileUpload(const httplib::Request& req, httplib::Response& resp);
    //文件列表请求处理函数
    static void List(const httplib::Request& req, httplib::Response& resp);
    //文件下载处理函数
    static void Download(const httplib::Request& req, httplib::Response& resp);
public:
    bool Start();//启动网络通信模块接口
};

bool FileUtil::Read(const string& name, string* body){
    ifstream fin(name, ifstream::binary);
    if(fin.is_open() == false){
        cout << "open file " << name << " failed!\n";
        return false;
    }
    int64_t fsize = boost::filesystem::file_size(name);//获取文件大小
    body->resize(fsize);
    if(fin.good() == false){
        cout << "file " << name << " read data failed!\n";
    }
    fin.close();
    return true;
}

bool FileUtil::Write(const string& name, const string& body){
    ofstream fout(name, ofstream::binary);
    //输出流ofstream, 默认打开文件会清空原有内容
    if(fout.is_open() == false){
        cout << "open file " << name << " failed!\n";
        return false;
    }
    fout.write(&body[0], body.size());
    if(fout.good() == false){
        cout << "file " << name << " write data failed!\n";
    }
    fout.close();
    return true;
}

bool CompressUtil::Compress(const string& src, const string& dst){
    string body;
    FileUtil::Read(src, &body);
    gzFile gf = gzopen(dst.c_str(), "wb");//大
    if(gf == nullptr){
        cout << "open file " << dst << " failed!\n";
        return false;
    }
    size_t wlen = 0;
    int ret;
    while(wlen < body.size()){
        ret = gzwrite(gf, &body[0], body.size() - wlen);
        if(wlen == 0){
            cout << "file " << dst << " write compress data failed!\n";
            return false;
        }
        wlen += ret;
    }
    gzclose(gf);
    return true;
}
bool CompressUtil::UnCompress(const string& src, const string& dst){
    ofstream fout(dst, ofstream::binary);
    if(fout.is_open() == false){
        cout << "open file " << src << " failed!\n";
        return false;
    }
    gzFile gf = gzopen(src.c_str(), "rb");
    if(gf == nullptr){
        cout << "open file " << src << " failed!\n";
        return false;
    }
    char tmp[4096] = {0};
    int ret;
    while((ret = gzread(gf, tmp, 4096)) > 0){
        fout.write(tmp, ret);
    }
    fout.close();
    gzclose(gf);
    return true;
}

}

