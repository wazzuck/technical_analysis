# !/bin/bash -x
# vim:set bg=dark number: #

echo "Deploying to server"

buildCommand="cd ~/vunderland/eod/src/; git pull; make Eod; exit"

ssh neville@192.168.1.21 $buildCommand
