from urllib import parse,request
import json

# 给定一个字符串形式的ip地址，返回 (省份名，经度，纬度)
def get_location(ip):
    textmod = {
        'ip': ip,
        'ak': '6d476d649bbd2cd4efeb3f9da44beef1',
        'coor': 'bd09ll'
    }
    textmod = parse.urlencode(textmod)

    header_dict = {'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Trident/7.0; rv:11.0) like Gecko'}
    url = 'http://api.map.baidu.com/location/ip'

    req = request.Request(url='%s%s%s' % (url,'?',textmod),headers=header_dict)
    res = request.urlopen(req)
    res = res.read()

    t = json.loads(res.decode(encoding='utf-8'))
    if 'content' not in t:
        return None, None, None

    point = t['content']['point']
    return t['content']['address_detail']['province'], float(point['x']), float(point['y'])

# if __name__ == '__main__':
#     print(get_location('166.111.4.100'))