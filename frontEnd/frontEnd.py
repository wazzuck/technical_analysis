#!/usr/bin/pypy3
# vim:set bg=dark number : #
import sqlite3, re, socket, sys, smtplib, ssl, leather, os, fcntl, struct, yaml
from bottle import route, get, post, request, static_file, template, run # or route
from datetime import date, datetime, timedelta

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

with open('/home/neville/vunderland/settings/googleAPI.yaml', 'r') as file:    
    email_data = yaml.safe_load(file)    
    
port = email_data['port']    
smtp_server = email_data['smtp_server']    
sender_email = email_data['sender_email']    
receiver_email = email_data['receiver_email']    
password = email_data['password']

"""

@get('/login') # or @route('/login')
def login():
	return '''
		<form action="/login" method="post">
			Username: <input name="username" type="text" />
			Password: <input name="password" type="password" />
			<input value="Login" type="submit" />
		</form>
	'''

def check_login(username, password):
	if username=="neville" and password=="moneyrea":
		return True
	else:
		return False

@post('/login') # or @route('/login', method='POST')
def do_login():
	username = request.forms.get('username')
	password = request.forms.get('password')
	if check_login(username, password):
		return "<p>Your login information was correct.</p>"
	else:
		return "<p>Login failed.</p>"
"""
def createPage(page="index"):
	html="""
<head>
	<style>
	table {
	  width:100%;
	}
	table, th, td {
	  border: 1px solid black;
	  border-collapse: collapse;
	}
	th, td {
	  padding: 15px;
	  text-align: left;
	}
	</style>
</head>

<table id="MyTable">
	<tr>
		<!--When a header is clicked, run the sortTable function, with a parameter-->
		<th onclick="sortTable(0)">Instrument</th>
		<th onclick="sortTable(1)">Industry</th>
		<th onclick="sortTable(2)">Sector</th>
		<th onclick="sortTable(3)">Market Segment Code</th>
		<th onclick="sortTable(4)">Yahoo</th>
		<th onclick="sortTable(5)">LSE</th>
	</tr>
"""

	""" Useful calculations
	Change SQL to select date, rather than last entry. Print date on webpage
	Average all prices in data set to see if market went up or down
	5 day average prices for each instrument
	Number of days an instruments has been going up or down
	Group instruments by sector to select tech, medicals etc

	Being able to 'flick' to previous day
	Look at interface to load specific market segment
	Look at interface to loads all data for an instruments.
	"""

	orderBySql=' ORDER BY PERCENTAGE_DIFFERENCE_1'

	#Note watch out for "," on second last item in the baseSql

	baseSql="""
		SELECT
		INSTRUMENT_NAME,
		MNEMONIC,
	    ICB_INDUSTRY,
	    ICB_SUPER_SECTOR_NAME,
	    MARKET_SEGMENT_CODE 
	    FROM REFDATA
	"""

	if re.search("^industry", page):
		sqlFilter=page.split("/")[1]
		sql=' WHERE ICB_INDUSTRY = "' + sqlFilter + '"'
		sql=baseSql + sql
	elif re.search("^sector", page):
		sqlFilter=page.split("/")[1]
		sql=' WHERE ICB_SUPER_SECTOR_NAME = "' + sqlFilter + '"'
		sql=baseSql + sql
	elif re.search("^search", page):
		sqlFilter=page.split("/")[1]
		"""
		if int(sqlFilter) > 0:
			sql=' WHERE PERCENTAGE_DIFFERENCE_1 > "' + sqlFilter + '"'
		else:
			sql=' WHERE PERCENTAGE_DIFFERENCE_1 < "' + sqlFilter + '"'
		sql=baseSql + sql
		"""
	elif re.search("^search_results", page):
		sqlFilter=page.split("/")[1]
		sql=' WHERE TECHNICAL.MNEMONIC = "' + sqlFilter + '"'
		sql=baseSql + sql
	elif page=="index":
		sql=baseSql

	#sql = sql + orderBySql

	sql = sql.strip()

	print (sql)

	print("Running SQL")
	cursor = connection.execute(sql)
	print("Finished SQL")

	for displayItem in enumerate(cursor):
		print(displayItem)

		displayString = str (displayItem)
		displayList = displayString.split(",")

		instrument_name = displayList[1][3:-1]
		mnemonic = displayList[2][2:-1]
		icb_industry = displayList[3][2:-1]
		icb_super_sector_name = displayList[4][2:-1]
		market_segment_code = displayList[5][2:-3]

		html+="\t<tr>\n"
		if page=="index":
			html+="\t\t<td><a href=instrument/" + mnemonic + " target=\"_blank\">" + instrument_name + "</a></td>\n"
		else:
			html+="\t\t<td>" + instrument_name + "</td>\n"
		#TODO Replace " " space char in cpp code / database

		if page=="index":
			html+="\t\t<td><a href=industry/" + icb_industry + " target=\"_blank\">" + icb_industry + "</a></td>\n"
			html+="\t\t<td><a href=sector/" + icb_super_sector_name + " target=\"_blank\">" + icb_super_sector_name + "</a></td>\n"
		else:
			html+="\t\t<td>" + icb_industry + "</td>\n"
			html+="\t\t<td>" + icb_super_sector_name + "</td>\n"

		html+="\t\t<td>" + market_segment_code + "</td>\n"
		html+="\t\t<td><a href=\"https://uk.finance.yahoo.com/quote/" + mnemonic + ".L\" target=\"_blank\">" + mnemonic + "</a></td>\n"
		#html+="\t\t<td><a href=\"https://duckduckgo.com/?q=%5C+" + mnemonic + "+londonstockexchange.com\" target=\"_blank\">" + mnemonic + "</a></td>\n"
		html+="\t\t<td><a href=\"https://duckduckgo.com/?q=\\soybeans\" target=\"_blank\">" + mnemonic + "</a></td>\n"
		html+="\t</tr>\n"                  

	"""
	for displayItem in enumerate(cursor):

		print(displayItem)
		displayString = str (displayItem)
		displayList = displayString.split(",")

		instrument_name = displayList[1][3:-1]
		icb_industry = displayList[2][2:-1]

		if (displayList[3][-1] == "'"):
			icb_super_sector_name = displayList[3][2:-1]
			market_segment_code = displayList[4][2:-1]
			market_cap = displayList[8]
			mnemonic = displayList[9][2:-1]
			print("Mnemonic " + mnemonic)
			ppo = displayList[10][1:-2]
		else:
			#There's a comma in the icb_super_sector_name field
			tempString = displayList[3] + "," + displayList[4]
			icb_super_sector_name = tempString[2:-1]
			market_segment_code = displayList[5][2:-1]
			market_cap = displayList[9]
			mnemonic = displayList[10][2:-1]
			print("Mnemonic " + mnemonic)
			ppo = displayList[11][1:-2]

		html+="\t<tr>\n"
		if page=="index":
			html+="\t\t<td><a href=instrument/" + mnemonic + " target=\"_blank\">" + instrument_name + "</a></td>\n"
		else:
			html+="\t\t<td>" + instrument_name + "</td>\n"
		#TODO Replace " " space char in cpp code / database
		if page=="index":
			html+="\t\t<td><a href=industry/" + icb_industry + " target=\"_blank\">" + icb_industry + "</a></td>\n"
			html+="\t\t<td><a href=sector/" + icb_super_sector_name + " target=\"_blank\">" + icb_super_sector_name + "</a></td>\n"
		else:
			html+="\t\t<td>" + icb_industry + "</td>\n"
			html+="\t\t<td>" + icb_super_sector_name + "</td>\n"

		html+="\t\t<td>" + market_segment_code + "</td>\n"
		html+="\t\t<td>" + str(ppo) + "</td>\n"
		html+="\t\t<td>" + str(market_cap) + "</td>\n"
		html+="\t\t<td><a href=\"https://uk.finance.yahoo.com/quote/" + mnemonic + ".L\" target=\"_blank\">" + mnemonic + "</a></td>\n"
		html+="\t\t<td><a href=\"https://duckduckgo.com/?q=!ducky+" + mnemonic + "+londonstockexchange.com\" target=\"_blank\">" + mnemonic + "</a></td>\n"
		html+="\t</tr>\n"

		break
	"""

	html+="""
</table>

<script>
//BUG This script works, but is very slow.
//
//After much thought javascript is probably too slow to sort large tables on the fly
//TODO Instead it is probably better to sort the data in the sqlite query and reload the table and refresh the page

function sortTable(n) {
  var table, rows, switching, i, x, y, shouldSwitch, dir, switchcount = 0;
  table = document.getElementById("MyTable");
  switching = true;
  // Set the sorting direction to ascending:
  dir = "asc";
  /* Make a loop that will continue until
  no switching has been done: */
  while (switching) {
    // Start by saying: no switching is done:
    switching = false;
    rows = table.rows;
    /* Loop through all table rows (except the
    first, which contains table headers): */
    for (i = 1; i < (rows.length - 1); i++) {
      // Start by saying there should be no switching:
      shouldSwitch = false;
      /* Get the two elements you want to compare,
      one from current row and one from the next: */
      x = rows[i].getElementsByTagName("TD")[n];
      y = rows[i + 1].getElementsByTagName("TD")[n];
      /* Check if the two rows should switch place,
      based on the direction, asc or desc: */
      if (dir == "asc") {
        if (x.innerHTML.toLowerCase() > y.innerHTML.toLowerCase()) {
          // If so, mark as a switch and break the loop:
          shouldSwitch = true;
          break;
        }
      } else if (dir == "desc") {
        if (x.innerHTML.toLowerCase() < y.innerHTML.toLowerCase()) {
          // If so, mark as a switch and break the loop:
          shouldSwitch = true;
          break;
        }
      }
    }
    if (shouldSwitch) {
      /* If a switch has been marked, make the switch
      and mark that a switch has been done: */
      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
      switching = true;
      // Each time a switch is done, increase this count by 1:
      switchcount ++;
    } else {
      /* If no switching has been done AND the direction is "asc",
      set the direction to "desc" and run the while loop again. */
      if (switchcount == 0 && dir == "asc") {
        dir = "desc";
        switching = true;
      }
    }
  }
}
</script>
"""
	return html

@route('/industry/<industry>')
def industryPage(industry):
	url="industry/" + industry
	industryPage = createPage(url)
	return industryPage

@route('/sector/<sector>')
def sectorPage(sector):
	url="sector/" + sector
	sectorPage = createPage(url)
	return sectorPage

def convertTupleValueToDate(tupleValue):
	dateString = tupleValue[0]
	year = int(dateString[0:4])
	month = int(dateString[4:6])
	day = int(dateString[6:8])
	dateValue = date(year, month, day)

	return dateValue

def createGraph(mnemonic):
	sql="SELECT DATE, CLOSE, EMAFAST, EMASLOW FROM " + mnemonic
	cursor = connection.execute(sql)

	queryReturnTuple = cursor.fetchall()
	#print("queryReturnTuple " + str(queryReturnTuple))
        
	startDateTuple = queryReturnTuple[-1]
	#print("startDateString " + str(startDateTuple[0]))
	startGraphDate = convertTupleValueToDate(startDateTuple)

	endDateTuple = queryReturnTuple[0]
	#print("endDateString " + str(endDateTuple[0]))
	endGraphDate = convertTupleValueToDate(endDateTuple)

	dateClosePriceTupleList = []
	dateEMAFastTupleList = []
	dateEMASlowTupleList = []
	
	for tupleValue in queryReturnTuple:
		print("value " + str(tupleValue))
		dateString = tupleValue[0]
		closePrice = tupleValue[1]
		emaFast = tupleValue[2]
		emaSlow = tupleValue[3]
		#print("dateString " + str(dateString) + " closePrice " + str(closePrice))

		priceDate = convertTupleValueToDate(tupleValue)

		dateClosePriceTuple = (priceDate, float(closePrice))
		dateClosePriceTupleList.append(dateClosePriceTuple)

		if emaFast != 0.0:
			dateEMAFastTuple = (priceDate, float(emaFast))
			dateEMAFastTupleList.append(dateEMAFastTuple)

		if emaSlow != 0.0:
			dateEMASlowTuple = (priceDate, float(emaSlow))
			dateEMASlowTupleList.append(dateEMASlowTuple)

	print(dateClosePriceTupleList)
	print(dateEMAFastTupleList)

	chart=leather.Chart(mnemonic)
	chart.add_x_scale(startGraphDate, endGraphDate)
	chart.add_line(dateClosePriceTupleList, name="Close Prices")
	chart.add_line(dateEMAFastTupleList, name="EMAFAST")
	chart.add_line(dateEMASlowTupleList, name="EMASLOW")
	chart.to_svg('/dev/shm/' + mnemonic + '.svg')

@route('/instrument/<mnemonic>')
def displayGraph(mnemonic):
	filename = mnemonic + ".svg"
	createGraph(mnemonic)
	graph_file=static_file(filename, root='/dev/shm/')

	return template('/home/neville/technical_analysis/frontEnd/graphFullScreen.tpl',param=filename)

"""Serve up the graph file to the client web browser"""
@route('/static/<filename>')
def serve_static(filename):
	return static_file(filename, root='/dev/shm')

@route('/table/<mnemonic>')
def displayTable(mnemonic):
	sql="SELECT * FROM " + mnemonic
	cursor = connection.execute(sql)
	sqlTuple = cursor.fetchall()
	html=""
	for entry in sqlTuple:
		html+="<p>" + str(entry) + "</p>\n"

	return html

@get('/search')
def search():

	"""
	BASIC DATA ANALYSIS

	Create a criteria search e.g.
	Percentage change > 5

	Serach by Industry or Sector

	Which fields are going to be availble?
	Start static and look at dynamic later
	TODO Get the list of fields from a schema from an SQL query?
	TODO Be able to choose multiple criteria like RACR using (i expect) javascript
	"""
	form = """
	<form action="/search" method="post">
		<label for="percentageChange">Percentage Change</label><br>
		<input type="text" id="pchange" name="pchange" value="3"><br>
		<input type="submit" value="Submit">
	</form> 
	"""
	return form

@post('/search') # or @route('/login', method='POST')
def do_search():
	search_item = request.forms.get('pchange')
	url="search/" + search_item
	searchResultsPage = createPage(url)
	return searchResultsPage

@route('/')
def index():
	indexPage = createPage()
	return indexPage

	"""
	TODO
	Add download database function
	"""

def exists(fileToCheck):
	if os.path.isfile(fileToCheck):
		print (fileToCheck + " does exist")
		return True
	else:
		print (fileToCheck + " does not exist")
		return False

#https://stackoverflow.com/questions/24196932/how-can-i-get-the-ip-address-from-nic-in-python
def getIPAdress(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', bytes(ifname[:15], 'utf-8'))
    )[20:24])

#############################################################################################
# GLOBALS
#############################################################################################

database_directory = "/dev/shm/"

#############################################################################################
# MAIN METHOD
#############################################################################################
if __name__ == '__main__':
	#TODO Based upon login, load portfolio
	#TODO Load 'total index' average as indicator of market state

	print ('Number of arguments:', len(sys.argv), 'arguments.')
	print ('Argument List:', str(sys.argv))

	if len(sys.argv) > 1:
		#TODO Think of a better var name than "date_in_filename_string"
		date_in_filename_string = sys.argv[1]
		db = database_directory + date_in_filename_string + ".eod.db"

		if not exists(db):
			print("Database not found.")
			sys.exit(1)

	else:
		date_in_filename = date.today()
		print("Today's date:", date_in_filename)

		if socket.gethostname() == "penguin":
			#More retries when debugging
			print("Running on laptop")
			num_retries = 100
		else:
			print("Running in live environment")
			num_retries = 5

		for x in range(num_retries):
			date_in_filename_string = date_in_filename.strftime("%Y%m%d")
			db = database_directory + date_in_filename_string + ".eod.db"

			if exists(db):
				break
			else:
				#TODO What is this for again?
				num_days_to_subtract = 1
				date_in_filename = date_in_filename - timedelta(days=num_days_to_subtract)

	connection = sqlite3.connect(db)
	cursor = connection.cursor()
	print("Connecting to " + db)

	if sys.stdin.isatty():
		print ("Running from terminal. Starting bottle")
	else:
		print ("Running from cron. Generating email and starting bottle")

		#Move email generator to above stanza for testing or put into a method and add an 'if check'
		
		#Generating gainers of > X%

		email_html = "<h1>Gainers</h1>"

		pchange="6"
		search_item = pchange
		url="search/" + search_item
		email_html += createPage(url)

		#Generating losers of < -X%
		email_html += "<h1>Losers</h1>"
		pchange="-6"
		search_item = pchange
		url="search/" + search_item
		email_html += createPage(url)

		msg = MIMEMultipart()
		msg['From'] = 'nevillebryce@gmail.com'
		msg['To'] = 'nevillebryce@gmail.com'
		msg['Subject'] = 'Today in the market'
		msg.attach(MIMEText(email_html, 'html'))
		# Create a secure SSL context
		context = ssl.create_default_context()

		context = ssl.create_default_context()
		with smtplib.SMTP(smtp_server, port) as server:
			server.ehlo()  # Can be omitted
			server.starttls(context=context)
			server.ehlo()  # Can be omitted
			server.login(sender_email, password)
			#server.sendmail(sender_email, receiver_email, msg.as_string())
			server.sendmail(sender_email, receiver_email, msg.as_string())

	if socket.gethostname() == "rpi":
		NIC = getIPAdress('end0')
	else:
		NIC="127.0.0.1"

	PORT=8080

	#Starting bottle
	run(host=NIC, port=PORT, debug=True, reloader=True)
