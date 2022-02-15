import requests
from bs4 import BeautifulSoup
from datetime import datetime
from getTime import strToDt as st


url1 = "https://forum.donanimhaber.com/sicak-firsatlar--f193?sayfa="
topList = []


class getPages:
    def __init__(self):
        self.temp =[]
        pass
    def page(self,url):
        self.url = url
        page = requests.get(self.url).text
        soup = BeautifulSoup(page,"lxml")
        pages = soup.find("div",{"class":"kl-icerik"}).find_all("div",class_="kl-icerik-satir")
        for page in pages:
            date = page.find("div", class_="kl-sonmesaj").find("span").text.strip().replace(" ","")[:19].strip()
            topicRow = page.get("class")
            if topicRow[1] !="reklam" and  len(topicRow)<3 and topicRow[1]=="yenikonu":
                topic = page.find("h3").text.strip()
                if "Bugün" in date and int(page.find("div",class_="kl-okunma").text.strip().replace(".","")[:6])<500:
                    list = []
                    link = page.find("a").get("href").strip()
                    link = "https://forum.donanimhaber.com" + link
                    list.append(topic)
                    list.append(link)
                    list.append(date)
                    if list in topList:
                        list = []
                        pass
                    else:
                        topList.append(list)
                        list = []
    def clearList(self):
        for i in topList:
           if (i[2]-datetime.today()).days == 1:
               topList.remove(i)
    def run(self):
        for i in range(0,20):
            url = url1 + str(i)
            self.page(url)
        if len(topList)>0 :
             for i in range(0,len(topList)):
                if type(topList[i][2]) == type("sa"):
                    topList[i][2] = st.transform(self,topList[i][2])
        if topList == self.temp:
            pass
        else:
            for i in topList:
                print(i[0])
                print(i[1])
                print(" ")
        self.temp = topList                  
    
    
getd = getPages()

while True:
    getd.run()

    
