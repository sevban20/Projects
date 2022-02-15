from datetime import datetime

today = datetime.now()

class strToDt:
    def __init__(self):
        pass
        
    def transform(self,topTime):
        self.topTime = topTime
        if "Bugün" in self.topTime:
            fulldate = today.strftime("%d%m%y")+" " + self.topTime[8:]
            fulldate = datetime.strptime(fulldate,"%d%m%y %H:%M:%S")
        else:
            pass
        return fulldate