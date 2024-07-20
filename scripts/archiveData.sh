#!/usr/bin/bash

mkdir ~/data/archive

cd ~/data/archive

tar cJvf eod_data_`date +'%d'`.tar.xz ~/data/2*

ln -sfn eod_data_`date +'%d'`.tar.xz data_latest.tar.xz
