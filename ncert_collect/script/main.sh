#!/usr/bin/bash

# $1 is url; $2 is name to search;
# print full dirname containing keyword
extract_dirname_githubpage() {
	curl $1 -o extract_temp.txt &> /dev/null  
	dirname=$( grep -i $2 extract_temp.txt)
	dirname=${dirname##*title=\"}
	dirname=${dirname%%\"*}
	rm extract_temp.txt
	echo $dirname
}

# $1 record
# $2 column no
# $3 equality value
check_field_equals() {
	if [ $(echo $1 | cut -d, -f$2) == $3 ]
	then
		return 0
	else
		return 1
	fi
}

# $1 - String of question
# echo - String <Class-Chapter-Exercise-Qno>
# return 0 -> valid question, 1 -> invalid question
extract_question_info() {

	question=$(echo $1 | sed 's/ //g'| grep -iEo 'Q\([0-9]+\),.*Exercise.*,Class[0-9]+')
	
	if [ $? -ne 0 ]; then
		return 1
	fi
	
	class=${question##*[cC]lass}

	chapter=$(echo $question | cut -d, -f2)
	echo $question | grep -i "Misc" &> /dev/null
	if [ $? -eq 0 ]; then
		exercise=misc
		chapter=${chapter##*[cC]hapter}
	else
		exercise=$(echo $chapter| cut -d. -f2)
		chapter=$(echo $chapter| cut -d. -f1)
		chapter=${chapter##*[eE]xercise}
	fi

	qno=${question##*(}
	qno=${qno%%)*}

	echo "$class-$chapter-$exercise-$qno"
	return 0
}

process_record() {
	id=$(echo $1 | cut -d, -f$idcol)
	qrecord="$(grep $id questions.csv)"

	mkdir $id
	cd $id


	# Preprocessing on links
	link=$(echo $1 | cut -d, -f$linkcol)
	link=$(echo $link | sed "s/ /%20/g" )
	link=${link%%.git*}
	if [ $(echo $link | grep "/tree/main" | wc -l) -ne 0 ]
	then
		link_suffix=""
	else
		link_suffix="/tree/main"
	fi
	

	all_complete=1
	topdirname=""
	for counter in ${!lowdir[@]}
	do
		if $(check_field_equals "$1" $(( $startcol_mark + $counter )) $completed_entry); then

			qdata=$(extract_question_info "$(echo $qrecord | csvcut -c"$(( $startcol_question + $counter ))")")
			[ $? -ne 0 ] && continue

			if [ $(echo $topdirname | grep -i ${topdir[$counter]} | wc -l) -eq 0 ]
			then
				topdirname=$(extract_dirname_githubpage $link ${topdir[$counter]} | sed "s/ /%20/g")
			fi
			lowdirname=$(extract_dirname_githubpage $link$link_suffix/$topdirname ${lowdir[$counter]} | sed "s/ /%20/g")
			echo $topdirname : $lowdirname
			svn co $link/trunk/$topdirname/$lowdirname ${lowdir[$counter]} &> /dev/null

		else
			echo $id completed upto ${topdir[$(( $counter - 1 ))]}:${lowdir[$(( $counter - 1 ))]}
			all_complete=0
			break
	done
	
	if [ $all_complete -eq 1 ]
	then
		echo $id completed all
	fi

	cd ..
	rm -rf $id
}


# Global variables
linkcol=4
idcol=2
startcol_mark=14
startcol_question=9
topdir=("matrices" "matrices" "matrices" "optimization" "optimization")
lowdir=("line" "circle" "conic" "basic" "advanced")
completed_entry=5

# Iterating through command output
#filtlist=$( awk -F, -v checkcol="$startcol" '$checkcol==5 {print $0}' marks.csv )
#
process_record "$(grep Hamdan marks.csv)"
#process_record "$(grep -i chavva marks.csv)"

# Quote variables to preserve newline
#while read -r item
#do
#	process_record "$item"
#done < <(echo "$filtlist") 

# Function tester code
#if $(check_field_equals "$(grep Hamdan marks.csv)" 16 98)
#then
#	echo success
#else
#	echo failure
#fi
