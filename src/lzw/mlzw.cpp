//
//  mlzw.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/9.
//

#include "mlzw.h"
int encoding(u_int8_t* unencoded_data,u_int8_t* encoded_data,int size)
{
    std::cout << "Encoding\n";
    std::map<std::string, uint8_t> table;
    for (int i = 0; i <= 255; i++) {
        std::string ch = "";
        ch += char(i);
        table[ch] = i&0xFF;
    }
 
    std::string p = "", c = "";
    p += unencoded_data[0];
    int code = 256;
    std::vector<int> output_code;
    std::cout << "String\tOutput_Code\tAddition\n";
    int flag = 0;
    for (int i = 0; i < size; i++) {
        if (i != size - 1)
            c += unencoded_data[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
//            std::cout << p << "\t" << table[p] << "\t\t"
//                 << p + c << "\t" << code << std::endl;
//            output_code.push_back(table[p]);
            encoded_data[flag++] = table[p];
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
//    cout << p << "\t" << table[p] << endl;
//    output_code.push_back(table[p]);
    encoded_data[flag++] = table[p];
    return flag;
//    return output_code;
}
