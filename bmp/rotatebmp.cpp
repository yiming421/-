#include <fstream>
#include <iostream>
#include <string.h>
#include <bitset>
using std::cerr; using std::endl; using std::ifstream; using std::ofstream;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned int LONG;//Linux下long为8字节
typedef unsigned char BYTE;
typedef unsigned int FXPT2DOT30;

#pragma pack(1)//对齐宽度为1

typedef struct tagCIEXYZ
{
    FXPT2DOT30 ciexyzX ;
    FXPT2DOT30 ciexyzY ;
    FXPT2DOT30 ciexyzZ ;
}
CIEXYZ, * LPCIEXYZ ;

typedef struct tagCIEXYZTRIPLE {
    CIEXYZ ciexyzRed ;
    CIEXYZ ciexyzGreen ;
    CIEXYZ ciexyzBlue ;
}CIEXYZTRIPLE;

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct {
    DWORD        bV5Size;
    LONG         bV5Width;
    LONG         bV5Height;
    WORD         bV5Planes;
    WORD         bV5BitCount;
    DWORD        bV5Compression;
    DWORD        bV5SizeImage;
    LONG         bV5XPelsPerMeter;
    LONG         bV5YPelsPerMeter;
    DWORD        bV5ClrUsed;
    DWORD        bV5ClrImportant;
    DWORD        bV5RedMask;
    DWORD        bV5GreenMask;
    DWORD        bV5BlueMask;
    DWORD        bV5AlphaMask;
    DWORD        bV5CSType;
    CIEXYZTRIPLE bV5Endpoints;
    DWORD        bV5GammaRed;
    DWORD        bV5GammaGreen;
    DWORD        bV5GammaBlue;
    DWORD        bV5Intent;
    DWORD        bV5ProfileData;
    DWORD        bV5ProfileSize;
    DWORD        bV5Reserved;
} BITMAPV5HEADER, *LPBITMAPV5HEADER, *PBITMAPV5HEADER;
//以上为Windows.h中定义的与位图处理有关的结构
#pragma pack()//对齐宽度恢复正常

bool judgeBit(char* input){//判断为24位文件还是32位文件
    ifstream in(input, std::ios::binary);
    if(!in){
        cerr << "Cannot open the file" << endl;
        return 0;
    }
    in.seekg(sizeof(BITMAPFILEHEADER) + sizeof(DWORD) + 2 * sizeof(LONG) + sizeof(WORD));
    WORD biBitCount;
    in.read((char*)(&biBitCount), sizeof(WORD));
    in.close();
    if(biBitCount == 24) return 1;
    else return 0;
}

class BMP{//BMP基类
public:
    virtual void readBMP(char* input) = 0;//读入
    virtual void rotate() = 0;//旋转
    virtual void writeBMP(char* output) const = 0;//写入
protected:
    BITMAPFILEHEADER head;
    char* data = nullptr;//图像数据区
};

class BMP24:public BMP{
public:
    void readBMP(char* input){;
        ifstream in(input, std::ios::binary);
        if(!in){
            cerr << "Cannot open the file" << endl; 
            return;
        }
        in.read((char*)(&head), sizeof(head));
        in.read((char*)(&infoHead), sizeof(infoHead));
        int diff = 3 - (infoHead.biWidth * 3 - 1) % 4;//设置对齐
        infoHead.biSizeImage = infoHead.biHeight * (infoHead.biWidth * 3 + diff);//某些格式的BMP中，biSizeImage = 0,这里直接算出
        data = new char [infoHead.biSizeImage];
        in.read(data, infoHead.biSizeImage);
        in.close();
    }
    void rotate(){
        std::swap(infoHead.biWidth, infoHead.biHeight);
        LONG w = infoHead.biWidth, h = infoHead.biHeight;
        int diff = 3 - (h * 3 - 1) % 4, newdiff = 3 - (w * 3 - 1) % 4;
        infoHead.biSizeImage = h * (w * 3 + newdiff);
        head.bfSize = infoHead.biSizeImage + sizeof(head) + sizeof(infoHead);
        char* newdata = new char [infoHead.biSizeImage];
        for(int i = 0; i < h; ++i){
            for(int j = 0; j < w; ++j)
                for(int i1 = 0; i1 < 3; ++i1)
                    *(newdata + i * (w * 3 + newdiff) + j * 3 + i1) = *(data + j * (h * 3 + diff) + (h - i - 1) * 3 + i1);//旋转
            for(int j = 0; j < newdiff; ++j)
                *(newdata + i * (w * 3 + newdiff) + w * 3 + j) = 0;//补零
        }
        delete [] data;
        data = new char [infoHead.biSizeImage];
        memcpy(data, newdata, infoHead.biSizeImage);
    }
    void writeBMP(char* output) const{
        std::ofstream of(output, std::ios::binary);
        of.write((char*)(&head), sizeof(head));
        of.write((char*)(&infoHead), sizeof(infoHead));
        of.write(data, infoHead.biSizeImage);
        delete [] data;
        of.close();
    }
private:
    BITMAPINFOHEADER infoHead;
};

class BMP32:public BMP{
public:
    void readBMP(char* input){
        ifstream in(input, std::ios::binary);
        if(!in){
            cerr << "Cannot open the file" << endl; 
            return;
        }
        in.read((char*)(&head), sizeof(head));
        in.read((char*)(&infoHead), sizeof(infoHead));
        infoHead.bV5SizeImage = infoHead.bV5Width * infoHead.bV5Height * 4;//32位BMP一像素四个字节
        data = new char [infoHead.bV5SizeImage];
        in.read(data, infoHead.bV5SizeImage);
        in.close();
    }
    void rotate(){
        std::swap(infoHead.bV5Width, infoHead.bV5Height);
        LONG w = infoHead.bV5Width, h = infoHead.bV5Height;
        char* newdata = new char [infoHead.bV5SizeImage];
        for(int i = 0; i < h; ++i)
            for(int j = 0; j < w; ++j)
                for(int i1 = 0; i1 < 4; ++i1)
                    *(newdata + i * (w * 4) + j * 4 + i1) = *(data + j * (h * 4) + (h - 1 - i) * 4 + i1);//旋转，不用补零  
        delete [] data;
        data = new char [infoHead.bV5SizeImage];
        memcpy(data, newdata, infoHead.bV5SizeImage);
    }
    void writeBMP(char* output) const{
        std::ofstream of(output, std::ios::binary);
        of.write((char*)(&head), sizeof(head));
        of.write((char*)(&infoHead), sizeof(infoHead));
        of.write(data, infoHead.bV5SizeImage);
        delete [] data;
        of.close();
    }
private:
    BITMAPV5HEADER infoHead;//样例中的32-bit BMP采用BITMAPV5HEADER
};

int main(int argc, char** argv){
    if(argc != 3){
        cerr << "Two arguments needed" << endl;
        return 0;
    }
    BMP* inbmp;
    BMP24 temp24; BMP32 temp32;
    if(judgeBit(argv[1])) inbmp = &temp24;
    else inbmp = &temp32;
    inbmp->readBMP(argv[1]);
    inbmp->rotate();
    inbmp->writeBMP(argv[2]);
    return 0;
}