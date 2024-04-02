#!/usr/bin/python3

import os, shutil, urllib.request, smtplib, ssl, getopt, traceback, sys

#pip3 install xlstocsv pandas xlrd
from xlsToCsv import xlsToCsv

getcsv("/dev/shm/list_of_sets_securities_78.xls", "/dev/shm/list_of_sets_securities_78.csv")
