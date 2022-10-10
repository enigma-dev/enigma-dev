#!/bin/bash

head="<!DOCTYPE html><html><head><link rel=\"stylesheet\" href=\"reportstyle.css\"><script src=\"reportparser.js\" type=\"text/javascript\"></script><script id=\"testreportXML\" type=\"text/xml\"><?xml version=\"1.0\" encoding=\"UTF-8\"?>"

tail="</script></head><body onload=\"loader()\"></body></html>"

sed "1s|.*|$head|" test-harness-out/TestReport.xml > test-harness-out/report.html
echo $tail >> test-harness-out/report.html

cp reportparser.js reportstyle.css test-harness-out/
