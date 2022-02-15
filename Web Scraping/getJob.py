from gettext import find
from bs4 import BeautifulSoup
import requests
import re
import json


jobList = {}

class getJob:
    def __init__(self,keyword,location,experience,resultSize):
        self.keyword = keyword
        self.location = location
        self.experience = experience
        self.resultSize = resultSize
    def getUrl(self):
        url = f"https://www.timesjobs.com/candidate/job-search.html?searchType=personalizedSearch&luceneResultSize={resultSize}&postWeek=10&from=submit&txtKeywords={keyword}&txtLocation={location}&cboWorkExp1={experience}"
        jobs = requests.get(url).text
        soup = BeautifulSoup(jobs,'lxml')

        jobs = soup.find("ul",{"class":"new-joblist"}).find_all("li",{"class":"clearfix job-bx wht-shd-bx"})
        id = 1
        for job in jobs:
            jobName = job.find("a").text.strip()
            compName = job.find("h3",{"class":"joblist-comp-name"}).text.strip()
            description = job.find("ul",{"class":"list-job-dtl"}).find_all("li")[0].text
            l=len(description)
            description=description[:l-13].strip()
            skills = job.find("ul",{"class":"list-job-dtl"}).find("span",{"class":"srp-skills"}).text.strip()
            date = job.find("span",{"class":"sim-posted"}).text.strip()
            link = job.find("a").get("href")
            jobList.update({
            id :{
                'jobName':jobName,
                'compName':compName,
                'description':description,
                'skills':skills,
                'date':date,
                'link':link
                }
            })
            id+=1
            print(f"\nJob Name:{jobName}\nCompany: {compName}\n{description}\nSkills: {skills}\n{date}\nTo Apply:{link}")
            print("----------------------------------------")

keyword = input("Enter job keyword: ")
keyword = keyword.replace(" ","+")
location = input("Which location(If it doesnt matter, press enter): ")
experience = input("Whats your experince(Just number.Eg.3)")
resultSize = input("How many result you want to see?(Default 100)")

if len(experience) > 0:
    experience = int(re.search(r'\d+',experience).group(0))
if len(resultSize)<1:
    resultSize = 100
getJob = getJob(keyword,location,experience,resultSize)

if len(keyword)>0:
    getJob.getUrl()
    save = input("Save results press 1: ")
    save = save.strip()
    if save == "1":
        with open ("jobList.json","w") as f:
            json.dump(jobList,f)
else:
    print("Please enter at least one keyword")
