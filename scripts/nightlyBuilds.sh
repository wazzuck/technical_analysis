# !/bin/bash -x
# vim:set bg=dark number: #

echo "Running nightly build"

cd ~/vunderland/eod/src

echo "Pulling latest git updates"
gitStatus="$(git fetch --dry-run 2<&1)"

if [ "$gitStatus" == "" ]
then
	echo "Repo up-to-date no need to recompile"
else
	echo
	cd ~/vunderland
	git fetch
	cd ~/vunderland/eod/src
	make Eod
	echo "Subject: nightlybuilds.sh" | sendmail nevillebryce@gmail.com < ~/logs/nightlybuilds.log
fi
