#!/bin/bash

if [ $# -eq 0 ]; then
    echo "No arguments provided"
    exit 1
fi

#verif presence dossier temp sinon crea
if [ ! -d temp ];
then
        mkdir temp
else
    find temp -mindepth 1 -delete
fi

#verif présence dossier image sinon crea
if [ ! -d images ];
then
        mkdir images
fi

all_args=$*
input_dir=$1


for i in $all_args #print the help list
do
        case $i in

        "-h")
        echo "liste des commandes : -d1 ; -d2 ; -t ; -l ; -s "

        exit 0;;

        "-d1")

        option_d1

        exit 0;;

        "-d2")

        option_d2

        exit 0;;

        "-t")

        option_t

        exit 0;;

        "-l")

        option_l

        exit 0;;

        "-s")

        option_s

        exit 0;;


    esac
done
