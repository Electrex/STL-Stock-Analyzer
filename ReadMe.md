# $TOCK Analyzer

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

$TOCK Analyzer is an application which compares two mutual funds performances overtime to determine which fund would yield a more profitable investment.


# Files

Input Files: Mutual Fund history data (1/2/2014 to 4/6/2020)


- VFINX.csv: NAV history file
- VTSMX.csv:  NAV history file
- VFINXdiv.csv:  dividend history file
- VTSMXdiv.csv:  dividend history file

# Input File format
VFINX / VTSMX (sorted by date, earliest to latest):
|Date      |Open      |High      |Low       |Close     |Adj Close|Volume|
|----------|----------|----------|----------|----------|---------|------|
|2000-01-03|134.039993|134.039993|134.039993|134.039993|94.997047|0     |
|2000-01-04|128.910004|128.910004|128.910004|128.910004|91.361305|0     |
|2000-01-05|129.160004|129.160004|129.160004|129.160004|91.538483|0     |
|...|...|...|...|..|...|...|

VFINXdiv / VTSMXdiv:
|Date      |Dividends|
|----------|---------|
|2007-12-21|0.75     |
|2008-09-25|0.635    |
|2003-12-26|0.47     |
|...|...|

# Output file
dividend_details.txt:
#### Dividends for VTSMX: 02/16/2016-02/15/2019
|Date   |   Dividend   |Shares  |  Div Amt | Sh Price| Purchased | New Sh Owned|
|----------|--------|-------|------|-----|-----|-------|
|06/13/2016|0.216   |213.904|46.20 |51.66|0.894|214.798|
|09/12/2016|0.250   |214.798|53.70 |53.84|0.997|215.796|
|12/19/2016|0.336   |215.796|72.51 |56.62|1.281|217.076|
|03/23/2017|0.247   |217.076|53.62 |58.48|0.917|217.993|
|06/20/2017|0.266   |217.993|57.99 |60.76|0.954|218.947|
|09/21/2017|0.253   |218.947|55.39 |62.41|0.888|219.835|
|12/20/2017|0.310   |219.835|68.15 |66.77|1.021|220.856|
|03/21/2018|0.259   |220.856|57.20 |67.82|0.843|221.699|
|06/21/2018|0.276   |221.699|61.19 |69.22|0.884|222.583|
|09/27/2018|0.326   |222.583|72.56 |72.79|0.997|223.580|
|12/21/2018|0.332   |223.580|74.23 |59.75|1.242|224.822|


#### Dividends for VFINX: 02/16/2016-02/15/2019
|Date   |   Dividend   |Shares  |  Div Amt | Sh Price| Purchased | New Sh Owned|
|----------|--------|-------|------|-----|-----|-------|
|03/18/2016|0.962|57.042|54.87|188.97|0.290|57.332|
|06/20/2016|0.908|57.332|52.06|192.15|0.271|57.603|
|09/12/2016|0.852|57.603|49.08|199.17|0.246|57.850|
|12/21/2016|1.254|57.850|72.54|208.90|0.347|58.197|
|03/21/2017|0.961|58.197|55.93|216.27|0.259|58.455|
|06/22/2017|0.960|58.455|56.12|224.71|0.250|58.705|
|09/19/2017|1.129|58.705|66.28|231.32|0.287|58.992|
|12/22/2017|1.126|58.992|66.42|247.63|0.268|59.260|
|03/23/2018|1.028|59.260|60.92|238.91|0.255|59.515|
|06/27/2018|1.098|59.515|65.35|249.21|0.262|59.777|
|09/25/2018|1.150|59.777|68.74|269.18|0.255|60.032|
|12/14/2018|1.224|60.032|73.48|239.88|0.306|60.339|

#### Dividends for VTSMX: 02/26/2007-12/18/2012
|Date   |   Dividend   |Shares  |  Div Amt | Sh Price| Purchased | New Sh Owned|
|----------|-----|-------|-----|-----|-----|-------|
|06/22/2007|0.130|284.091|36.93|36.35|1.016|285.107|
|09/21/2007|0.150|285.107|42.77|36.72|1.165|286.272|
|...|...|...|..|...|...|...|

# Console Output


```shell
Fund: VTSMX
Analysis Date: 02/15/2019
Latest NAV: $69.46
Minimum for the last year = $58.17
Maximum for the last year = $73.61
50 day moving average = $65.12
200 day moving average = $68.69
% gain over 200 Day Moving Avg: 1.13%
% return over the last 3 years: 56.16%

Fund: VFINX
Analysis Date: 02/15/2019
Latest NAV: $256.90
Minimum for the last year = $216.98
Maximum for the last year = $271.72
50 day moving average = $241.78
200 day moving average = $253.90
% gain over 200 Day Moving Avg: 1.18%
% return over the last 3 years: 55.01%
....
Average gain for VTSMX = 6990.82
Average gain for VFINX = 6564.69
```
