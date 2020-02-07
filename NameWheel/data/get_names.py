import codecs 
import re
import random
from urllib import error, request
from bs4 import BeautifulSoup

USER_AGENTS = ["Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 2.0.50727; Media Center PC 6.0)",
        "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 1.0.3705; .NET CLR 1.1.4322)",
        "Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.2; .NET CLR 1.1.4322; .NET CLR 2.0.50727; InfoPath.2; .NET CLR 3.0.04506.30)",
        "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN) AppleWebKit/523.15 (KHTML, like Gecko, Safari/419.3) Arora/0.3 (Change: 287 c9dfb30)",
        "Mozilla/5.0 (X11; U; Linux; en-US) AppleWebKit/527+ (KHTML, like Gecko, Safari/419.3) Arora/0.6",
        "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.2pre) Gecko/20070215 K-Ninja/2.1.1",
        "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9) Gecko/20080705 Firefox/3.0 Kapiko/3.0",
        "Mozilla/5.0 (X11; Linux i686; U;) Gecko/20070322 Kazehakase/0.4.5"]
boy = []
girl = []
url_list = []

def handle_request(url):
    user_agent = random.choice(USER_AGENTS)
    headers = {
        'User-Agent':user_agent
    }
    return request.Request(url=url, headers=headers)

def get_links(url):
    list = []
    try:
        soup = BeautifulSoup(request.urlopen(handle_request(url), timeout=10).read(), 'html.parser') 
    except Exception as e:
        print("出现异常:" + str(e))
        return 0
    for link in soup.find_all('a',class_='btn btn2'):
        link = 'http:' + link.attrs['href']
        link = re.sub("/name_list.html", "", link)
        print(link)
        list.append(link)
    return list

def get_name(file, url):
    try:
        soup = BeautifulSoup(request.urlopen(handle_request(url), timeout=20).read(), 'html.parser') 
    except Exception as e:
        print("访问 " + url + " 时出现异常:" + str(e))
        url_list.append(url)
        return 0
    surname = soup.find(name='div', style="text-align: center;").get_text()
    if surname.rfind("姓之家"):
        surname = re.sub("姓之家", '', surname)
    else:
        surname = re.sub("之家", '', surname)
    print('读取' + url + "\tsurname = " + surname)
    sur_count = len(surname)
    count = 0
    for name in soup.find_all(name='div', style="margin-top: 20px;"):
        _list = list(name.get_text())
        _list.insert(sur_count, ' ')
        file.write(''.join(_list) + '\n')
        count = count + 1
    print('读取完毕，读取了' + str(count) + '个名字')
    return count


if __name__ == '__main__':
    main_url = "http://www.resgain.net/xsdq.html"
    f_boy = open("boys.txt", "w+", encoding = 'UTF-8')
    f_girl = open("girls.txt", "w+", encoding = 'UTF-8')
    for i in range(4):
        if i == 0 :
            boy.append("/name/boys.html")
            girl.append("/name/girls.html")
        else :
            boy.append("/name/boys_" + str(i + 1) + ".html")
            girl.append("/name/girls_" + str(i + 1) + ".html")
    print("开始读取所有链接")
    urls = get_links(main_url)
    for url in urls:
        for sex in boy:
            url_list.append(url + sex)
        for sex in girl:
            url_list.append(url + sex)
    print("链接读取完毕")
    for url in url_list:
        print("读取：" + url)
        if(url.rfind("boys") == -1):
            get_name(f_girl, url)
        else:
            get_name(f_boy, url)
    f_boy.close()
    f_girl.close()