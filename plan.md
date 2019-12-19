IP地址 - 物理地址，所以可以通过分析数据集，来可视化全球IP地址资源分布。
DNS流量统计，可以把每个dns地址的流量数据做一些时间分析。

目前获取到的信息：
    dns流量数据包，记录了某校园网的DNS通信数据，在连续的10天里记录了超过4000个活跃用户的DNS请求信息。
    Campus DNS network traffic consisting of more than 4000 active users (in peak load hours) for 10 random days in the month of April-May, 2016 is available in hourly PCAP files in the dataset. (At present only traffic for Day0(Full) and Day1(partial) could be uploaded due to 10GB data limit)

全国IP地址分析 - 测量到每个省份的连接属性情况。
    首先拿到每个省市的ip地址范围，然后利用fping工具去统计信息：利用fping可以得到到每个省份的ping延时、ip地址可达性信息。
    然后参考ipv4地址空间测绘一文，利用百度api获取地图经纬度，统计如下信息：
        每个省份的平均ping达时间
            每个省份的ip范围可从 http://ips.chacuo.net/ 上获取
        每个省份不同运营商（教育网、电信、移动、联通、铁通、网通、长城、中科网、广电等）的平均ping达时间
            该信息可从 http://ipcn.chacuo.net/ 上获取
        每个省份的ip地址可达性情况（利用希尔伯特图）
            希尔伯特图的相关信息可以参考ipv4地址空间测绘一文
    
    保存数据格式：
        每个 /24 的子网是一个最小的单元。测试这个单元需要对这个 /24 子网进行扫描，获取平均ping用时，存活统计以及地理位置信息。
        /16 的子网保存为一个文件，记录如下信息：

        [x, y, average_ping, alive_count] * 256
        numpy array shape = (256, 4)

        一共有不到1GB数据。


