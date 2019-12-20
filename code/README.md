## fping-4.2/
    - 修改了fping的源码，能使其把<能连接到的地址数量, 平均双向时延>输出到文件中。
    - fping [args] -Z filename
    - 安装方法：
        在 fping-4.2/下
        make
        sudo make install

## run.cpp
    - 编译： $ g++ run.cpp -lpthread -o run
    - 运行： $ sudo ./run a b c d e f
    探测 a.b.c.0 到 d.e.f.255 区间的fping结果，并保存到 ../result/x_y_z.txt 文件中。
    - 该程序开启了大量线程，可以快速得到结果

## iplocate.py
    get_location函数调用了百度地图API，给定一个字符串形式的ip地址，返回 (省份名，经度，纬度)

## main.py
    run_ping函数有一个可选参数：checkpoint，表示从第几个省份开始ping。