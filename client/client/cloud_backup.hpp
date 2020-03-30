#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<unordered_map>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>
#include"httplib.h"
//#include<openssl/md5.h>
using namespace std;
#define STORAGE_FILE "./list.backup"


class FileUtil { //文件工具类
public:
	static bool Read(const string& name, string* body);//从文件中读取内容
	static bool Write(const string& name, const string& body);//向文件中写入数据
};

class DataManager {//数据管理模块
	string m_stroe_filename;//磁盘中持久化存储文件信息的文件名称
	unordered_map<string, string> m_backup_list;
public:
	DataManager(const string& filename) : m_stroe_filename(filename) {}
	bool Insert(const string& key, const string& val);
	bool GetEtag(const string& filename, string* etag);//通过文件名获取文件原来的etag
	bool Storage();//持久化存储
	bool InitLoad();//程序初始化时加载原有数据
};

class CloudClient {//目录监控模块
	string m_listen_dir;//所要监控的目录名称
	string m_server_ip;
	uint16_t m_server_port;
public:
	CloudClient(const string path, const string ip, const uint16_t port) :
		m_listen_dir(path),
		m_server_ip(ip),
		m_server_port(port)
	{}
	bool GetBackupFileList(vector<string>* list);//获取需要备份的文件列表
	bool GetEtag(const string& pathname, string* etag);//计算文件的Etag信息
	void Start();//文件备份流程
};

bool FileUtil::Read(const string& name, string* body) {
	ifstream fin(name, ifstream::binary);
	if (fin.is_open() == false) {
		cout << "open file " << name << " failed!\n";
		return false;
	}
	int64_t fsize = boost::filesystem::file_size(name);//获取文件大小
	body->resize(fsize);
	fin.read(&((*body)[0]), fsize);
	if (fin.good() == false) {
		cout << "file " << name << " read data failed!\n";
	}
	fin.close();
	return true;
}

bool FileUtil::Write(const string& name, const string& body) {
	ofstream fout(name, ofstream::binary);
	//输出流ofstream, 默认打开文件会清空原有内容
	if (fout.is_open() == false) {
		cout << "open file " << name << " failed!\n";
		return false;
	}
	fout.write(&body[0], body.size());
	if (fout.good() == false) {
		cout << "file " << name << " write data failed!\n";
	}
	fout.close();
	return true;
}

bool DataManager::Insert(const string& key, const string& val) {
	m_backup_list[key] = val;
	Storage();
	return true;
}

bool DataManager::GetEtag(const string& key, string* etag) {
	auto it = m_backup_list.find(key);
	if (it == m_backup_list.end()) {
		return false;
	}
	return true;
}

bool DataManager::Storage() {
	stringstream buf;
	for (auto& i : m_backup_list) {
		buf << i.first << ' ' << i.second << endl;
	}
	return FileUtil::Write(m_stroe_filename, buf.str());
}

bool DataManager::InitLoad() {
	string body;
	if (FileUtil::Read(m_stroe_filename, &body) == false) {
		return false;
	}
	vector<string> list;
	boost::split(list, body, boost::is_any_of("\n"), boost::token_compress_on);
	size_t pos;
	for (auto& i : list) {
		pos = i.find(' ');
		if (pos == string::npos) {
			continue;
		}
		string first = i.substr(0, pos);
		string second = i.substr(pos + 1);
		Insert(first, second);
	}
	return true;
}

DataManager g_data_manage(STORAGE_FILE);

bool CloudClient::GetBackupFileList(vector<string>* list) {
	//进行目录监控, 获取指定目录下的所有文件名称
	//opendir()//系统调用接口
	//readdir()//系统调用接口
	//scandir()C库
	//使用boost库
	boost::filesystem::directory_iterator begin(m_listen_dir);
	boost::filesystem::directory_iterator end;
	for (auto it = begin; it != end; ++it) {
		if (boost::filesystem::is_directory(it->status())) {//是目录
			continue;
		}
		string pathname = begin->path().string();
		string name = begin->path().filename().string();
		string cur_etag, old_etag;
		GetEtag(pathname, &cur_etag);
		g_data_manage.GetEtag(name, &old_etag);
		if (cur_etag != old_etag) {
			list->push_back(name);
		}
	}
	return true;
}
bool CloudClient::GetEtag(const string& pathname, string* etag) {
	//文件大小
	int64_t fsize = boost::filesystem::file_size(pathname);//文件最后一次修改时间
	//文件最后一次修改时间
	time_t mtime = boost::filesystem::last_write_time(pathname);
	*etag = to_string(fsize) + '-' + to_string(mtime);
	return true;
}
void CloudClient::Start() {
	g_data_manage.InitLoad();
	vector<string> list;
	GetBackupFileList(&list);
	for (auto& name : list) {
		string pathname = m_listen_dir + name;
		string body;
		FileUtil::Read(pathname, &body);
		httplib::Client client(m_server_ip, m_server_port);
		string req_path = '/' + name;
		auto rsp = client.Put(req_path.c_str(), body, "application/octet-stream");
		if (rsp == nullptr || rsp->status != 200) {
			cout << "fail" << name << "backup failed\n";
			continue;//失败了等下次循环还会继续备份
		}
		string etag;
		GetEtag(pathname, &etag);
		g_data_manage.Insert(name, etag);//更新
		Sleep(1000);
	}
}