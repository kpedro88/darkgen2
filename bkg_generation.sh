#!/bin/bash -e

case `uname` in
	Linux) ECHO="echo -e" ;;
	*) ECHO="echo" ;;
esac

usage() {
	$ECHO "bkg_generation.sh [options]"
	$ECHO
	$ECHO "Options:"
	$ECHO "-d            \tdirectory of premade cards (default = $PWD/mg5cards)"
	$ECHO "-p            \tprocess name (default = ttbar)"
	$ECHO "-c            \tcustom card name (default = process name)"
	$ECHO "-h            \tdisplay this message and exit"
	exit $1
}

CARDSDIR=$PWD/mg5cards
PROCNAME=ttbar
CUSTOMCARD=""

# check arguments
while getopts "d:p:c:h" opt; do
	case "$opt" in
	d) CARDSDIR=$OPTARG
	;;
	p) PROCNAME=$OPTARG
	;;
	c) CUSTOMCARD=$OPTARG
	;;
	h) usage 0
	;;
	esac
done

# set custom name to proc name if not set
if [ -z "$CUSTOMCARD" ]; then
	CUSTOMCARD=$PROCNAME
fi

if [ -z "$CARDSDIR" ] && [ -z "$PROCNAME" ]; then
	usage 1
fi

# check to make sure process or custom directory doesn't already exist
if [ -d ${PROCNAME} ]; then
	rm -rf ${PROCNAME}
fi
if [ -d ${CUSTOMCARD} ]; then
	rm -rf ${CUSTOMCARD}
fi

# Run the code-generation step to create the process directory
mg5_aMC ${CARDSDIR}/${PROCNAME}_proc_card.dat

# move generic process directory to specific custom directory for this job
if [ "$PROCNAME" != "$CUSTOMCARD" ]; then
	mv -v ${PROCNAME} ${CUSTOMCARD}
fi
cd ${CUSTOMCARD}

# Locating the run card

ORIGCARDS=(
${PROCNAME}_run_card.dat \
${PROCNAME}_param_card.dat \
pythia8_card.dat
)
NEWCARDS=(
run_card.dat \
param_card.dat \
pythia8_card.dat
)

for ((i=0; i < ${#ORIGCARDS[@]}; i++)); do
	ORIGCARD=${ORIGCARDS[$i]}
	NEWCARD=${NEWCARDS[$i]}
	if [ ! -e ${CARDSDIR}/${ORIGCARD} ]; then
		$ECHO ${CARDSDIR}/${ORIGCARD} " does not exist!"
		#exit 1;
	else
		cp ${CARDSDIR}/${ORIGCARD} ./Cards/${NEWCARD}
	fi
done

# Generate events

echo "done" > makegrid.dat
echo "./Cards/param_card.dat" >> makegrid.dat
# Specific customization for this job (nevents, seed, mass values)
if [ -e ${CARDSDIR}/${CUSTOMCARD}_customizecards.dat ]; then
	cat ${CARDSDIR}/${CUSTOMCARD}_customizecards.dat >> makegrid.dat
	echo "" >> makegrid.dat
fi
echo "done" >> makegrid.dat

# reset shell var because run_shower doesn't work in tcsh
export SHELL=bash
cat makegrid.dat | ./bin/generate_events pilotrun

$ECHO "End of job"
  
exit 0
