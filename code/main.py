from iplocate import get_location
import os.path as osp
import os
import sys

ispi2n = []
ispn2i = {}
isps = []

prvi2n = []
prvn2i = {}
prvs = []

pingres = {}


def ip2int(ip):
    ip = ip.split('.')
    return ((int(ip[0])*256 + int(ip[1]))*256 + int(ip[2]))*256 + int(ip[3])
def int2ip(i):
    ans = []
    for _ in range(4):
        ans.append(i % 256)
        i /= 256
    return '%d.%d.%d.%d' % (ans[3], ans[2], ans[1], ans[0])

def init_ISP():
    isp_root = '../ipdata/ISP/'
    for root, dirs, files in os.walk(isp_root):
        for file in files:
            isp_name = file[:2]
            ispi2n.append(isp_name)
            ispn2i[isp_name] = len(ispi2n) - 1
            iplist = []
            with open(osp.join(isp_root, file), 'r') as fin:
                for li in fin:
                    li = li.replace('\n', '').split()[:2]
                    li = [ip2int(_) for _ in li]
                    iplist.append(li)
            isps.append(iplist)

def init_province():
    prv_root = '../ipdata/province/'
    for root, dirs, files in os.walk(prv_root):
        for file in files:
            prv_name = file.split('.')[0]
            prvi2n.append(prv_name)
            prvn2i[prv_name] = len(prvi2n) - 1
            iplist = []
            with open(osp.join(prv_root, file), 'r') as fin:
                for li in fin:
                    li = li.replace('\n', '').split()[:2]
                    li = [ip2int(_) for _ in li]
                    iplist.append(li)
            prvs.append(iplist)

# 给定ip，获得该ip对应的运营商id
def get_ISP(ip):
    if isinstance(ip, str):
        ip = ip2int(ip)
    elif isinstance(ip, int):
        pass
    else:
        raise Exception("ip is not a str nor a integer.")

    for idx, isp in enumerate(isps):
        for ran in isp:
            if ip >= ran[0] and ip <= ran[1]:
                return idx
    return None

def run_ping(start):
    for i in range(start, len(prvi2n)):     # 逐省
        prv_name = prvi2n[i]
        prv_ips = prvs[i]
        for ran in prv_ips: # 每个IP段
            st = int2ip(ran[0])
            ed = int2ip(ran[1])
            a, b, c, _ = [int(_) for _ in st.split('.')]
            d, e, f, _ = [int(_) for _ in ed.split('.')]

            ipseg_list = (a, b, c, d, e, f)
            
            print('%3d.%3d.%3d  ---  %3d.%3d.%3d' % ipseg_list)
            if not os.path.exists('../final_result/%03d_%03d_%03d_%03d_%03d_%03d.txt' % ipseg_list):
                os.system('./run %d %d %d %d %d %d' % ipseg_list)

                # 为了节省空间，每次运行完一个ip段之后会把结果汇总成一个文件
                ipseg_sum, ipseg_alive_sum, ipseg_avgping_sum = 0.0, 0.0, 0.0
                with open(osp.join('../final_result/', '%03d_%03d_%03d_%03d_%03d_%03d.txt' % ipseg_list), 'w') as fout:
                    for root, dirs, files in os.walk('../result/'):
                        for file in files:
                            ip = file.split('.')[0].replace('_', '.') + '.0'
                            with open(osp.join(root, file), 'r') as fin:
                                alive, avgping = [float(_) for _ in fin.readline().split()]
                                ipseg_alive_sum += alive
                                ipseg_avgping_sum += avgping * alive
                                ipseg_sum += 256
                            fout.write('%.0f %.2f\n' % (alive, avgping))
                    os.system('rm ../result/*')
                print("%.0f/%.0f %.2f" % (ipseg_alive_sum, ipseg_sum, ipseg_avgping_sum / (ipseg_alive_sum if ipseg_alive_sum > 0 else 1)))

        print('Province %d [%s] complete.' % (i, prvi2n[i]))

if __name__ == '__main__':
    init_ISP()
    init_province()
    os.system('rm ../result/*')

    start = 0
    if len(sys.argv) > 1:
        start = int(sys.argv[1])
    
    run_ping(start)