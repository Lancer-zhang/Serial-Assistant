#ifndef CONSTANT_H
#define CONSTANT_H
#define             DATA_BUFFER                         0                                        //数据缓冲区

//数据帧控制 字节头
#define            FIND_FRAME_START            100
#define            NEED_FRAME_HEAD            101
#define            NEED_FRAME_LENGTH            102
#define            NEED_FRAME_NUM              103

#define            FRAME_START            0x55
#define            FRAME_HEAD             0xAA

#define            FRAME_MAX_LENGTH            32
#define            FRAME_MAX_NUMBER            0X05
#define            DATA_START            0
#define            DATA_END            105

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
//快乐小鸟游戏状态的枚举类型
enum GameStatus{GAMETITLE,
                GAMEREADY,
                GAMEPLAY,
                GAMEOVER};
//快乐小鸟奖牌获取的类型
enum Medal{PLATINICMEDAL = 12,
           GOLDMEDAL = 13,
           SILVERMEDAL = 14,
           COPPERMEDAL = 15,
          NONE = 0};
//快乐小鸟是否通过水管
enum PipeType{above=0,
              following=1};

//截图绘图状态
enum DrawStatus {
    waitDraw=0,
    drawing,
    drawed,
};
#endif // CONSTANT_H
