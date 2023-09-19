#!/usr/bin/bash

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

if $( qdata=$(extract_question_info "$(grep 22056 questions.csv | csvcut -c"2,12")") ); then
	echo Valid question
else
	echo Invalid question
fi

#filtlist=$(csvcut -c"2,12" questions.csv)

# Quote variables to preserve newline
#while read -r item
#do
#	extract_question_info "$item"
#	if [ $? -ne 0 ]; then
#		echo "Invalid format"
#	fi
#done < <(echo "$filtlist")
