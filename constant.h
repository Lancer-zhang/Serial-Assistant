#ifndef CONSTANT_H
#define CONSTANT_H
#define             DATA_BUFFER                         0                                        //数据缓冲区

//数据帧控制 字节头 +
//static const int FINDING_55   = 100;
//static const int NEED_AA      = 101;
//static const int NEED_LENGTH  = 102;
//static const int NEED_NUM  = 103;

//static const int DATA_LEN_MAX = 32;
//static const int DATA_NUM_MAX = 0x05;
//static const int DATA_START   = 0;
//static const int DATA_END   = 105;

#define             DELAY_100MS                        100


enum DATA_FORMAT
{
    Utf_8,
    AscII,
    Binary,
    Octal,
    Decimal,
    Hexadecimal,
};
#endif // CONSTANT_H
