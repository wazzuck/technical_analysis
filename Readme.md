### Technical-Analysis

A multithreaded program to read several years of historic data and perform various statistical analysis calculations outputting the data to a webpage which is used to sort, trend and graph the data to generate insights.

#### Calculations currently implemented

- Stochastics
- MACD
- RSI
- EMA

#### 3rd party libs used

Backend
- TALib for most of the calculations. https://ta-lib.org/
- Sqlite for outputing the data that is then read in by the Frontend. https://www.sqlite.org/

Frontend
* Bottle for generating the HTML. https://bottlepy.org/docs/dev/index.html
* Leather for SVG graphs. https://leather.readthedocs.io/en/latest/about.html
