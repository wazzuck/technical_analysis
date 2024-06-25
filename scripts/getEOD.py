#!/usr/bin/python3

import os, shutil, urllib.request, smtplib, ssl, getopt, traceback, sys, yaml, textwrap, argparse

from ftplib import FTP
from datetime import datetime
from xlsx2csv import Xlsx2csv #Need to add to PYTHONPATH export PYTHONPATH=/home/neville/.local/lib/python2.7/site-packages/
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

from ftplib import FTP
from datetime import datetime
from xlsx2csv import Xlsx2csv #Need to add to PYTHONPATH export PYTHONPATH=/home/neville/.local/lib/python2.7/site-packages/
from email.mime.multipart import MIMEMultipart

with open('/home/neville/vunderland/settings/googleAPI.yaml', 'r') as file:
	email_data = yaml.safe_load(file)

port = email_data['port']
smtp_server = email_data['smtp_server']
sender_email = email_data['sender_email']
receiver_email = email_data['receiver_email']
password = email_data['password']

with open('/home/neville/vunderland/settings/eodData.yaml', 'r') as file:
		eod_data = yaml.safe_load(file)

U = eod_data['user']
P = eod_data['password']
URL = eod_data['url']

#############################################################################################
# Function getFile
#############################################################################################
def getFile(file):

	pathSplit=file.split("/")

	try:
		if len(pathSplit) == 1:
			print("Downloading " + file)
			ftp = FTP(URL)

			ftp.login(user=U, passwd=P)

			#ftp.retrlines('LIST')   -- Debug

			with open(file, 'wb') as fp:
				ftp.retrbinary("RETR " + file, fp.write)

			ftp.quit()

			print("Successfully downloaded " + file)

		else:
			print(len(pathSplit))
			print("using absolute method")

			path="/" + pathSplit[1]

			absoluteFile = pathSplit[2]
			directory = pathSplit[1]

			print(path)
			print(file)

			ftp = FTP(URL)
			ftp.login(user=U, passwd=P)
			ftp.cwd(path)
			#ftp.retrlines('LIST')   -- Debug

			with open(directory + "_" + absoluteFile, 'wb') as fp:
				ftp.retrbinary("RETR " + absoluteFile, fp.write)

			ftp.quit()

			print("Successfully downloaded " + file)

	except:
		global ERROR
		global email_body
		print("Error occurred downloading " + file)
		email_body+="Error downloading " + file + "\n"
		ERROR=True
			
#############################################################################################
# MAIN METHOD
#############################################################################################

#GLOBALS
ERROR=False
email_body=""

def main():

	parser = argparse.ArgumentParser(description="Script to download EOD files for calcuations")
	parser.add_argument("date", nargs='?', default=None, help="date to process")
	args = parser.parse_args()

	dateObject = datetime.now()

	if args.date is None:
		today = dateObject.strftime("%Y%m%d")
	else:
		today=sys.argv[1]

	downloadDirectory="/home/neville/data/" + today

	fileList = ["NASDAQ_" + today + ".txt",
				"NYSE_" + today + ".txt",
				"LSE_" + today + ".txt",
				"/Fundamentals/LSE.txt"
			   ]

	if os.path.isdir(downloadDirectory):
		print("Directory " + downloadDirectory + " exists. Removing")
		shutil.rmtree(downloadDirectory)

	os.mkdir(downloadDirectory)
	os.chdir(downloadDirectory)

	for file in fileList:
		getFile(file)

	print('Beginning file download with urllib2...')

	#TODO Check here for LSE reports
	#https://www.londonstockexchange.com/reports?tab=issuers

	opener = urllib.request.build_opener()
	opener.addheaders = [('User-agent', 'Mozilla/5.0')]
	urllib.request.install_opener(opener)

	instrumentList = eod_data['instrument_list']

	urllib.request.urlretrieve(instrumentList, downloadDirectory + '/lseCompanyList.xlsx')

	print('Converting .xlsx to .csv')
	Xlsx2csv(downloadDirectory + "/lseCompanyList.xlsx", outputencoding="utf-8").convert(downloadDirectory + "/lseCompanyList.csv")

	print("Deleting " + downloadDirectory + "/lseCompanyList.xlsx")
	os.remove(downloadDirectory + "/lseCompanyList.xlsx")

	if ERROR == True:
		print("An error occurred")
		msg = MIMEMultipart()
		msg['From'] = sender_email
		msg['To'] = receiver_email
		msg['Subject'] = 'Error Downloading Data'
		msg.attach(MIMEText(email_body, "plain"))
		# Create a secure SSL context
		context = ssl.create_default_context()

		context = ssl.create_default_context()
		with smtplib.SMTP(smtp_server, port) as server:
			server.ehlo()  # Can be omitted
			server.starttls(context=context)
			server.ehlo()  # Can be omitted
			server.login(sender_email, password)
			server.sendmail(sender_email, receiver_email, msg.as_string())
			print(traceback.print_exc(file=sys.stdout))

if __name__ == '__main__':
	main()

