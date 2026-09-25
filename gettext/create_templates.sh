#!/bin/sh

# fist step is to generate a portable object template a POT file
xgettext -d my-domaine -o my-domaine.pot -L C ./multi_lang_utility.c

FR_FILE="fr-msg.po"

if [ -f "$FR_FILE" ]; then
	echo "update the existing frensh translation file"
	msgmerge --update fr-msg.po my-domaine.pot
	msgmerge --update es-msg.po my-domaine.pot
else 
	echo "generating po files"
	msginit -i ./my-domaine.pot --no-translator -o fr-msg.po -l=fr_FR.UTF-8
	msginit -i ./my-domaine.pot --no-translator -o es-msg.po -l=es_ES.UTF-8
fi

