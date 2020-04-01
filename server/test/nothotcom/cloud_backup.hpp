#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<unistd.h>
#include<zlib.h>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>
#include<pthread.h>
#include"httplib.h"
using namespace std;

#define BACKUP_PATH "./backup/"
#define GZBACKUP_PATH "./gzbackup/"
#define LIST_PATHNAME "./list"
namespace cloud_sys{
#define NOTHOT_TIME 10 //最后一次访问时间在10秒以上的
#define INTERVAL_TIME 10 //非热点检测每30秒一次

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
    unordered_map<string, string> m_file_list;//数据管理容器
    pthread_rwlock_t m_rwlock;
public:

    DataManagement(){
        pthread_rwlock_init(&m_rwlock, NULL);
    }
    ~DataManagement(){
        pthread_rwlock_destroy(&m_rwlock);
    }
    bool IsExists(const string& name);//判断文件是否存在
    bool IsCompress(const string& name);//判断文件是否已经压缩
    bool UncompressList(vector<string>* list);//获取未压缩文件列表
    bool GetGzName(const string& src, string* dst);
    bool Insert(const string& src, const string& dst);//插入/更新数据
    bool GetAllFileName(vector<string>* list);
    bool Storage();//数据更新/改变后保存到磁盘
    bool InitLoad();//程序启动时加载磁盘中的数据
};

class NotHot{
    bool IsNotHot(const string& name);//判断文件是否为热点文件
public:
    bool Start();//开始压缩接口
    string getComFilePath();
};

class Server{
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
    fin.read(&((*body)[0]), fsize);
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
        cout << "gzopen file " << dst << " failed!\n";
        return false;
    }
    size_t wlen = 0;
    int ret;
    while(wlen < body.size()){
        ret = gzwrite(gf, &body[0], body.size() - wlen);
        if(ret == 0){
            cout << "file " << dst << " gzwrite compress data failed!\n";
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

bool DataManagement::IsExists(const string& name){//判断文件是否存在
    pthread_rwlock_rdlock(&m_rwlock);
    if(m_file_list.find(name) == m_file_list.end()){
        pthread_rwlock_unlock(&m_rwlock);
        return false;
    }
    pthread_rwlock_unlock(&m_rwlock);
    return true;
}

bool DataManagement::IsCompress(const string& name){
    pthread_rwlock_rdlock(&m_rwlock);
    auto it = m_file_list.find(name);
    if(it == m_file_list.end()){//不存在
        pthread_rwlock_unlock(&m_rwlock);
        return false;
    }
    if(it->first == it->second){
        pthread_rwlock_unlock(&m_rwlock);
        return false;
    }
    pthread_rwlock_unlock(&m_rwlock);
    return true;
}

bool DataManagement::UncompressList(vector<string>* list){
    pthread_rwlock_rdlock(&m_rwlock);
    for(auto& e : m_file_list){
        if(e.first == e.second){
            list->push_back(e.first);
        }
    }
    pthread_rwlock_unlock(&m_rwlock);
    return true;
}

bool DataManagement::Insert(const string& src, const string& dst){
    pthread_rwlock_wrlock(&m_rwlock);
    m_file_list[src] = dst;
    pthread_rwlock_unlock(&m_rwlock);
    Storage();//插入新数据就保存一下//可以优化一下
    return true;
}

bool DataManagement::GetGzName(const string& src, string* dst){
    auto it = m_file_list.find(src);
    if(it == m_file_list.end()){
        return false;
    }
    *dst = it->second;
    return true;
}
bool DataManagement::GetAllFileName(vector<string>* list){
    pthread_rwlock_rdlock(&m_rwlock);
    list->reserve(m_file_list.size());
    for(auto& e : m_file_list){
        list->push_back(e.first);
    }
    pthread_rwlock_unlock(&m_rwlock);
    return true;
}

bool DataManagement::Storage(){//持久化存储
    stringstream tmp;
    pthread_rwlock_wrlock(&m_rwlock);
    for(auto& e : m_file_list){
        tmp << e.first << ' ' << e.second << "\r\n";
    }
    pthread_rwlock_unlock(&m_rwlock);
    FileUtil::Write(LIST_PATHNAME, tmp.str());
    return true;
}

bool DataManagement::InitLoad(){//启动时从磁盘加载数据
    string buf;
    if(boost::filesystem::exists(BACKUP_PATH) == false){
        boost::filesystem::create_directory(BACKUP_PATH);               
    }
    if(boost::filesystem::exists(GZBACKUP_PATH) == false){
         boost::filesystem::create_directory(GZBACKUP_PATH);            
    }
    if(FileUtil::Read(LIST_PATHNAME, &buf) == false){
        cout << "尝试生成" << LIST_PATHNAME << endl; 
        if(FileUtil::Write(LIST_PATHNAME,"") == false){
            cout << "生成失败!\n";
            return false;
        }
        cout << "生成成功\n";
    }

    //boost::split(vector<string>& list, string& str, " ", boost::token_compress_off);
    //boost::token_compress_on
    vector<string> list;
    boost::split(list, buf, boost::is_any_of("\r\n"), boost::token_compress_on);
    size_t pos;
    for(auto& s : list){
        pos = s.find(' ');
        if(pos == string::npos){
            continue;
        }
        string first = s.substr(0, pos);
        string second = s.substr(pos + 1);
        Insert(first, second);
    }
    return true;
}

DataManagement g_data_manage;
bool NotHot::Start(){
    //需要一个循环, 是一个持续的过程, 每隔一段时间判断一下
    if(g_data_manage.InitLoad() == false){
        cout << "InitLoad failed!\n";
        return false;
    }
    while(1){
        cout << "检测一次\n";
        vector<string> list;
        g_data_manage.UncompressList(&list);
        cout << "有" << list.size() << "个未压缩文件\n";
        for(auto& i : list){
            if(IsNotHot(BACKUP_PATH + i)){
                string src_name_path = BACKUP_PATH + i;
                string dst_name_path = GZBACKUP_PATH + i;
                dst_name_path += ".gz";
                if(CompressUtil::Compress(src_name_path, dst_name_path)){
                    g_data_manage.Insert(i, i + ".gz");//更新
                    unlink(src_name_path.c_str());
                }
            }
        }
        sleep(INTERVAL_TIME);
    }
    return true;
}

bool NotHot::IsNotHot(const string& name){
    //当前时间减去最后一次访问时间 > n 秒的文件是非热点
    time_t cur = time(0);
    struct stat st;
    if(stat(name.c_str(), &st) < 0){
        perror("stat error :");
        return false;
    }
    if((cur - st.st_atim.tv_sec) > NOTHOT_TIME){
        return true;
    }
    return false;
}

bool Server::Start(){
    m_server.Put("/(.*)", FileUpload);
    m_server.Get("/list", List);
    m_server.Get("/download/(.*)", Download);

    m_server.listen("0.0.0.0", 9000);//搭建tcp服务器, 进行http数据接收处理以及响应
    return 0;
}

//req.method 解析出的请求方法
//req.path 解析出的请求的资源路径, 就是url中域名/之后的一部分
//req.headers 请求的头部信息键值对
//req.body 请求的正文数据

void Server::FileUpload(const httplib::Request& req, httplib::Response& resp){
    string pathname = BACKUP_PATH;
    pathname += req.matches[1];
    //pathname : 路径+文件名, matches[1]就是需要备份的文件名
    FileUtil::Write(pathname, req.body);
    g_data_manage.Insert(req.matches[1], req.matches[1]);
    resp.status = 200;//默认就是200, 不写也行
}

void Server::List(const httplib::Request& req, httplib::Response& resp){
    vector<string> list;
    g_data_manage.GetAllFileName(&list);
    stringstream buf;
    buf << "<html><body><hr />";
    for(auto& i : list){
        buf << "<a href='/download/" << i << "'>" << i << "</a>";
        buf << "<hr />";
        //buf << "<a href='/download/a.txt'> a.txt </a";
    }
    buf << "<hr /></body></html>";
    resp.set_content(buf.str().c_str(), buf.str().size(), "text/html");
    //resp.body = buf.str();
    //resp.set_header("Content-Type", "text/html") ;
    resp.status = 200;
}

void Server::Download(const httplib::Request& req, httplib::Response& resp){
    if(g_data_manage.IsExists(req.matches[1]) == false){
        resp.status = 404;
        return;
    }
    string pathname = BACKUP_PATH;
    pathname += req.matches[1];
    if(g_data_manage.IsCompress(req.matches[1]) == true){//文件已经被压缩
        string gzname;
        g_data_manage.GetGzName(req.matches[1], &gzname);
        string gzpathname = GZBACKUP_PATH + gzname;
        CompressUtil::UnCompress(gzpathname, pathname);
        g_data_manage.Insert(req.matches[1], req.matches[1]);
        unlink(gzpathname.c_str());
    }
    FileUtil::Read(pathname, &resp.body);
    resp.set_header("Content-Type", "application/octet-stream");
    resp.status = 200;
}

}

