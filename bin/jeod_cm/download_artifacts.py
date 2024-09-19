#!/usr/bin/env python3

import os, subprocess, sys, time, shutil
import json
import requests
import hashlib
import uuid
from common import getGitlabToken

import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

token = getGitlabToken()

gitlab_url = 'https://esgl-gitlab.jsc.nasa.gov/api/v4'
headers = {'PRIVATE-TOKEN': token}

## Get IDs based on project name JEOD/jeod-dev
data = {}
geturl = '{0}/projects/JEOD%2Fjeod-dev'.format(gitlab_url)
response = requests.get(geturl, data=data, headers=headers, verify=False)
response.raise_for_status()
projects = response.json()
projectId=projects['id']
groupId=projects['namespace']['id']
project_url = '{0}/projects/{1}'.format(gitlab_url, projectId)
group_url = '{0}/groups/{1}'.format(gitlab_url, groupId)


scriptPath = os.path.realpath(__file__)
scriptRootPath = os.path.realpath(__file__ + '/../../../')
relPath = scriptPath.partition(scriptRootPath)[2][1:]
rootPath = os.getcwd()

if len(sys.argv) != 2:
  print("Usage: download_artifacts.py <Job ID>")
  sys.exit(-1)

data = {
       }
job_url = project_url + '/jobs/' + str(sys.argv[1])
response = requests.get(job_url, data=data, headers=headers,verify=False)
response.raise_for_status()
rjson = response.json()
jobHash = rjson['commit']['id']
ccmd = ['git', 'log', '--pretty=tformat:"%H"', '-1']
proc = subprocess.Popen(ccmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding="utf-8")
res = proc.communicate()
currHash = res[0].split('\n')[0]
print(currHash, jobHash, rjson['ref'])

if currHash != jobHash:
   print("Warning, the current hash of this repo is\n\t", currHash, "\nbut the artifacts downloaded were generated on hash\n\t", \
         jobHash, "\nin reference to\n\t", rjson['ref'])
   print("The regression data may be out-of-sync with the CI job test")
   yesNo = input("Would you like to continue downloading? (y/n) ")
   while True:
      if str(yesNo).lower() == 'n':
         print("Exiting..")
         sys.exit(-1)
      elif str(yesNo).lower() == 'y':
         print("Proceeding..")
         break
      else:
         print("Invalid response...")
         yesNo = input("Would you like to continue downloading? (y/n) ")

data = {
       }
artifacts_url = gitlab_url + '/jobs/' + str(sys.argv[1]) + '/artifacts'
outName = 'artifacts_' + str(sys.argv[1]) + '.zip'
if os.path.exists(outName):
  print("Removed old zip file")
  os.remove(outName)
if os.path.exists('artifacts'):
  print("Removed old artifacts directory")
  shutil.rmtree('artifacts')
print("Downloading job as", outName, "...")
response = requests.get(artifacts_url, data=data, headers=headers,verify=False)
response.raise_for_status()
with open(outName, 'wb') as fp:
   for chunk in response.iter_content(chunk_size=128):
      fp.write(chunk)
os.system('unzip ' + outName)

if __name__ == '__main__':
   print("Script complete")
   os.system("notify-send -t 1000000000 \"Your friendly neighborhood download_artifacts.py script reports: Script complete!\"")
sys.exit(0)
