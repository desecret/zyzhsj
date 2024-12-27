#include <bits/stdc++.h>
using namespace std;

#define DES_KEY "133457799BBCDFF1"

/**

 *数据初始置换表

 */
int T1[8][8]={58,50,42,34,26,18,10,2,
              60,52,44,36,28,20,12,4,
              62,54,46,38,30,22,14,6,
              64,56,48,40,32,24,16,8,
              57,49,41,33,25,17,9,1,
              59,51,43,35,27,19,11,3,
              61,53,45,37,29,21,13,5,
              63,55,47,39,31,23,15,7};
/**

 *密钥初始置换表

 */
int T2[8][7]={57,49,41,33,25,17,9,
              1,58,50,42,34,26,18,
              10,2,59,51,43,35,27,
              19,11,3,60,52,44,36,
              3,55,47,39,31,23,15,
              7,62,54,46,38,30,22,
              14,6,61,53,45,37,29,
              21,13,5,28,20,12,4};

/**

 *密钥循环左移位数表

 */
int T3[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

/**

 *密钥压缩置换表

 */
int T4[8][6]={14,17,11,24,1,5,
              3,28,15,6,21,10,
              23,19,12,4,26,8,
              16,7,27,20,13,2,
              41,52,31,37,47,55,
              30,40,51,45,33,48,
              44,49,39,56,34,53,
              46,42,50,36,29,32};

/**

 *数据扩展表

 */
int T5[8][6]={32,1,2,3,4,5,
              4,5,6,7,8,9,
              8,9,10,11,12,13,
              12,13,14,15,16,17,
              16,17,18,19,20,21,
              20,21,22,23,24,25,
              24,25,26,27,28,29,
              28,29,30,31,32,1};

/**

 *S盒置换表

 */
int S[8][4][16]={{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
                {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
                {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
                {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
                {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
                {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
                {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};

/**

 *P盒置换表

 */
int P[4][8]={16,7,20,21,
             29,12,28,17,
             1,15,23,26,
             5,18,31,10,
             2,8,24,14,
             32,27,3,9,
             19,13,30,6,
             22,11,4,25};


/**

 *最终置换表

 */
int T6[8][8]={  40,8,48,16,56,24,64,32,
                  39,7,47,15,55,23,63,31,
                  38,6,46,14,54,22,62,30,
                  37,5,45,13,53,21,61,29,
                  36,4,44,12,52,20,60,28,
                  35,3,43,11,51,19,59,27,
                  34,2,42,10,50,18,58,26,
                  33,1,41,9,49,17,57,25};

/**

 *最终置换函数 64位->64位

 *函数说明：s为完成最后一轮循环得到的64为数据

 *返回值为密文或明文

 */
string final_permutation(string s)
{
    string rs="";
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            rs+=s[T6[i][j]-1];
        }
    }
    return rs;
}

/**

 *P盒置换函数 32位->32位

 *函数说明：s为S盒的输出

 */
string P_box(string s)
{
    string rs="";
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<8;j++)
        {
            rs+=(s[P[i][j]-1]);
        }
    }
    return rs;
}

/**

 *S盒置换函数 48位->32位

 *函数说明：s为48位数据

 *返回值为32位

 */
string S_box(string s)
{
    string rs="";
    string s1;
    int k1,k2;//S盒的行号和列号
    int h=1;//决定使用那个S盒
    for(int i=0;i<=42;i=i+6,h++)
    {
        k1=(s[i]-'0')*2+(s[i+5]-'0')*1;
        k2=(s[i+1]-'0')*8+(s[i+2]-'0')*4+(s[i+3]-'0')*2+(s[i+4]-'0')*1;
        int x=S[h-1][k1][k2];
        s1="";
        int y=8;
        for(int j=1;j<=4;j++)
        {
            if(x<y)
            {
                s1+="0";
                y/=2;
            }else
            {
                s1+="1";
                x=x%y;
                y/=2;
            }
        }
        rs+=s1;
    }
    return rs;
}

/**

 *异或运算函数

 *要求位数相同

 */
string XOR(string s1,string s2)
{
    string rs="";
    for(int i=0;i<s1.length()&&i<s2.length();i++)
    {
        rs+=((s1[i]-'0')^(s2[i]-'0'))+'0';
    }
    return rs;
}

/**

 *数据扩展函数 32->48

 *函数说明：s为数据的右半部分 32位

 *扩展成48位的输出

 */
string plaintext_righthalf_extended_permutation(string s)
{
    string rs="";
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
            rs+=s[T5[i][j]-1];
        }
    }
    return rs;
}

/**

 *密钥压缩置换函数 56位->48位

 *函数说明：s为56为的密钥

 *输出为48位的子密钥

 */
string secret_key_compression_replacement(string s)
{
    string rs="";
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<6;j++)
        {
            rs+=s[T4[i][j]-1];
        }
    }
    return rs;
}

/**

 *密钥循环左移函数 56位->56位

 *函数说明：k为左移位数 s为密钥

 *返回值位数不变

 */
string secret_ket_left_move(int k,string s)//密钥循环左移k位
{
    string s1=s.substr(0,28);
    string s2=s.substr(28,28);
    string rs=s1.substr(k,28-k)+s1.substr(0,k)+s2.substr(k,28-k)+s2.substr(0,k);
    return rs;
}

/**

 *密钥初始置换函数 64位->58位

 *函数说明：s为64位的初始密钥

 *返回值为58位

 */
string secret_key_initial_permutation(string s)
{
    string rs="";
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<7;j++)
        {
            rs+=s[T2[i][j]-1];
        }
    }
    return rs;
}

/**

 *明文初始置换函数 64位->64位

 *函数说明：s为初始明文 64位

 *返回值为6位

 */
string plaintext_initial_permutation(string s)//明文初始置换
{
    string rs="";
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            rs+=s[T1[i][j]-1];
        }
    }
    return rs;
}

/**

 *16进制转2进制函数

 *函数说明：s为16进制字符串

 *返回为2进制字符串

 */
string H(string s)
{
    string s1;
    string rs="";
    for(int i=0;i<s.length();i++)
    {
        int x;
        if(s[i]>='0'&&s[i]<='9')
        {
            x=s[i]-'0';
        }else
        {
            x=s[i]-'A'+10;
        }
        s1="";
        int y=8;
        for(int j=1;j<=4;j++)
        {
            if(x<y)
            {
                y/=2;
                s1+="0";
            }else
            {
                s1+="1";
                x=x%y;
                y=y/2;
            }
        }
        rs+=s1;
    }
    return rs;
}

 /**

 *2进制转16进制函数

 *str为2进制字符串

 *返回值为16进制字符串

 */
string G(string str)
{
    string rs="";
    char temp;
    for(int i=0;i<=str.length()-4;i=i+4)
    {
        int x=(str[i]-'0')*8+(str[i+1]-'0')*4+(str[i+2]-'0')*2+str[i+3]-'0';

        if(x>=10)
        {
            temp=(char)(x-10+'A');
        }else
        {
            temp=(char)(x+'0');
        }
        rs+=temp;
    }
    return rs;
}

/**

 *封装函数f

 *函数说明：接收32位数据和48位的子密钥 产生一个32位的输出

 *str1:32位数据  str2:48位的子密钥

 *返回值32位

 */

string f(string str1,string str2)
{
    string expendR=plaintext_righthalf_extended_permutation(str1);

    string rs=XOR(expendR,str2);

    rs=S_box(rs);

    rs=P_box(rs);

    return rs;
}

/**

 *子密钥生成函数

 *函数说明：s为给定的密钥

 *生成16个子密钥

 */
string Keys[20];

void generateKeys(string s)
{
    s=secret_key_initial_permutation(s);

    for(int i=1;i<=16;i++)
    {
        s=secret_ket_left_move(T3[i-1],s);
        Keys[i]=secret_key_compression_replacement(s);
    }
}

/**

 *DES加密函数 64位->64位

 *函数说明：str1为64位的给定明文

 *返回值为64位的密文

 */
 string encrypt(string str1)
 {
     //第一步:明文初始置换 64->64
     str1=plaintext_initial_permutation(str1);

     //第二步:数据分组
     string left=str1.substr(0,32);
     string right=str1.substr(32,32);

     string newleft;

     //第三步:16轮迭代
     for(int i=1;i<=16;i++)
     {
         newleft=right;

         right=XOR(left,f(right,Keys[i]));

         left=newleft;

     }

     //第四步:合并数据 注意位R16L16
     string rs=right+left;

     //结尾置换
     rs=final_permutation(rs);
     return rs;

 }
 /**

 *解密函数

 *str为密文

 *输出明文

 */
 string decrypt(string str)
 {
     //把密文当作明文进行初始明文置换
     str=plaintext_initial_permutation(str);

     //分组
     string left=str.substr(0,32);
     string right=str.substr(32,32);
     

     string newleft;

     //逆序的子密钥使用 16轮迭代
     for(int i=16;i>=1;i--)
     {

         newleft=right;
         right=XOR(left,f(right,Keys[i]));
         left=newleft;

     }

     //合并
     string rs=right+left;

     //最后置换
     rs=final_permutation(rs);

     return rs;
 }

 string getDesEncryptResult(string str) {
    //进制转换 16->2
    str = H(str);
    //生成16个子密钥
    generateKeys(H(DES_KEY));
    //加密
    string rs = encrypt(str);
    return G(rs);
 }

// int main()
// {
//    //明文 16进制
//    string str1="0123456789ABCDEF";
//    //密钥16进制
//    string str2="133457799BBCDFF1";

//    string temp=str1;//保存十六进制的明文以便后面验证算法正确性

//    //进制转换 16->2
//    str1=H(str1);
//    str2=H(str2);
//    cout<<"二进制明文:"<<str1<<endl;
//    cout<<"二进制密文:"<<str2<<endl<<endl;

//    cout<<"16个子密钥:"<<endl;
//    //生成16个子密钥
//    generateKeys(str2);

//    //加密
//    string rs=encrypt(str1);
//    cout<<"密文（二进制）:"<<rs<<endl;

//    cout<<"密文（16进制）："<<G(rs)<<endl;

//    cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl<<"下面是解密过程数据"<<endl<<endl;

   //解密
//    rs=decrypt(rs);

//    cout<<"明文（二进制）:"<<rs<<endl;

//    //进制转换
//    rs=G(rs);

//    cout<<"明文（16进制）："<<rs<<endl;

//    if(temp==rs)
//     cout<<"解密得到的明文和初始明文匹配成功，算法正确"<<endl;
//    else
//     cout<<"解密得到的明文和初始明文匹配不成功，算法错误"<<endl;

//     getchar();
//    return 0;
// }