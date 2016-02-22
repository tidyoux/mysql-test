//
//  main.cpp
//  mysql_test
//
//  Created by haitao on 16/2/19.
//  Copyright © 2016年 artisangame. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "MySqlWrapper.h"

#define log(s) \
    std::cout << s << std::endl;

int main(int argc, const char * argv[]) {
    MySqlWrapper sql;
    if(sql.connect())
    {
        // list
        const int rowCount = sql.Query("call get_all_data();");
        log("row count: " << rowCount);
        for (int i = 0; i < rowCount; i++)
        {
            sql.GetRow();
            const int fieldCount = sql.getRowCount();
            
            int id = 0;
            std::string name;
            sql.GetField(id, 0);
            sql.GetField(name, 1);
            log(fieldCount << "|row index: " << i << ", id: " << id << ", name: " << name);
        }
        
        // add
        std::ostringstream oss;
        oss << "call add_item("
        << "\'" << "name" << time(0) << "\'" << ");";
        sql.Query(oss.str().c_str());
        
        // remove
        oss.str("");
        oss.clear();
        oss << "call remove_item("
        << 1 << ");";
        sql.Query(oss.str().c_str());
        
        // update
        oss.str("");
        oss.clear();
        oss << "call update_item("
        << 36 << ", \'a_new_name\'" << ");";
        sql.Query(oss.str().c_str());
    }
    
    return 0;
}
