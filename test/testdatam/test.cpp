#include<iostream>
#include"cloud_backup.hpp"

void test1(){
    cloud_sys::DataManagement data_manage;
    data_manage.Insert("a.txt", "a.txt");
    data_manage.Insert("b.txt", "b.txt.gz");
    data_manage.Insert("c.txt", "c.txt");
    data_manage.Insert("d.txt", "d.txt.gz");
    data_manage.Storage();
}
void test2(){
    cloud_sys::DataManagement data_manage;
    data_manage.InitLoad();
    vector<string> list;
    data_manage.GetAllFileName(&list);
    cout << "所有文件列表\n";
    for(auto& i : list){
        cout << i << endl;
    }
    list.clear();
    data_manage.UncompressList(&list);
    cout << "未压缩文件列表\n";
    for(auto& i : list){
        cout << i << endl;
    }
}
int main(){
    //test1();
    test2();
    return 0;
}
